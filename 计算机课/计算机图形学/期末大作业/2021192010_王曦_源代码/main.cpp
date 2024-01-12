// #define STB_IMAGE_IMPLEMENTATION
// #include "stb_image.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shaders.h"
#include "camera.h"
#include "massPoint.h"
#include "rigid.h"
#include "spring.h"
#include "cloth.h"
#include "render.h"
#include "car.h"

#include <iostream>
#include <map>

float lastX = SCR_WIDTH / 2.0f, lastY = SCR_HEIGHT / 2.0f;  // 上一次焦点的坐标
bool firstMouse = true;  // 窗口是否是第一次获取焦点

// 控制摄像机速度的变量
float deltaTime = 0.0f;	 // 当前帧与上一帧的时间差
float lastFrame = 0.0f;  // 上一帧的时间

glm::vec3 lightPos(3.0f, 4.0f, 0.0f);  // 光源位置

// 控制 PCF 是否开启
bool pcf = false;
bool pcfKeyPressed = false;

// 控制 Gamma 是否开启
bool gamma = false;
bool gammaKeyPressed = false;

// 切换 Phong 模型与 Blinn-Phong 模型
bool blinn = false;
bool blinnKeyPressed = false;

// 控制风车是否开启
bool blowingKeyPressed = false;

// 控制仿真是否运行
bool running;  // 仿真是否运行
bool runningKeyPressed = false;

// 控制手电筒是否开启
bool torch = false;
bool torchKeyPressed = false;

// 切换布料的面绘制模式与线绘制模式
bool drawKeyPressed = false;

// 视角: 1 为自由摄像机, 2 为风车, 3 为自动环绕摄像机
int cameraMode = 1;

// ---------= 渲染场景 =---------

// 纹理
unsigned int floorTexture, marbleTexture, wallTexture;
unsigned int boxDiffuseTexture, boxSpecularTexture;
unsigned int skyboxTexture;
unsigned int grassTexture, windowTexture;

unsigned int quadVAO = 0, quadVBO;  // 正方形的 VAO 和 VBO

// 在归一化坐标中的 xy 平面绘制 1 * 1 的正方形
void renderQuad() {
    if (quadVAO == 0) {  // 初始化
        // 正方形的顶点
        float quadVertices[] = {
            // 顶点坐标          // 材质坐标
            -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
            -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
             1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
             1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        };

        glGenBuffers(1, &quadVBO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);

        glGenVertexArrays(1, &quadVAO);
        glBindVertexArray(quadVAO);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
    }

    // 绘制正方形
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}

unsigned int floorVAO;  // 地面的 VAO

Car car(glm::vec3(-1.0f, 0.0f, 4.0f));

// 渲染场景
void renderScene(const Shader& shader) {
    // ---------= 绘制地面 =---------

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, floorTexture);

    // 地面
    glm::mat4 model = glm::mat4(1.0f);
    shader.setMat4("model", model);
    glBindVertexArray(floorVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    // ---------= 绘制石头 =---------

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, marbleTexture);

    // 石头
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 2.0f));
    model = glm::rotate(model, glm::radians(60.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
    model = glm::scale(model, glm::vec3(0.25));
    shader.setMat4("model", model);
    renderCube();

    // ---------= 绘制车 =---------

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, wallTexture);

    car.draw(shader);
}

// ---------= 布料模拟 =---------

const float timeInterval = 0.01;  // 时间间隔

// 风
glm::vec3 wind;
glm::vec3 windStartPosition;  // 风的起点
glm::vec3 windDirection;  // 风向
bool windBlowing = false;  // 是否刮风
float windForceScale = 10.0f;  // 风力的大小

// 地面
glm::vec3 groundPosition(-5.0, -0.5, 0);  // 地面的位置
glm::vec2 groundSize(10, 10);  // 地面的大小
glm::vec4 groundColor(0.8, 0.8, 0.8, 1.0);  // 地面的颜色
Ground ground(groundPosition, groundSize, groundColor);

// 球
glm::vec3 ballPosition(0.0, 1.0, -3.0);  // 球的位置
float ballRadius = 1;  // 球的半径
glm::vec4 ballColor(0.988, 0.902, 0.788, 0.9);  // 球的颜色
Ball ball(ballPosition, ballRadius, ballColor);

