#include "Angel.h"
#include <string>

const glm::vec3 WHITE(1.0, 1.0, 1.0);
const glm::vec3 BLACK(0.0, 0.0, 0.0);
const glm::vec3 RED(1.0, 0.0, 0.0);
const glm::vec3 GREEN(0.0, 1.0, 0.0);
const glm::vec3 BLUE(0.0, 0.0, 1.0);

const int TRIANGLE_NUM_POINTS = 3;
const int SQUARE_NUM = 6;
const int SQUARE_NUM_POINTS = 4 * SQUARE_NUM; // 获得三角形的每个角度

double getTriangleAngle(int point) {
	return 2 * M_PI / 3 * point;
}

// 获得正方形的每个角度
double getSquareAngle(int point) {
	return M_PI / 4 + (M_PI / 2 * point);
}

// 生成三角形上的每个点
void generateTrianglePoints(glm::vec2 vertices[], glm::vec3 colors[], int startVertexIndex)
{
	// 三角形尺寸
	glm::vec2 scale(0.25, 0.25);
	// 三角形中心位置
	glm::vec2 center(0.0, 0.70);

	// @TODO: 在此函数中修改三角形的顶点位置
	for (int i = 0; i < 3; i++) {
		// 当前顶点对应的角度
		double currentAngle = getTriangleAngle(i);
		// 根据角度及三角形中心计算顶点坐标
		vertices[startVertexIndex + i] = glm::vec2(cos(currentAngle), sin(currentAngle)) * scale + center;
	}

	// 确定三个顶点的颜色
	colors[startVertexIndex] = RED;
	colors[startVertexIndex + 1] = BLUE;
	colors[startVertexIndex + 2] = GREEN;
}

// 生成正方形上的每个点
void generateSquarePoints(glm::vec2 vertices[], glm::vec3 colors[], int squareNum, int startVertexIndex)
{
	// 最大正方形尺寸
	glm::vec2 scale(0.90, 0.90);
	// 正方形中心位置
	glm::vec2 center(0.0, -0.25);

	glm::vec3 currentColor = WHITE;
	int vertexIndex = startVertexIndex;

	// @TODO: 在此函数中修改，生成多个嵌套正方形
	// 根据正方形及顶点对应角度，计算当前正方形四个顶点坐标
	for (int j = 0; j < 4; j++)
	{
	    double currentAngle = getSquareAngle(j);
	    vertices[vertexIndex] = glm::vec2(cos(currentAngle), sin(currentAngle)) * scale + center;
	    colors[vertexIndex] = currentColor;
	    vertexIndex++;
	}
}

// 全局变量，两个vao，一个绘制三角形，一个绘制正方形
GLuint vao[2], program;
void init()
{

	// 定义三角形的三个点
	glm::vec2 triangle_vertices[TRIANGLE_NUM_POINTS];
	glm::vec3 triangle_colors[TRIANGLE_NUM_POINTS];
	// 定义矩形的点
	glm::vec2 square_vertices[SQUARE_NUM_POINTS];
	glm::vec3 square_colors[SQUARE_NUM_POINTS];

	// 调用生成形状顶点位置的函数
	generateTrianglePoints(triangle_vertices, triangle_colors, 0);
	generateSquarePoints(square_vertices, square_colors, SQUARE_NUM, 0);


	// 读取着色器并使用
	std::string vshader, fshader;
	vshader = "shaders/vshader.glsl";
	fshader = "shaders/fshader.glsl";
	program = InitShader(vshader.c_str(), fshader.c_str());
	glUseProgram(program);

	/*
	* 初始化三角形的数据
	*/
	// 创建顶点数组对象
	glGenVertexArrays(1, &vao[0]);		// 分配1个顶点数组对象
	glBindVertexArray(vao[0]);			// 绑定顶点数组对象

// 创建顶点缓存对象，vbo[2]是因为我们将要使用两个缓存对象，
// 一个是顶点坐标，一个是顶点颜色
	GLuint vbo[2];

	// 分配1个顶点缓存对象
	glGenBuffers(1, &vbo[0]);
	// 绑定顶点缓存对象
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	// 分配数据所需的存储空间，将数据拷贝到OpenGL服务端内存
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_vertices), triangle_vertices, GL_STATIC_DRAW);
	// 从顶点着色器中初始化顶点的位置
	GLuint location = glGetAttribLocation(program, "vPosition");
	// 启用顶点属性数组
	glEnableVertexAttribArray(location);
	// 关联到顶点属性数组 (index, size, type, normalized, stride, *pointer)
	glVertexAttribPointer(
		location,
		2,
		GL_FLOAT,
		GL_FALSE,
		sizeof(glm::vec2),
		BUFFER_OFFSET(0));

	// 给颜色
	glGenBuffers(1, &vbo[1]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_colors), triangle_colors, GL_STATIC_DRAW);
	GLuint cLocation = glGetAttribLocation(program, "vColor");
	glEnableVertexAttribArray(cLocation);
	glVertexAttribPointer(
		cLocation,
		3,
		GL_FLOAT,
		GL_FALSE,
		sizeof(glm::vec3),
		BUFFER_OFFSET(0));

	/*
	* 初始化正方形的数据
	*/
	// 创建顶点数组对象
	glGenVertexArrays(1, &vao[1]);      // 分配1个顶点数组对象
	glBindVertexArray(vao[1]);          // 绑定顶点数组对象
 // 创建顶点缓存对象，我们可以重复使用我们声明的vbo变量
// 正方形顶点坐标
	glGenBuffers(1, &vbo[0]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(square_vertices), square_vertices, GL_STATIC_DRAW);
	// 从顶点着色器中初始化顶点的位置
	location = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(location);
	glVertexAttribPointer(
		location,
		2,
		GL_FLOAT,
		GL_FALSE,
		sizeof(glm::vec2),
		BUFFER_OFFSET(0));
	// 正方形颜色
	glGenBuffers(1, &vbo[1]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(square_colors), square_colors, GL_STATIC_DRAW);
	cLocation = glGetAttribLocation(program, "vColor");
	glEnableVertexAttribArray(cLocation);
	glVertexAttribPointer(
		cLocation,
		3,
		GL_FLOAT,
		GL_FALSE,
		sizeof(glm::vec3),
		BUFFER_OFFSET(0));


	// 黑色背景
	glClearColor(0.0, 0.0, 0.0, 1.0);
}

void display(void)
{
	// 清理窗口
	glClear(GL_COLOR_BUFFER_BIT);

	// 激活着色器
	glUseProgram(program);
	// 绑定三角形的顶点数组对象
	glBindVertexArray(vao[0]);
	glDrawArrays(GL_TRIANGLES, 0, TRIANGLE_NUM_POINTS);
	// 绑定正方形的顶点数组对象
	glBindVertexArray(vao[1]);

	// 注意这里使用的绘制模式为GL_TRIANGLE_FAN
	// 它会以顶点数据的一个点为中心顶点，绘制三角形

	// 绘制多个正方形
	for (int i = 0; i < SQUARE_NUM; ++i) {
		glDrawArrays(GL_TRIANGLE_FAN, (i * 4), 4);
	}
	// 强制所有进行中的OpenGL命令完成
	glFlush();
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

int main(int argc, char** argv)
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
	GLFWwindow* window = glfwCreateWindow(512, 512, "Red Triangle", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// 调用任何OpenGL的函数之前初始化GLAD
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	init();
	// 指定当前窗口进行重绘时要调用的函数
	while (!glfwWindowShouldClose(window))
	{
		display();

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
