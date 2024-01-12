#include "Angel.h"

#include <vector>
#include <fstream>
#include <string>

// #pragma comment(lib, "glew32.lib")

using namespace std;

int window;

// 三角面片中的顶点序列
typedef struct vIndex {
	unsigned int a, b, c;
	vIndex(int ia, int ib, int ic) : a(ia), b(ib), c(ic) {}
} vec3i;

std::vector<glm::vec3> vertices;
std::vector<vec3i> faces;

int nVertices = 0;
int nFaces = 0;
int nEdges = 0;

std::vector<glm::vec3> points;   // 传入着色器的绘制点
std::vector<glm::vec3> colors;   // 传入着色器的颜色

const int NUM_VERTICES = 8;

const glm::vec3 vertex_colors[NUM_VERTICES] = {
	glm::vec3(1.0, 1.0, 1.0),  // White
	glm::vec3(1.0, 1.0, 0.0),  // Yellow
	glm::vec3(0.0, 1.0, 0.0),  // Green
	glm::vec3(0.0, 1.0, 1.0),  // Cyan
	glm::vec3(1.0, 0.0, 1.0),  // Magenta
	glm::vec3(1.0, 0.0, 0.0),  // Red
	glm::vec3(0.0, 0.0, 0.0),  // Black
	glm::vec3(0.0, 0.0, 1.0)   // Blue
};

void read_off(const std::string filename)
{
	// fin打开文件读取文件信息
	if (filename.empty()) {
		return;
	}
	std::ifstream fin;
	fin.open(filename);
	// @TODO: Task1:修改此函数读取OFF文件中三维模型的信息
	if (!fin)
	{
		printf("文件有误\n");
		return;
	}
	else
	{
		printf("文件打开成功\n");
		vertices.clear();
		faces.clear();

		// 读取OFF字符串
		string str;
		fin >> str;
		// 读取文件中顶点数、面片数、边数
		fin >> nVertices >> nFaces >> nEdges;

		// 根据顶点数，循环读取每个顶点坐标，将其保存到vertices


		// 根据面片数，循环读取每个面片信息，并用构建的vec3i结构体保存到faces


	}
	fin.close();
}

void storeFacesPoints()
{
	points.clear();
	colors.clear();
	// @TODO: Task1:修改此函数在points和colors容器中存储每个三角面片的各个点和颜色信息
	// 在points容器中，依次添加每个面片的顶点，并在colors容器中，添加该点的颜色信息
	// 比如一个正方形由两个三角形构成，那么vertices会由4个顶点的数据构成，faces会记录两个三角形的顶点下标，
	// 而points就是记录这2个三角形的顶点，总共6个顶点的数据。
	// colors容器则是和points的顶点一一对应，保存这个顶点的颜色，这里我们可以使用顶点坐标或者自己设定的颜色赋值。


}

void init()
{

	// 读取off模型文件
	read_off("./assets/cube.off");
	storeFacesPoints();

	// 创建顶点数组对象
	GLuint vao[1];
    glGenVertexArrays(1, vao);  	// 分配1个顶点数组对象
	glBindVertexArray(vao[0]);  	// 绑定顶点数组对象

// 创建并初始化顶点缓存对象
	GLuint buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(glm::vec3) + colors.size() * sizeof(glm::vec3), NULL, GL_DYNAMIC_DRAW);

	// @TODO: Task1:修改完成后再打开下面注释，否则程序会报错
	// 分别读取数据
	// glBufferSubData(GL_ARRAY_BUFFER, 0, points.size() * sizeof(glm::vec3), &points[0]);
	// glBufferSubData(GL_ARRAY_BUFFER, points.size() * sizeof(glm::vec3), colors.size() * sizeof(glm::vec3), &colors[0]);

	// 读取着色器并使用
	std::string vshader, fshader;
	vshader = "shaders/vshader.glsl";
	fshader = "shaders/fshader.glsl";
	GLuint program = InitShader(vshader.c_str(), fshader.c_str());
	glUseProgram(program);

	// 从顶点着色器中初始化顶点的位置
	GLuint pLocation = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(pLocation);
	glVertexAttribPointer(pLocation, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	// 从顶点着色器中初始化顶点的颜色
	GLuint cLocation = glGetAttribLocation(program, "vColor");
	glEnableVertexAttribArray(cLocation);
	glVertexAttribPointer(cLocation, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(points.size() * sizeof(glm::vec3)));

	// 黑色背景
	glClearColor(0.0, 0.0, 0.0, 1.0);
}