// 布料
glm::vec3 clothPosition(-3.0, 5.5, -2.0);  // 布料的位置
glm::vec2 clothSize(6.0, 6.0);  // 布料的大小
Cloth cloth(clothPosition, clothSize);

// 重力加速度
glm::vec3 gravity(0.0, -9.8 / cloth.iterationFrequency, 0.0);
// glm::vec3 gravity(0.0, 0.0, 0.0);  // 失重

// ---------= 功能性函数 =---------

// 键盘输入的回调函数
void processInput(GLFWwindow* window) {
    // ESC: 关闭窗口
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // 不同摄像机模式下的控制
    if (cameraMode == 1) {  // 自由摄像机
        // W 、S 、A 、D : 控制摄像机前、后、左、右移动
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            camera.ProcessKeyboard(FORWARD, deltaTime);
        else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            camera.ProcessKeyboard(BACKWARD, deltaTime);
        else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            camera.ProcessKeyboard(LEFT, deltaTime);
        else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            camera.ProcessKeyboard(RIGHT, deltaTime);
    }
    else if (cameraMode == 2) {  // 风车
        // W 、S 、A 、D : 控制风车前、后、左、右移动
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            car.processKeyboard(CAR_FORWARD, deltaTime);
        else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            car.processKeyboard(CAR_BACKWARD, deltaTime);
        else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            car.processKeyboard(CAR_LEFT, deltaTime);
        else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            car.processKeyboard(CAR_RIGHT, deltaTime);
    }
    else if (cameraMode == 3) {  // 自动环绕摄像机
        // W 、S : 控制摄像机上、下移动
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            camera.ProcessKeyboard(UP, deltaTime);
        else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            camera.ProcessKeyboard(DOWN, deltaTime);
    }

    // 1 、2 、3 : 切换摄像机模式
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
        cameraMode = 1;

        std::cout << "[Camera]  Manual" << std::endl; 
    }
    else if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
        cameraMode = 2;

        std::cout << "[Camera]  Windmill" << std::endl; 
    }
    else if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) {
        cameraMode = 3;

        std::cout << "[Camera]  Auto" << std::endl; 
    }

    // P : 控制 PCF 是否开启
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS && !pcfKeyPressed)  {
        pcf = !pcf;
        pcfKeyPressed = true;

        if (pcf) std::cout << "[PCF]  On" << std::endl;
        else std::cout << "[PCF]  Off" << std::endl;
    }
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_RELEASE)  {
        pcfKeyPressed = false;
    }

    // G : 控制 Gamma 校正是否开启
    if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS && !gammaKeyPressed)  {
        gamma = !gamma;
        gammaKeyPressed = true;

        if (gamma) std::cout << "[Gamma]  On" << std::endl;
        else std::cout << "[Gamma]  Off" << std::endl;
    }
    if (glfwGetKey(window, GLFW_KEY_G) == GLFW_RELEASE)  {
        gammaKeyPressed = false;
    }

    // B : 切换 Phong 模型与 Blinn-Phong 模型
    if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS && !blinnKeyPressed)  {
        blinn = !blinn;
        blinnKeyPressed = true;

        if (blinn) std::cout << "[Light]  Blinn-Phong" << std::endl; 
        else std::cout << "[Light]  Phong" << std::endl; 
    }
    if (glfwGetKey(window, GLFW_KEY_B) == GLFW_RELEASE)  {
        blinnKeyPressed = false;
    }

    // T : 控制手电筒是否开启
    if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS && !torchKeyPressed)  {
        torch ^= 1;
        torchKeyPressed = true;

        if (torch) std::cout << "[Torch]  On" << std::endl; 
        else std::cout << "[Torch]  Off" << std::endl; 
    }
    if (glfwGetKey(window, GLFW_KEY_T) == GLFW_RELEASE)  {
        torchKeyPressed = false;
    }

    // ---------= 仿真 =---------
    // C : 控制风车是否开启
    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS && !blowingKeyPressed)  {
        car.blowing ^= 1;
        blowingKeyPressed = true;

        if (car.blowing) std::cout << "[Windmill]  On" << std::endl; 
        else std::cout << "[Windmill]  Off" << std::endl; 
    }
    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_RELEASE)  {
        blowingKeyPressed = false;
    }

    // - 、+ : 增减风车转速
    if (glfwGetKey(window, GLFW_KEY_MINUS) == GLFW_PRESS) 
        car.deltaTheta = std::max(car.deltaTheta - 2.5f, 2.5f);
    else if (glfwGetKey(window, GLFW_KEY_EQUAL) == GLFW_PRESS) 
        car.deltaTheta = std::min(car.deltaTheta + 2.5f, 10.0f);

    // L : (Lock) 控制仿真的开始和暂停
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS && !runningKeyPressed)  {
        running ^= 1;
        runningKeyPressed = true;

        if (running) std::cout << "[Simulation]  Running" << std::endl; 
        else std::cout << "[Simulation]  Pausing" << std::endl; 
    }
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_RELEASE)  {
        runningKeyPressed = false;
    }

    // F : 切换布料的面绘制模式与线绘制模式
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS && !drawKeyPressed)  {
        cloth.drawMode = (cloth.drawMode == Cloth::DRAW_FACES ? Cloth::DRAW_LINES : Cloth::DRAW_FACES);
        drawKeyPressed = true;

        if (cloth.drawMode == Cloth::DRAW_FACES) std::cout << "[Cloth]  Draw faces" << std::endl;
        else std::cout << "[Cloth]  Draw lines" << std::endl;
    }
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_RELEASE)  {
        drawKeyPressed = false;
    }

    // ↑ 、↓ 、← 、→ : 控制刮风
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS && running)
        cloth.applyForce(glm::vec3(0.0f, 0.0f, -windForceScale));
    else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS && running)
        cloth.applyForce(glm::vec3(0.0f, 0.0f, windForceScale));
    else if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS && running)
        cloth.applyForce(glm::vec3(-windForceScale, 0.0f, 0.0f));
    else if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS && running)
        cloth.applyForce(glm::vec3(windForceScale, 0.0f, 0.0f));

    // 取消布料上边两顶点的固定
    if (glfwGetKey(window, GLFW_KEY_9) == GLFW_PRESS)
        cloth.unpin(glm::vec2(0, 0));
    else if (glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS)
        cloth.unpin(glm::vec2(cloth.pointsPerRow - 1, 0));
}

