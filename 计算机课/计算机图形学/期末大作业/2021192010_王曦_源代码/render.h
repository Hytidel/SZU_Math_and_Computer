#pragma once

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "shaders.h"
#include "camera.h"
#include "cloth.h"
#include "rigid.h"

#include <iostream>

// 加载纹理
unsigned int loadTexture(char const* path) {
    unsigned int textureID;
    glGenTextures(1, &textureID);
    
    int width, height, nrComponents;
    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data) {
        GLenum format;
        if (nrComponents == 1) format = GL_RED;
        else if (nrComponents == 3) format = GL_RGB;
        else if (nrComponents == 4) format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        // 有 alpha 纹理时, 以 GL_CLAMP_TO_EDGE 为环绕方式以避免看到半透明的纹理四边形
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }
    return textureID;
}

// const unsigned int SCR_WIDTH = 800, SCR_HEIGHT = 600;  // 窗口大小 (单位: 像素)
const unsigned int SCR_WIDTH = 1280, SCR_HEIGHT = 720;  // 窗口大小 (单位: 像素)
// const unsigned int SCR_WIDTH = 1920, SCR_HEIGHT = 1080;  // 窗口大小 (单位: 像素)

// 摄像机
Camera camera(glm::vec3(-12.0f, 4.0f, -3.0f));

// 光源类
struct Light {
    glm::vec3 position;
    glm::vec3 color;

    Light(glm::vec3 _position, glm::vec3 _color) : position(_position), color(_color) {}
};
Light light(glm::vec3(-5.0f, 7.0f, 6.0f), glm::vec3(1.0f, 0.5f, 0.3f));

// 刚体渲染父类
struct RigidRender {
// ---------= 成员变量 =---------    
    std::vector<Point*> faces;
    int pointsCnt;  // 每个面的质点数

    glm::vec4 color;  // 刚体颜色

    glm::vec3 *vboPosition;  // 位置坐标的 VBO
    glm::vec3 *vboNormal;  // 法向量的 VBO

    GLuint vaoID;
    GLuint vboIDs[2];

    GLint attribPointerPosition;  // 位置坐标指针
    GLint attribPointerNormal;  // 法向量指针