void display(void)
{
	// @TODO: Task2:清理窗口，包括颜色缓存和深度缓存
	glClear(GL_COLOR_BUFFER_BIT);

	glDrawArrays(GL_TRIANGLES, 0, points.size());
}


// 窗口键盘回调函数。
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
	else if (key == GLFW_KEY_MINUS && action == GLFW_PRESS)
	{
		cout << "read cube.off" << endl;
		read_off("./assets/cube.off");
		storeFacesPoints();
		// @TODO: Task1:修改完成后再打开下面注释，否则程序会报错
		// glBufferSubData(GL_ARRAY_BUFFER, 0, points.size() * sizeof(glm::vec3), &points[0]);
		// glBufferSubData(GL_ARRAY_BUFFER, points.size() * sizeof(glm::vec3), colors.size() * sizeof(glm::vec3), &colors[0]);
		//glutPostWindowRedisplay(window);
	}
	else if (key == GLFW_KEY_EQUAL && action == GLFW_PRESS)
	{
		cout << "read cube2.off" << endl;
		read_off("./assets/cube2.off");
		storeFacesPoints();
		// @TODO: Task1:修改完成后再打开下面注释，否则程序会报错
		// glBufferSubData(GL_ARRAY_BUFFER, 0, points.size() * sizeof(glm::vec3), &points[0]);
		// glBufferSubData(GL_ARRAY_BUFFER, points.size() * sizeof(glm::vec3), colors.size() * sizeof(glm::vec3), &colors[0]);
	}
	else if (key == GLFW_KEY_0 && action == GLFW_PRESS)
	{
		cout << "reset" << endl;
		// 关闭深度测试
		glDisable(GL_DEPTH_TEST);
		// 关闭面剔除
		glDisable(GL_CULL_FACE);
		// 使用填充绘制模式
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	// @TODO: Task2:启用深度测试
	else if (key == GLFW_KEY_1 && action == GLFW_PRESS && mode == 0x0000) // 0x0000表示组合键为空 
	{
		cout << "depth test: enable" << endl;
		

	}
	// @TODO: Task2:关闭深度测试
	else if (key == GLFW_KEY_1 && action == GLFW_PRESS && mode == GLFW_MOD_SHIFT)
	{
		cout << "depth test: disable" << endl;
		
		
	}
	// @TODO: Task3:启用反面剔除
	else if (key == GLFW_KEY_2 && action == GLFW_PRESS && mode == 0x0000)
	{
		cout << "cull back: enable" << endl;
		

	}
	// @TODO: Task3:关闭反面剔除
	else if (key == GLFW_KEY_2 && action == GLFW_PRESS && mode == GLFW_MOD_SHIFT)
	{
		cout << "cull back: disable" << endl;
		

	}
	// @TODO: Task4:启用正面剔除
	else if (key == GLFW_KEY_3 && action == GLFW_PRESS && mode == 0x0000)
	{
		cout << "cull front: enable" << endl;
		

	}
	// @TODO: Task4:关闭正面剔除
	else if (key == GLFW_KEY_3 && action == GLFW_PRESS && mode == GLFW_MOD_SHIFT)
	{
		cout << "cull front: disable" << endl;
		
		
	}
	// @TODO: Task5:启用线绘制模式
	else if (key == GLFW_KEY_4 && action == GLFW_PRESS && mode == 0x0000)
	{
		cout << "line mode: enable" << endl;
		

	}
	// @TODO: Task5:关闭线绘制模式
	else if (key == GLFW_KEY_4 && action == GLFW_PRESS && mode == GLFW_MOD_SHIFT)
	{
		cout << "line mode: disable" << endl;
		
		
	}
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
	GLFWwindow* window = glfwCreateWindow(600, 600, "3D OFF Model", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, key_callback);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// 调用任何OpenGL的函数之前初始化GLAD
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	init();
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

