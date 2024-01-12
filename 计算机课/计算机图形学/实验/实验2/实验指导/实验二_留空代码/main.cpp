#include "Angel.h"

#include <vector>
#include <string>
#include <fstream>
#include <string>

// #pragma comment(lib, "glew32.lib")

// 三角面片中的顶点序列
typedef struct vIndex {
	unsigned int a, b, c;
	vIndex(int ia, int ib, int ic) : a(ia), b(ib), c(ic) {}
} vec3i;

std::string filename;
std::vector<vec3i> faces;

int nVertices = 0;
int nFaces = 0;
int nEdges = 0;

int Automatic_rotation = 0;

const int X_AXIS = 0;
const int Y_AXIS = 1;
const int Z_AXIS = 2;

const int ANIMATION_START = 0;
const int ANIMATION_STOP = 1;
const int ANIMATION_AXIS = 2; 
const int TRANSFORM_SCALE = 5;
const int TRANSFORM_ROTATE = 6;
const int TRANSFORM_TRANSLATE = 7;
const int SHOW_STATUS = 8;
const int RESET = 9;

int rotateAxis = Y_AXIS;	// 旋转轴，默认为y轴

const double DELTA_DELTA = 0.1;		// Delta的变化率
const double DEFAULT_DELTA = 0.3;	// 默认的Delta值

double scaleDelta = DEFAULT_DELTA;
double rotateDelta = DEFAULT_DELTA;
double translateDelta = DEFAULT_DELTA;

glm::vec3 scaleTheta(1.0, 1.0, 1.0);     // 缩放控制变量
glm::vec3 rotateTheta(0.0, 0.0, 0.0);    // 旋转控制变量
glm::vec3 translateTheta(0.0, 0.0, 0.0);	// 平移控制变量

GLint matrixLocation;
int currentTransform = TRANSFORM_TRANSLATE;	// 设置当前变换
int mainWindow;

// 模型中的各个点
// const int NUM_VERTICES = 8;
const int NUM_VERTICES = 2904;
glm::vec3 vertices[NUM_VERTICES];

void read_off(const std::string filename)
{
	if (filename.empty()) {
		return;
	}
	std::ifstream fin;
	fin.open(filename);

	//// TODO:	完成对OFF格式三维模型文件的读取:

	fin.close();
}

void init()
{
	// 创建顶点数组对象
	GLuint vao[1];
	#ifdef __APPLE__	// for MacOS
		glGenVertexArraysAPPLE(1, vao);
		glBindVertexArrayAPPLE(vao[0]);
	#else				// for Windows
		glGenVertexArrays(1, vao);
		glBindVertexArray(vao[0]);
	#endif

	// 创建并初始化顶点缓存对象
	GLuint buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, nVertices * sizeof(glm::vec3), vertices, GL_STATIC_DRAW);

	// 创建并初始化顶点索引缓存对象
	GLuint vertexIndexBuffer;
	glGenBuffers(1, &vertexIndexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexIndexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, faces.size() * sizeof(vec3i), faces.data(), GL_STATIC_DRAW);

	// 读取着色器并使用
	std::string vshader, fshader;
	#ifdef __APPLE__	// for MacOS
		vshader = "shaders/vshader_mac.glsl";
		fshader = "shaders/fshader_mac.glsl";
	#else				// for Windows
		vshader = "shaders/vshader_win.glsl";
		fshader = "shaders/fshader_win.glsl";
	#endif
	GLuint program = InitShader(vshader.c_str(), fshader.c_str());
	glUseProgram(program);

	// 从顶点着色器中初始化顶点的位置
	GLuint pLocation = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(pLocation);
	glVertexAttribPointer(pLocation, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));


	/// /TODO: 获取变换矩阵的存储位置，并保存到matrixLocation：

	// 黑色背景
	glClearColor(0, 0, 0, 0);
}

void display()
{
	// 清理窗口
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// 生成变换矩阵
	glm::mat4 m(1.0, 0.0, 0.0, 0.0,
		0.0, 1.0, 0.0, 0.0,
		0.0, 0.0, 1.0, 0.0,
		0.0, 0.0, 0.0, 1.0);

	//// TODO: 计算变换矩阵:

	////TODO: 从指定位置matrixLocation中传入变换矩阵:

	// 绘制图形中的各个三角形
	glDrawElements(GL_TRIANGLES, int(faces.size() * 3), GL_UNSIGNED_INT, BUFFER_OFFSET(0));

}

// 根据Delta值更新Theta
void updateTheta(int axis, int sign) {
	switch (currentTransform) {
	case TRANSFORM_SCALE:
		scaleTheta[axis] += sign * scaleDelta;
		break;
	case TRANSFORM_ROTATE:
		rotateTheta[axis] += sign * rotateDelta;
		break;
	case TRANSFORM_TRANSLATE:
		translateTheta[axis] += sign * translateDelta;
		break;
	}
}