// 鼠标输入的回调函数
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn) {
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    // 若窗口第一次获取焦点, 则直接设置 lastX 和 lastY
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    // 计算偏移量(单位: 像素)
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;  // 注意 y 轴从下往上指

    lastX = xpos;
    lastY = ypos;

    if (cameraMode == 1)  // 自由摄像机
        camera.ProcessMouseMovement(xoffset, yoffset);
    else if (cameraMode == 2)  // 风车
        car.processMouseMovement(xoffset, yoffset);
}

// 滚轮输入的回调函数
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

// 窗口改变大小的回调函数
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

// 加载正方体贴图
// 方向: +X (right), -X (left), +Y (top), -Y (bottom), +Z (front), -Z (back)
unsigned int loadCubemap(std::vector<std::string> faces) {
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++) {
        unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data) {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else {
            std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    return textureID;
}

int main() {
// ---------= 初始化 = ---------

    // 初始化 GLFW
    glfwInit();

	// 设置版本为 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	// 使用核心模式
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // 创建一个 SCR_WIDTH * SCR_HEIGHT 的、标题为 "2021192010_王曦_期末作业" 的窗口, 
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "2021192010_王曦_期末作业", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // 初始化 GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // 设置 I/O 回调函数
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);  // 设置键盘输入的回调函数
    glfwSetCursorPosCallback(window, mouse_callback);  // 设置鼠标输入的回调函数
    glfwSetScrollCallback(window, scroll_callback);  // 设置滚轮输入的回调函数
    
    // 隐藏并捕捉光标, 用于实现 FPS 的摄像机系统
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

// ---------= 设置着色器 = ---------

    // 物体着色器
	std::string vshader = "shaders/colors.vshader";
    std::string fshader = "shaders/colors.fshader";
    Shader objectShader(vshader.c_str(), fshader.c_str());
    // 深度贴图着色器
    vshader = "shaders/depth.vshader";
    fshader = "shaders/depth.fshader";
    Shader depthShader(vshader.c_str(), fshader.c_str());
    // debug 深度贴图着色器
    vshader = "shaders/debug_depth.vshader";
    fshader = "shaders/debug_depth.fshader";
    Shader debugDepthShader(vshader.c_str(), fshader.c_str());
    // 光源正方体着色器
    vshader = "shaders/lightCube.vshader";
    fshader = "shaders/lightCube.fshader";
    Shader lightCubeShader(vshader.c_str(), fshader.c_str());
    // 钢框箱子着色器
    vshader = "shaders/box.vshader";
    fshader = "shaders/box.fshader";
    Shader boxShader(vshader.c_str(), fshader.c_str());
    // 天空盒着色器
    vshader = "shaders/skybox.vshader";
    fshader = "shaders/skybox.fshader";
    Shader skyboxShader(vshader.c_str(), fshader.c_str());
    // 草的着色器
    vshader = "shaders/grass.vshader";
    fshader = "shaders/grass.fshader";
    Shader grassShader(vshader.c_str(), fshader.c_str());
    // 窗户的着色器
    vshader = "shaders/window.vshader";
    fshader = "shaders/window.fshader";
    Shader windowShader(vshader.c_str(), fshader.c_str());

// ---------= 绘制 = ---------

    // ---------= 设置物体的 VAO 、VBO 、属性指针 = ---------

    // ---------= 地面 =---------

    // 地面的 6 个顶点 (每个面 2 个三角形, 共 6 个顶点)
    float floorVertices[] = {
        // 顶点坐标              // 法向量          // 材质坐标
         25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,  25.0f,  0.0f,
        -25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,   0.0f,  0.0f,
        -25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,   0.0f, 25.0f,

         25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,  25.0f,  0.0f,
        -25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,   0.0f, 25.0f,
         25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,  25.0f, 25.0f
    };

    // 设置地面的 VBO 
    unsigned int floorVBO;  // VBO : 顶点缓冲对象
    // 生成 VBO 对象
    glGenBuffers(1, &floorVBO);
    // 将 VBO 绑定到 GL_ARRAY_BUFFER (顶点缓冲对象) 上
    glBindBuffer(GL_ARRAY_BUFFER, floorVBO);
    // 将定义的数据复制到当前绑定缓冲, 因三角形的顶点位置不改变, 则用 GL_STATIC_DRAW 形式
    glBufferData(GL_ARRAY_BUFFER, sizeof(floorVertices), &floorVertices, GL_STATIC_DRAW);
    // 设置地面的 VAO
    // 生成顶点数组对象
    glGenVertexArrays(1, &floorVAO);
    // 绑定 VAO
    glBindVertexArray(floorVAO);
    // 设置位置属性指针
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // 设置法向量属性指针
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // 设置纹理属性指针
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    // 解绑 VAO
    glBindVertexArray(0); 

    // ---------= 草 =---------

    // 草正方形的顶点
    float grassVertices[] = {
        // 顶点坐标           // 纹理坐标
        0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
        0.0f, -0.5f,  0.0f,  0.0f,  1.0f,
        1.0f, -0.5f,  0.0f,  1.0f,  1.0f,

        0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
        1.0f, -0.5f,  0.0f,  1.0f,  1.0f,
        1.0f,  0.5f,  0.0f,  1.0f,  0.0f
    };

    // 草的位置
    std::vector<glm::vec3> grassPosition {
        glm::vec3(5.0f, 0.0f, 5.0f),
        glm::vec3(6.0f, 0.0f, 6.0f), 
        glm::vec3(7.0f, 0.0f, 7.0f), 
        glm::vec3(8.0f, 0.0f, 8.0f), 
    };

    // 设置草的 VBO
    unsigned int grassVBO;  // VBO : 顶点缓冲对象
    // 生成 VBO 对象
    glGenBuffers(1, &grassVBO);
    // 将 VBO 绑定到 GL_ARRAY_BUFFER (顶点缓冲对象) 上
    glBindBuffer(GL_ARRAY_BUFFER, grassVBO);
    // 将定义的数据复制到当前绑定缓冲, 因三角形的顶点位置不改变, 则用 GL_STATIC_DRAW 形式
    glBufferData(GL_ARRAY_BUFFER, sizeof(grassVertices), &grassVertices, GL_STATIC_DRAW);
    // 设置草的 VAO
    unsigned int grassVAO;  // VAO: 顶点数组对象
    // 生成顶点数组对象
    glGenVertexArrays(1, &grassVAO);
    // 绑定 VAO
    glBindVertexArray(grassVAO);
    // 设置位置属性指针
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // 设置纹理属性指针
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // 解绑 VAO
    glBindVertexArray(0);

    // ---------= 窗户 =---------

    // 窗户正方形的顶点
    float windowVertices[] = {
        // 顶点坐标           // 纹理坐标
        0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
        0.0f, -0.5f,  0.0f,  0.0f,  1.0f,
        1.0f, -0.5f,  0.0f,  1.0f,  1.0f,

        0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
        1.0f, -0.5f,  0.0f,  1.0f,  1.0f,
        1.0f,  0.5f,  0.0f,  1.0f,  0.0f
    };

    // 窗户的位置
    std::vector<glm::vec3> windowPosition {
        glm::vec3(11.0f, 1.0f, 11.0f),
        glm::vec3(6.0f, 1.0f, 12.0f),
    };

    // 设置窗户的 VBO
    unsigned int windowVBO;  // VBO : 顶点缓冲对象
    // 生成 VBO 对象
    glGenBuffers(1, &windowVBO);
    // 将 VBO 绑定到 GL_ARRAY_BUFFER (顶点缓冲对象) 上
    glBindBuffer(GL_ARRAY_BUFFER, windowVBO);
    // 将定义的数据复制到当前绑定缓冲, 因三角形的顶点位置不改变, 则用 GL_STATIC_DRAW 形式
    glBufferData(GL_ARRAY_BUFFER, sizeof(windowVertices), &windowVertices, GL_STATIC_DRAW);
    // 设置窗户的 VAO
    unsigned int windowVAO;  // VAO: 顶点数组对象
    // 生成顶点数组对象
    glGenVertexArrays(1, &windowVAO);
    // 绑定 VAO
    glBindVertexArray(windowVAO);
    // 设置位置属性指针
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // 设置纹理属性指针
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // 解绑 VAO
    glBindVertexArray(0);

    // ---------= 加载纹理 = ---------

    floorTexture = loadTexture("textures/lawn.jpg");  // 草坪的纹理
    marbleTexture = loadTexture("textures/marble.jpg");  // 石头的纹理
    wallTexture = loadTexture("textures/wall.jpg");  // 风车的纹理
    boxDiffuseTexture = loadTexture("textures/container2.png");  // 钢框箱子的漫反射纹理
    boxSpecularTexture = loadTexture("textures/container2_specular.png");  // 钢框箱子的镜面反射纹理
    std::vector<std::string> faces {
        "textures/skybox/right.jpg",
        "textures/skybox/left.jpg",
        "textures/skybox/top.jpg",
        "textures/skybox/bottom.jpg",
        "textures/skybox/front.jpg",
        "textures/skybox/back.jpg"
    };
    skyboxTexture = loadCubemap(faces);  // 天空盒的纹理
    grassTexture = loadTexture("textures/grass.png");  // 草的纹理
    windowTexture = loadTexture("textures/window.png");

    // ---------= 深度贴图 = ---------

    const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;  // 深度贴图的分辨率
    
    // 设置深度的帧缓冲对象
    unsigned int depthMapFBO;
    glGenFramebuffers(1, &depthMapFBO);

    // 设置深度贴图
    unsigned int depthMap;
    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);

    // 加载深度贴图
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    // 将深度贴图的环绕方式设为 GL_CLAMP_TO_BORDER s.t. 采样深度贴图 [0, 1] 范围以外的区域时, 
    // 纹理函数总返回 1.0 的深度值和 0.0 的阴影值, 避免采样过多
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    GLfloat borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
    
    // 将深度贴图绑定到帧缓冲
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);

    // 将读写缓冲设为 GL_NONE , 表示不渲染任何颜色数据
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // 设置 uniform 变量前使用着色器程序对象
    objectShader.use();
    objectShader.setInt("diffuseTexture", 0);
    objectShader.setInt("shadowMap", 1);
    debugDepthShader.use();
    debugDepthShader.setInt("depthMap", 0);
    skyboxShader.use();
    skyboxShader.setInt("skybox", 0);

