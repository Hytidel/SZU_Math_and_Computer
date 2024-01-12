#include "Angel.h"
#include "TriMesh.h"
#include "Camera.h"
#include "MeshPainter.h"

#include <vector>
#include <string>

int WIDTH = 600;
int HEIGHT = 600;

int mainWindow;

Camera* camera = new Camera();
Light* light = new Light();
MeshPainter *painter = new MeshPainter();

// 这个用来回收和删除我们创建的物体对象
std::vector<TriMesh *> meshList;

void init()
{
	std::string vshader, fshader;
	// 读取着色器并使用
#ifdef __APPLE__	// for MacOS
	vshader = "shaders/vshader_mac.glsl";
	fshader = "shaders/fshader_mac.glsl";
#else				// for Windows
	vshader = "shaders/vshader_win.glsl";
	fshader = "shaders/fshader_win.glsl";
#endif

	// 设置光源位置
	light->setTranslation(glm::vec3(0.0, 0.0, 2.0));
	light->setAmbient(glm::vec4(1.0, 1.0, 1.0, 1.0)); // 环境光
	light->setDiffuse(glm::vec4(1.0, 1.0, 1.0, 1.0)); // 漫反射
	light->setSpecular(glm::vec4(1.0, 1.0, 1.0, 1.0)); // 镜面反射
	light->setAttenuation(1.0, 0.045, 0.0075); // 衰减系数


 	TriMesh* table = new TriMesh();
	// @TODO: Task2 读取桌子模型
	// table->setNormalize(true);
	// table->readObj("./assets/table.obj");

	// 设置物体的旋转位移

	// 加到painter中

 
 	TriMesh* wawa = new TriMesh();
	// @TODO: Task2 读取娃娃模型

	// 设置物体的旋转位移

	// 加到painter中


	glClearColor(1.0, 1.0, 1.0, 1.0);
	// glClearColor(0.0, 0.0, 0.0, 1.0);
}



void display()
{
// #ifdef __APPLE__ // 解决 macOS 10.15 显示画面缩小问题
// 	glViewport(0, 0, WIDTH * 2, HEIGHT * 2);
// #endif
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	painter->drawMeshes(light, camera);

	//glutSwapBuffers();
}


void printHelp()
{
	std::cout << "================================================" << std::endl;
	std::cout << "Use mouse to controll the light position (drag)." << std::endl;
	std::cout << "================================================" << std::endl << std::endl;

	std::cout << "Keyboard Usage" << std::endl;
	std::cout <<
		"[Window]" << std::endl <<
		"ESC:		Exit" << std::endl <<
		"h:		Print help message" << std::endl <<

		std::endl <<
		"[Camera]" << std::endl <<
		"SPACE:		Reset camera parameters" << std::endl <<
		"u/U:		Increase/Decrease the rotate angle" << std::endl <<
		"i/I:		Increase/Decrease the up angle" << std::endl <<
		"o/O:		Increase/Decrease the camera radius" << std::endl << std::endl;

}

// 键盘响应函数
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	float tmp;
	glm::vec4 ambient;
	if (action == GLFW_PRESS) {
		switch (key)
		{
		case GLFW_KEY_ESCAPE: exit(EXIT_SUCCESS); break;
		case GLFW_KEY_H: printHelp(); break;
		default:
			camera->keyboard(key, action, mode);
			break;
		}
	}
}

void cleanData() {
	// 释放内存
	
	delete camera;
	camera = NULL;

	delete light;
	light = NULL;

	painter->cleanMeshes();

	delete painter;
	painter = NULL;
	
	for (int i=0; i<meshList.size(); i++) {
		delete meshList[i];
	}
	meshList.clear();
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
	GLFWwindow* window = glfwCreateWindow(600, 600, "Simple-texture", NULL, NULL);
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

	// Init mesh, shaders, buffer
	init();
	// 输出帮助信息
	printHelp();
	// 启用深度测试
	glEnable(GL_DEPTH_TEST);
	while (!glfwWindowShouldClose(window))
	{
		display();
		//reshape();

		// 交换颜色缓冲 以及 检查有没有触发什么事件（比如键盘输入、鼠标移动等）
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	cleanData();


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