// 复原Theta和Delta
void resetTheta()
{
	scaleTheta = glm::vec3(1.0, 1.0, 1.0);
	rotateTheta = glm::vec3(0.0, 0.0, 0.0);
	translateTheta = glm::vec3(0.0, 0.0, 0.0);
	scaleDelta = DEFAULT_DELTA;
	rotateDelta = DEFAULT_DELTA;
	translateDelta = DEFAULT_DELTA;
}

// 更新变化Delta值
void updateDelta(int sign)
{
	switch (currentTransform) {
	case TRANSFORM_SCALE:
		scaleDelta += sign * DELTA_DELTA;
		break;
	case TRANSFORM_ROTATE:
		rotateDelta += sign * DELTA_DELTA;
		break;
	case TRANSFORM_TRANSLATE:
		translateDelta += sign * DELTA_DELTA;
		break;
	}
}

// 键盘操作函数
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	float tmp;
	glm::vec4 ambient;
	if (action == GLFW_PRESS) {
		switch (key)
		{
		case GLFW_KEY_ESCAPE: exit(EXIT_SUCCESS); break;
		// 手动旋转
		case GLFW_KEY_Q:
			updateTheta(X_AXIS, 1);
			break;
		case GLFW_KEY_A:
			updateTheta(X_AXIS, -1);
			break;
		case GLFW_KEY_W:
			updateTheta(Y_AXIS, 1);
			break;
		case GLFW_KEY_S:
			updateTheta(Y_AXIS, -1);
			break;
		case GLFW_KEY_E:
			updateTheta(Z_AXIS, 1);
			break;
		case GLFW_KEY_D:
			updateTheta(Z_AXIS, -1);
			break;
		// 设置旋转度数
		case GLFW_KEY_R:
			updateDelta(1);
			break;
		case GLFW_KEY_F:
			updateDelta(-1);
			break;
		case GLFW_KEY_T:
			resetTheta();
			break;
		////TODO: 添加代码完成键盘自动旋转动画的旋转轴选取：

			break;
		}
	}
}

//TODO: 修改mouse_button_callback函数，完成鼠标控制旋转的开始与停止：

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	
}

void prinfStatus()
{
	printf("\nCurrent Status:\n");
	switch (currentTransform) {
	case TRANSFORM_SCALE:
		printf("	Current Transform: Scale\n");
		printf("	scaleTheta: (%.2f, %.2f, %.2f)\n", scaleTheta.x, scaleTheta.y, scaleTheta.z);
		printf("	scaleDelta: %.2f\n", scaleDelta);
		break;
	case TRANSFORM_ROTATE:
		printf("	Current Transform: Rotate\n");
		printf("	rotateTheta: (%.2f, %.2f, %.2f)\n", rotateTheta.x, rotateTheta.y, rotateTheta.z);
		printf("	rotateDelta: %.2f\n", rotateDelta);
		break;
	case TRANSFORM_TRANSLATE:
		printf("	Current Transform: Translate\n");
		printf("	translateTheta: (%.2f, %.2f, %.2f)\n", translateTheta.x, translateTheta.y, translateTheta.z);
		printf("	translateDelta: %.2f\n", translateDelta);
		break;
	}
}

void printHelp() {
	printf("Mouse options:\n");
	printf("	Left Button: Start Animation\n");
	printf("	Right Button: Stop Animation\n");
	printf("\n\n");
	printf("Keyboard options:\n");
	printf("	Esc: Exit Program\n");
	printf("	R: Increase delta of currently selected transform\n");
	printf("	F: Decrease delta of currently selected transform\n");
	printf("	T: Reset all transformations and deltas\n");
	printf(" - While in animation\n");
	printf("	Z: Set Z Axis as the rotation axis\n");
	printf("	X: Set X Axis as the rotation axis\n");
	printf("	C: Set Y Axis as the rotation axis\n");
	printf(" - While not in animation\n");
	printf("	Q: Increase x\n");
	printf("	A: Decrease x\n");
	printf("	W: Increase y\n");
	printf("	S: Decrease y\n");
	printf("	E: Increase z\n");
	printf("	D: Decrease z\n");
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

int main(int argc, char **argv)
{

	// 初始化GLFW库，必须是应用程序调用的第一个GLFW函数
	glfwInit();

	// 配置GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// 配置窗口属性
	GLFWwindow* window = glfwCreateWindow(600, 600, "学号_姓名_作业二", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, key_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// 调用任何OpenGL的函数之前初始化GLAD
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// 读入OFF
	// read_off("cube.off");	// 读取立方体
	read_off("cow.off");		// 读取牛模型
	// Init mesh, shaders, buffer
	init();
	// 输出帮助信息
	printHelp();
	// 启用深度测试
	glEnable(GL_DEPTH_TEST);
	while (!glfwWindowShouldClose(window))
	{
		if (Automatic_rotation == 1)
			updateTheta(rotateAxis, 1);
		display();
		//reshape();

		// 交换颜色缓冲 以及 检查有没有触发什么事件（比如键盘输入、鼠标移动等）
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	return 0;

}

// 每当窗口改变大小，GLFW会调用这个函数并填充相应的参数供你处理。
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}