// ---------= 渲染 = ---------

    // 设置全局状态
    glEnable(GL_DEPTH_TEST);  // 启用深度测试
    glPointSize(3);  // 设置点大小为 3

    // 渲染类对象
    ClothRender clothRender(&cloth);
    ClothSpringRender clothSpringRender(&cloth);
    GroundRender groundRender(&ground);
    BallRender ballRender(&ball);

    // 给布料一个初始力, 让布料与球分离, 防止布料卡在球里
    glm::vec3 force(10.0, 40.0, 20.0);
    cloth.applyForce(force);

    running = true;

	// 渲染循环
    while (!glfwWindowShouldClose(window)) {
        // 计算控制摄像机速度的变量
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // 处理键盘输入
        processInput(window);

        // 设置背景色
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);  // 灰色
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // 清除颜色缓冲、深度缓冲

    // ---------= 绘制物体 = ---------

        // ---------= 渲染深度贴图 =---------
        glm::mat4 lightProjection, lightView;
        glm::mat4 lightSpaceMatrix;
        float near_plane = 1.0f, far_plane = 7.5f;
        lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
        lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
        lightSpaceMatrix = lightProjection * lightView;

        // 设置 uniform 变量前使用着色器程序对象
        depthShader.use();
        depthShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);

        // 临时设置视野
        glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        glClear(GL_DEPTH_BUFFER_BIT);
        // glActiveTexture(GL_TEXTURE0);
        // glBindTexture(GL_TEXTURE_2D, floorTexture);

        // 正面剔除修复悬浮
        glCullFace(GL_FRONT);
        renderScene(depthShader);
        // 复原背面剔除
        glCullFace(GL_BACK);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        // 复原视野
        glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // ---------= 绘制物体和光源 =---------
        objectShader.use();
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        objectShader.setMat4("projection", projection);
        objectShader.setMat4("view", view);
        objectShader.setVec3("viewPos", camera.Position);
        objectShader.setVec3("lightPos", lightPos);
        objectShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);
        objectShader.setInt("pcf", pcf);
        objectShader.setInt("blinn", blinn);
        objectShader.setInt("gamma", gamma);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, depthMap);
        renderScene(objectShader);

        // ---------= 绘制光源正方体 =---------
        lightCubeShader.use();
        lightCubeShader.setMat4("projection", projection);
        lightCubeShader.setMat4("view", view);
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.1f));  // 大小缩小为 0.1 倍
        lightCubeShader.setMat4("model", model);
        renderCube();

        // ---------= 绘制钢框箱子 =---------
        boxShader.use();
        // 设置矩阵
        projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        boxShader.setMat4("projection", projection);
        view = camera.GetViewMatrix();
        boxShader.setMat4("view", view);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(8.0f, 1.2f, 8.0f));
        model = glm::rotate(model, glm::radians(45.0f), glm::vec3(1.0f, 0.3f, 0.5f));
        model = glm::scale(model, glm::vec3(0.75f));
        boxShader.setMat4("model", model);
        // 设置材质
        boxShader.setInt("material.diffuse", 0);
        boxShader.setInt("material.specular", 1);
        boxShader.setFloat("material.shininess", 32.0f);
        // 设置聚光
        boxShader.setInt("torch", torch);
        boxShader.setFloat("light.cutOff", glm::cos(glm::radians(6.5f)));
        boxShader.setFloat("light.outerCutOff", glm::cos(glm::radians(12.5f)));
        // 设置光源
        boxShader.setVec3("light.position", camera.Position);
        boxShader.setVec3("light.direction", camera.Front);
        boxShader.setVec3("viewPos", camera.Position);
        boxShader.setVec3("light.ambient", 0.1f, 0.1f, 0.1f);
        boxShader.setVec3("light.diffuse", 0.75f, 0.75f, 0.75f);
        boxShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
        boxShader.setFloat("light.constant", 1.0f);
        boxShader.setFloat("light.linear", 0.09f);
        boxShader.setFloat("light.quadratic", 0.032f);
        // 激活并绑定漫反射纹理
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, boxDiffuseTexture);
        // 激活并绑定镜面反射纹理
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, boxSpecularTexture);
        // 绘制正方体
        renderCube();

        // ---------= 绘制草 =---------

        // 设置矩阵
        grassShader.use();
        view = camera.GetViewMatrix();
        projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        grassShader.setMat4("view", view);
        grassShader.setMat4("projection", projection);

        // 激活并绑定草的纹理
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, grassTexture);

        // 绑定草的 VAO 
        glBindVertexArray(grassVAO);

        // 绘制草
        for (unsigned int i = 0; i < grassPosition.size(); i++) {
            model = glm::mat4(1.0f);
            model = glm::translate(model, grassPosition[i]);
            model = glm::rotate(model, glm::radians(i * 45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
            grassShader.setMat4("model", model);

            // 绘制四边形的 6 个顶点
            glDrawArrays(GL_TRIANGLES, 0, 6);
        }

        // 解绑 VAO
        glBindVertexArray(0);

    // ---------= 视角切换 = ---------

        if (cameraMode == 2) {
            camera.Position = glm::vec3(car.position.x, car.position.y + 2.5f, car.position.z);
            camera.Front = car.front;
        }
        else if (cameraMode == 3) {
            float time = (float)glfwGetTime();
            glm::vec3 front = camera.Position - glm::vec3(0.0f);
            camera.Front = -front;

            camera.Position.x = 15.0f * cos(time / 2), camera.Position.z = 15.0f * sin(time / 2);
        }

    // ---------= 仿真 = ---------

        if (running) {
            // car.currentTime = (float)glfwGetTime();

            for (int i = 0; i < cloth.iterationFrequency; i++) {
                cloth.getForce(timeInterval, gravity);  // 计算当前受力
                cloth.integrate(timeInterval);  // 显式积分
                cloth.collisionResponse(&ground, &ball);  // 碰撞检测
            }

            // 更新各面的法向量
            cloth.getNormal();

            // 风车
            if (car.blowing) {
                windStartPosition = car.position;
                windDirection = glm::normalize(car.front);
                windForceScale = car.deltaTheta;
                wind = windDirection * windForceScale;
                cloth.applyForce(wind);
            }
        }

        // 绘制
        if (cloth.drawMode == Cloth::DRAW_LINES) clothSpringRender.draw();
        else clothRender.draw();

        ballRender.draw();
        // groundRender.draw();  // 仿真的地面与草地重合, 无需绘制

    // ---------= 渲染 debug 深度贴图 = ---------

        // 用于 debug 的深度贴图
        debugDepthShader.use();
        debugDepthShader.setFloat("near_plane", near_plane);
        debugDepthShader.setFloat("far_plane", far_plane);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, depthMap);
        // 渲染正方形
        // renderQuad();

        // 解绑 VAO
        // glBindVertexArray(0);

    // ---------= 绘制天空盒 = ---------

        // 绘制天空盒时将深度函数改为 <= 
        glDepthFunc(GL_LEQUAL);

        // 设置矩阵
        skyboxShader.use();
        view = glm::mat4(glm::mat3(camera.GetViewMatrix()));  // 移除位移
        skyboxShader.setMat4("view", view);
        skyboxShader.setMat4("projection", projection);

        // 激活并绑定纹理
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture);
       
        // 绘制正方体
        renderCube();

        // 恢复深度函数
        glDepthFunc(GL_LESS);

    // ---------= 绘制窗户 = ---------

        glEnable(GL_BLEND);  // 启用混合
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  // 设置半透明的混合函数

        // 设置矩阵
        windowShader.use();
        view = camera.GetViewMatrix();
        projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        windowShader.setMat4("view", view);
        windowShader.setMat4("projection", projection);

        // 激活并绑定窗户的纹理
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, windowTexture);

        // 绑定窗户的 VAO 和纹理
        glBindVertexArray(windowVAO);

        // 将窗户按深度非降序排列
        std::map<float, glm::vec3> sorted;
        for (unsigned int i = 0; i < windowPosition.size(); i++) {
            float distance = glm::length(camera.Position - windowPosition[i]);
            sorted[distance] = windowPosition[i];
        }
        // 先绘制深度大的窗户
        for (std::map<float, glm::vec3>::reverse_iterator it = sorted.rbegin(); it != sorted.rend(); ++it) {
            model = glm::mat4(1.0f);
            model = glm::translate(model, it->second);
            model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
            model = glm::scale(model, glm::vec3(2.0f));
            windowShader.setMat4("model", model);

            // 绘制四边形的 6 个顶点
            glDrawArrays(GL_TRIANGLES, 0, 6);
        }

    // ---------= 绘制 = ---------

        // 交换颜色缓冲, 用于此次迭代绘制
        glfwSwapBuffers(window);

        // 检查触发事件, 如键盘输入、鼠标移动、窗口大小改变等, 并调用对应的回调函数
        glfwPollEvents();
    }

// ---------= 结束 = ---------

    // 回收之前分配的空间
    // glDeleteVertexArrays(1, &floorVAO);
    // glDeleteBuffers(1, &floorVBO);

	// 释放之前分配的资源
    glfwTerminate();
	
    return 0;
}