    Shader shader;

// ---------= 成员函数 =---------
    // 初始化
    void init(std::vector<Point*> _faces, glm::vec4 _color, glm::vec3 modelVector) {
        // ---------= 初始化成员变量 =---------
        faces = _faces;
        pointsCnt = faces.size();
        if (pointsCnt <= 0) {
            std::cout << "ERROR::RigidRender : No points exists." << std::endl;
            exit(-1);
        }

        color = _color;

        vboPosition = new glm::vec3[pointsCnt];
        vboNormal = new glm::vec3[pointsCnt];
        for (int i = 0; i < pointsCnt; i++) {
            vboPosition[i] = faces[i]->position;
            vboNormal[i] = faces[i]->normal;
        }

        // 生成 VAO 和 VBO
        glGenVertexArrays(1, &vaoID);
        glGenBuffers(2, vboIDs);
        // 绑定 VAO
        glBindVertexArray(vaoID);

        // 设置属性指针
        attribPointerPosition = 0, attribPointerNormal = 1;
        // 绑定位置坐标指针
        glBindBuffer(GL_ARRAY_BUFFER, vboIDs[0]);
        glVertexAttribPointer(attribPointerPosition, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
        glBufferData(GL_ARRAY_BUFFER, pointsCnt * sizeof(glm::vec3), vboPosition, GL_DYNAMIC_DRAW);
        // 绑定法向量指针
        glBindBuffer(GL_ARRAY_BUFFER, vboIDs[1]);
        glVertexAttribPointer(attribPointerNormal, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
        glBufferData(GL_ARRAY_BUFFER, pointsCnt * sizeof(glm::vec3), vboNormal, GL_DYNAMIC_DRAW);
        // 启动属性指针
        glEnableVertexAttribArray(attribPointerPosition);
        glEnableVertexAttribArray(attribPointerNormal);

        // ---------= 设置 uniform 变量 =---------
        // 初始化 shader 并使用
        shader = Shader("shaders/rigid.vshader", "shaders/rigid.fshader");
        shader.use();

        // 设置刚体颜色
        shader.setVec4("uniform_rigidColor", color);
        // 设置投影矩阵
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        shader.setMat4("uniform_projectionMatrix", projection);
        // 设置模型矩阵
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, modelVector);
        shader.setMat4("uniform_modelMatrix", model);
        // std::cout << model[0][0] << ' ' << model[0][1] << ' ' << model[0][2] << ' ' << model[0][3] << std::endl;
        // std::cout << model[1][0] << ' ' << model[1][1] << ' ' << model[1][2] << ' ' << model[1][3] << std::endl;
        // std::cout << model[2][0] << ' ' << model[2][1] << ' ' << model[2][2] << ' ' << model[2][3] << std::endl;
        // std::cout << model[3][0] << ' ' << model[3][1] << ' ' << model[3][2] << ' ' << model[3][3] << std::endl;
        // 设置光源位置和颜色
        shader.setVec3("uniform_lightPosition", light.position);
        shader.setVec3("uniform_lightColor", light.color);

        // ---------= 清空 =---------
        // 解绑 VAO 和 VBO
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    // 绘制刚体
    void draw() {
        // 刚体不移动, 故无需更新质点位置

        // 绑定 VAO 和 VBO
        glBindVertexArray(vaoID);
        glBindBuffer(GL_ARRAY_BUFFER, vboIDs[0]);
        glBufferSubData(GL_ARRAY_BUFFER, 0, pointsCnt * sizeof(glm::vec3), vboPosition);
        glBindBuffer(GL_ARRAY_BUFFER, vboIDs[1]);
        glBufferSubData(GL_ARRAY_BUFFER, 0, pointsCnt * sizeof(glm::vec3), vboNormal);

        // 设置观察矩阵
        shader.use();
        glm::mat4 view = camera.GetViewMatrix();
        shader.setMat4("uniform_viewMatrix", view);

        // 启用混合
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        // 绘制
        glDrawArrays(GL_TRIANGLES, 0, pointsCnt);

        // 关闭混合
        glDisable(GL_BLEND);

        // 解绑 shader
        glUseProgram(0);

        // 解绑 VAO 和 VBO
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
};

// 地面渲染类
struct GroundRender {
    Ground *ground;
    RigidRender render;

    GroundRender(Ground *_ground) : ground(_ground) {
        render.init(ground->faces, ground->color, ground->position);
    }

    // 绘制地面
    void draw() {
        render.draw();
    }
};

// 球渲染类
struct BallRender {
    Ball *ball;
    RigidRender render;

    BallRender(Ball *_ball) : ball(_ball) {
        render.init(ball->sphere->faces, ball->color, ball->center);
    }

    // 绘制球
    void draw() {
        render.draw();
    }
};

// 布料渲染类
struct ClothRender {
// ---------= 成员变量 =---------
    Cloth *cloth;
    int pointsCnt;  // 每个面的质点数

    glm::vec3 *vboPosition;  // 位置坐标的 VBO
    glm::vec2 *vboTexture;  // 材质坐标的 VBO
    glm::vec3 *vboNormal;  // 法向量的 VBO

    GLuint vaoID;
    GLuint vboIDs[3];
    GLuint textureID;

    GLint attribPointerPosition;  // 位置坐标指针
    GLint attribPointerTexture;  // 材质坐标指针
    GLint attribPointerNormal;  // 法向量指针

    Shader shader;

// ---------= 构造与析构 =---------
    ClothRender(Cloth *_cloth) : cloth(_cloth) {
        // ---------= 初始化成员变量 =---------
        pointsCnt = cloth->faces.size();
        if (pointsCnt <= 0) {
            std::cout << "ERROR::ClothRender : No points exists." << std::endl;
            exit(-1);
        }

        vboPosition = new glm::vec3[pointsCnt];
        vboTexture = new glm::vec2[pointsCnt];
        vboNormal = new glm::vec3[pointsCnt];
        for (int i = 0; i < pointsCnt; i++) {
            vboPosition[i] = cloth->faces[i]->position;
            vboTexture[i] = cloth->faces[i]->textureCoordinate;
            vboNormal[i] = cloth->faces[i]->normal;
        }

        // 生成 VAO 和 VBO
        glGenVertexArrays(1, &vaoID);
        glGenBuffers(3, vboIDs);
        // 绑定 VAO
        glBindVertexArray(vaoID);

        // 设置属性指针
        attribPointerPosition = 0, attribPointerTexture = 1, attribPointerNormal = 2;
        // 绑定位置坐标指针
        glBindBuffer(GL_ARRAY_BUFFER, vboIDs[0]);
        glVertexAttribPointer(attribPointerPosition, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
        glBufferData(GL_ARRAY_BUFFER, pointsCnt * sizeof(glm::vec3), vboPosition, GL_DYNAMIC_DRAW);
        // 绑定纹理坐标指针
        glBindBuffer(GL_ARRAY_BUFFER, vboIDs[1]);
        glVertexAttribPointer(attribPointerTexture, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
        glBufferData(GL_ARRAY_BUFFER, pointsCnt * sizeof(glm::vec2), vboTexture, GL_DYNAMIC_DRAW);
        // 绑定法向量指针
        glBindBuffer(GL_ARRAY_BUFFER, vboIDs[2]);
        glVertexAttribPointer(attribPointerNormal, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
        glBufferData(GL_ARRAY_BUFFER, pointsCnt * sizeof(glm::vec3), vboNormal, GL_DYNAMIC_DRAW);
        // 启动属性指针
        glEnableVertexAttribArray(attribPointerPosition);
        glEnableVertexAttribArray(attribPointerTexture);
        glEnableVertexAttribArray(attribPointerNormal);

        // ---------= 加载纹理 =---------
        textureID = loadTexture("textures/cloth.jpg");
        glBindTexture(GL_TEXTURE_2D, textureID);

        // 反转 y 轴
        stbi_set_flip_vertically_on_load(true);

        // ---------= 设置 uniform 变量 =---------
        // 初始化 shader 并使用
        shader = Shader("shaders/cloth.vshader", "shaders/cloth.fshader");
        shader.use();

        // 设置纹理采样器
        shader.setInt("uniform_texture", 0);
        // 设置投影矩阵
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        shader.setMat4("uniform_projectionMatrix", projection);
        // 设置模型矩阵
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, cloth->position);
        shader.setMat4("uniform_modelMatrix", model);
        // 设置光源位置和颜色
        shader.setVec3("uniform_lightPosition", light.position);
        shader.setVec3("uniform_lightColor", light.color);

        // ---------= 清空 =---------
        // 解绑 VAO 和 VBO
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    // 绘制布料
    void draw() {
        // 更新质点位置
        for (int i = 0; i < pointsCnt; i++) {
            vboPosition[i] = cloth->faces[i]->position;
            vboNormal[i] = cloth->faces[i]->normal;
        }

        // 绑定 VAO 和 VBO
        glBindVertexArray(vaoID);
        glBindBuffer(GL_ARRAY_BUFFER, vboIDs[0]);
        glBufferSubData(GL_ARRAY_BUFFER, 0, pointsCnt * sizeof(glm::vec3), vboPosition);
        glBindBuffer(GL_ARRAY_BUFFER, vboIDs[1]);
        glBufferSubData(GL_ARRAY_BUFFER, 0, pointsCnt * sizeof(glm::vec2), vboTexture);
        glBindBuffer(GL_ARRAY_BUFFER, vboIDs[2]);
        glBufferSubData(GL_ARRAY_BUFFER, 0, pointsCnt * sizeof(glm::vec3), vboNormal);

        // 绑定纹理
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureID);

        // 设置观察矩阵
        shader.use();
        glm::mat4 view = camera.GetViewMatrix();
        shader.setMat4("uniform_viewMatrix", view);

        // 启用混合
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        // 绘制
        switch (cloth->drawMode) {
            case Cloth::DRAW_NODES:
                glDrawArrays(GL_POINTS, 0, pointsCnt);
                break;
            case Cloth::DRAW_LINES:
                glDrawArrays(GL_LINES, 0, pointsCnt);
                break;
            default:
                glDrawArrays(GL_TRIANGLES, 0, pointsCnt);
                break;
        }

        // 关闭混合
        glDisable(GL_BLEND);

        // 解绑 shader
        glUseProgram(0);

        // 解绑 VAO 和 VBO
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
};

// 弹簧渲染类
struct SpringRender {
// ---------= 成员变量 =---------    
    std::vector<Spring*> springs;
    int springsCnt;  // 弹簧数

    glm::vec4 color;  // 弹簧颜色

    glm::vec3 *vboPosition;  // 位置坐标的 VBO
    glm::vec3 *vboNormal;  // 法向量的 VBO

    GLuint vaoID;
    GLuint vboIDs[2];

    GLint attribPointerPosition;  // 位置坐标指针
    GLint attribPointerNormal;  // 法向量指针

    Shader shader;

// ---------= 成员函数 =---------
    // 初始化
    void init(std::vector<Spring*> _springs, glm::vec4 _color, glm::vec3 modelVector) {
        // ---------= 初始化成员变量 =---------
        springs = _springs;
        springsCnt = springs.size();
        if (springsCnt <= 0) {
            std::cout << "ERROR::RigidRender : No points exists." << std::endl;
            exit(-1);
        }

        color = _color;

        // 每个弹簧有两个质点
        vboPosition = new glm::vec3[springsCnt * 2];
        vboNormal = new glm::vec3[springsCnt * 2];
        for (int i = 0; i < springsCnt; i++) {
            vboPosition[i * 2] = springs[i]->point1->position;
            vboPosition[i * 2 + 1] = springs[i]->point2->position;
            vboNormal[i * 2] = springs[i]->point1->normal;
            vboNormal[i * 2 + 1] = springs[i]->point2->normal;
        }

        // 生成 VAO 和 VBO
        glGenVertexArrays(1, &vaoID);
        glGenBuffers(2, vboIDs);
        // 绑定 VAO
        glBindVertexArray(vaoID);

        // 设置属性指针
        attribPointerPosition = 0, attribPointerNormal = 1;
        // 绑定位置坐标指针
        glBindBuffer(GL_ARRAY_BUFFER, vboIDs[0]);
        glVertexAttribPointer(attribPointerPosition, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
        glBufferData(GL_ARRAY_BUFFER, springsCnt * 2 * sizeof(glm::vec3), vboPosition, GL_DYNAMIC_DRAW);
        // 绑定法向量指针
        glBindBuffer(GL_ARRAY_BUFFER, vboIDs[1]);
        glVertexAttribPointer(attribPointerNormal, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
        glBufferData(GL_ARRAY_BUFFER, springsCnt * 2 * sizeof(glm::vec3), vboNormal, GL_DYNAMIC_DRAW);
        // 启动属性指针
        glEnableVertexAttribArray(attribPointerPosition);
        glEnableVertexAttribArray(attribPointerNormal);

        // ---------= 设置 uniform 变量 =---------
        // 初始化 shader 并使用
        shader = Shader("shaders/spring.vshader", "shaders/spring.fshader");
        shader.use();

        // 设置刚体颜色
        shader.setVec4("uniform_springColor", color);
        // 设置投影矩阵
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        shader.setMat4("uniform_projectionMatrix", projection);
        // 设置模型矩阵
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, modelVector);
        shader.setMat4("uniform_modelMatrix", model);
        // 设置光源位置和颜色
        shader.setVec3("uniform_lightPosition", light.position);
        shader.setVec3("uniform_lightColor", light.color);

        // ---------= 清空 =---------
        // 解绑 VAO 和 VBO
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    // 绘制弹簧
    void draw() {
        // 更新质点位置
        for (int i = 0; i < springsCnt; i++) {
            vboPosition[i * 2] = springs[i]->point1->position;
            vboPosition[i * 2 + 1] = springs[i]->point2->position;
            vboNormal[i * 2] = springs[i]->point1->normal;
            vboNormal[i * 2 + 1] = springs[i]->point2->normal;
        }

        // 绑定 VAO 和 VBO
        glBindVertexArray(vaoID);
        glBindBuffer(GL_ARRAY_BUFFER, vboIDs[0]);
        glBufferSubData(GL_ARRAY_BUFFER, 0, springsCnt * 2 * sizeof(glm::vec3), vboPosition);
        glBindBuffer(GL_ARRAY_BUFFER, vboIDs[1]);
        glBufferSubData(GL_ARRAY_BUFFER, 0, springsCnt * 2 * sizeof(glm::vec3), vboNormal);

        // 设置观察矩阵
        shader.use();
        glm::mat4 view = camera.GetViewMatrix();
        shader.setMat4("uniform_viewMatrix", view);

        // 启用混合
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        // 绘制
        glDrawArrays(GL_LINES, 0, springsCnt * 2);

        // 关闭混合
        glDisable(GL_BLEND);

        // 解绑 shader
        glUseProgram(0);

        // 解绑 VAO 和 VBO
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
};

// 衣服与弹簧渲染类
struct ClothSpringRender {
// ---------= 成员变量 =---------
    Cloth *cloth;
    SpringRender render;

    glm::vec4 color;

// ---------= 构造与析构 =---------
    ClothSpringRender(Cloth *_cloth) : cloth(_cloth) {
        color = glm::vec4(1.0);
        render.init(cloth->springs, color, cloth->position);
    }

// ---------= 成员函数 =---------
    void draw() {
        render.draw();
    }
};
