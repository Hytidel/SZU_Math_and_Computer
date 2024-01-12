#include "Angel.h"
#include "TriMesh.h"
#include "Camera.h"
#include <vector>
#include <string>

int WIDTH = 600;
int HEIGHT = 600;

int mainWindow;

struct openGLObject
{
	// 顶点数组对象
	GLuint vao;
	// 顶点缓存对象
	GLuint vbo;

	// 着色器程序
	GLuint program;
	// 着色器文件
	std::string vshader;
	std::string fshader;
	// 着色器变量
	GLuint pLocation;
	GLuint cLocation;

	// 投影变换变量
	GLuint modelLocation;
	GLuint viewLocation;
	GLuint projectionLocation;
};


openGLObject cube_object;

TriMesh *cube = new TriMesh();

Camera* camera_1 = new Camera();
Camera* camera_2 = new Camera();
Camera* camera_3 = new Camera();
Camera *camera_4 = new Camera();


void bindObjectAndData(TriMesh* mesh, openGLObject& object, const std::string &vshader, const std::string &fshader) {

	// 创建顶点数组对象
    glGenVertexArrays(1, &object.vao);  	// 分配1个顶点数组对象
	glBindVertexArray(object.vao);  	// 绑定顶点数组对象

	// 创建并初始化顶点缓存对象
	glGenBuffers(1, &object.vbo);
	glBindBuffer(GL_ARRAY_BUFFER, object.vbo);
	glBufferData(GL_ARRAY_BUFFER, 
		mesh->getPoints().size() * sizeof(glm::vec3) + mesh->getColors().size() * sizeof(glm::vec3),
		NULL, 
		GL_STATIC_DRAW);

	glBufferSubData(GL_ARRAY_BUFFER, 0, mesh->getPoints().size() * sizeof(glm::vec3), &mesh->getPoints()[0]);
	glBufferSubData(GL_ARRAY_BUFFER, mesh->getPoints().size() * sizeof(glm::vec3), mesh->getColors().size() * sizeof(glm::vec3), &mesh->getColors()[0]);

	object.vshader = vshader;
	object.fshader = fshader;
	object.program = InitShader(object.vshader.c_str(), object.fshader.c_str());

	// 从顶点着色器中初始化顶点的位置
	object.pLocation = glGetAttribLocation(object.program, "vPosition");
	glEnableVertexAttribArray(object.pLocation);
	glVertexAttribPointer(object.pLocation, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	// 从顶点着色器中初始化顶点的颜色
	object.cLocation = glGetAttribLocation(object.program, "vColor");
	glEnableVertexAttribArray(object.cLocation);
	glVertexAttribPointer(object.cLocation, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(mesh->getPoints().size() * sizeof(glm::vec3)));

	// 获得矩阵位置
	object.modelLocation = glGetUniformLocation(object.program, "model");
	object.viewLocation = glGetUniformLocation(object.program, "view");
	object.projectionLocation = glGetUniformLocation(object.program, "projection");
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

void init()
{
	std::string vshader, fshader;
	// 读取着色器并使用
	vshader = "shaders/vshader.glsl";
	fshader = "shaders/fshader.glsl";

	// 读取立方体
	cube->readOff("./assets/cube.off");
	bindObjectAndData(cube, cube_object, vshader, fshader);

	// 黑色背景
	glClearColor(0.0, 0.0, 0.0, 1.0);
}


void display_1()
{
	glViewport(0, HEIGHT / 2, WIDTH / 2, HEIGHT / 2);

	camera_1->updateCamera();

	// 物体模型的变换矩阵
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::rotate(model, glm::radians(camera_1->upAngle), glm::vec3(-1.0, 0.0, 0.0));
	model = glm::rotate(model, glm::radians(camera_1->rotateAngle), glm::vec3(0.0, 1.0, 0.0));
	model = glm::scale(model, glm::vec3(1.0, 1.0, 1.0) * glm::vec1(1.0 / ( camera_1->scale )));

	glm::mat4 view(1.0);
	view[2][2] = -1.0;
	glm::mat4 projection(1.0);

	// 传递投影变换矩阵
	glUniformMatrix4fv(cube_object.modelLocation, 1, GL_TRUE, &model[0][0]);
	glUniformMatrix4fv(cube_object.viewLocation, 1, GL_TRUE, &view[0][0]);
	glUniformMatrix4fv(cube_object.projectionLocation, 1, GL_TRUE, &projection[0][0]);

	glUseProgram(cube_object.program);
	glDrawArrays(GL_TRIANGLES, 0, cube->getPoints().size());
}

void display_2()
{
	glViewport(0, 0, WIDTH / 2, HEIGHT / 2);

	camera_2->updateCamera();

	glm::mat4 modelMatrix= glm::mat4(1.0); // 物体模型的变换矩阵	
	
	// 调用 Camera::lookAt 函数计算视图变换矩阵
	camera_2->viewMatrix = camera_2->lookAt(camera_2->eye, camera_2->at, camera_2->up);	
	
	// 调用 Camera:frustum 函数计算透视投影矩阵
	float top = tan(camera_2->fov * M_PI / 180 / 2) * camera_2->zNear;
	float right = top * camera_2->aspect;
	camera_2->projMatrix = camera_2->frustum(-right, right, -top, top, camera_2->zNear, camera_2->zFar);
	
	// 传递投影变换矩阵
	glUniformMatrix4fv(cube_object.modelLocation, 1, GL_TRUE, &modelMatrix[0][0]);
	glUniformMatrix4fv(cube_object.viewLocation, 1, GL_TRUE, &camera_2->viewMatrix[0][0]);
	glUniformMatrix4fv(cube_object.projectionLocation, 1, GL_TRUE, &camera_2->projMatrix[0][0]);

	glUseProgram(cube_object.program);
	glDrawArrays(GL_TRIANGLES, 0, cube->getPoints().size());
}

void display_3()
{
	glViewport(WIDTH / 2, HEIGHT / 2, WIDTH / 2, HEIGHT / 2);

	camera_3->updateCamera();

	glm::mat4 modelMatrix = glm::mat4(1.0); // 物体模型的变换矩阵	
	camera_3->viewMatrix = camera_3->lookAt(camera_3->eye, camera_3->at, camera_3->up);	// 调用 Camera::lookAt 函数计算视图变换矩阵
	glm::mat4 viewma = camera_3->viewMatrix;
	
	// @TODO: Task2: 调用 Camera::ortho 函数计算正交投影矩阵
	camera_3->projMatrix = glm::mat4(1.0f);

	
	// 传递投影变换矩阵
	glUniformMatrix4fv(cube_object.modelLocation, 1, GL_TRUE, &modelMatrix[0][0]);
	glUniformMatrix4fv(cube_object.viewLocation, 1, GL_TRUE, &camera_3->viewMatrix[0][0]);
	glUniformMatrix4fv(cube_object.projectionLocation, 1, GL_TRUE, &camera_3->projMatrix[0][0]);

	glUseProgram(cube_object.program);
	glDrawArrays(GL_TRIANGLES, 0, cube->getPoints().size());
}

void display_4()
{
	glViewport(WIDTH / 2, 0, WIDTH / 2, HEIGHT / 2);

	camera_4->updateCamera();

	glm::mat4 modelMatrix = glm::mat4(1.0); // 物体模型的变换矩阵	
	camera_4->viewMatrix = camera_4->lookAt(camera_4->eye, camera_4->at, camera_4->up);	// 调用 Camera::lookAt 函数计算视图变换矩阵

	// @TODO: Task3: 调用 Camera::perspective 函数计算透视投影矩阵
	camera_4->projMatrix = glm::mat4(1.0f);

	// 传递投影变换矩阵
	glUniformMatrix4fv(cube_object.modelLocation, 1, GL_TRUE, &modelMatrix[0][0]);
	glUniformMatrix4fv(cube_object.viewLocation, 1, GL_TRUE, &camera_4->viewMatrix[0][0]);
	glUniformMatrix4fv(cube_object.projectionLocation, 1, GL_TRUE, &camera_4->projMatrix[0][0]);

	glUseProgram(cube_object.program);
	glDrawArrays(GL_TRIANGLES, 0, cube->getPoints().size());
}


void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// 调用4个display分别在4个子视图内绘制立方体
	// 每个display使用的相机的的投影类型与计算方法都不一样
	display_1();
	display_2();
	display_3();
	display_4();
}

void reshape(GLsizei w, GLsizei h)
{
	WIDTH = w;
	HEIGHT = h;
	glViewport(0, 0, WIDTH, HEIGHT);
}

void printHelp()
{
	std::cout << "Keyboard Usage" << std::endl;
	std::cout <<
		"Q/ESC:		Exit" << std::endl <<
		"h:			Print help message" << std::endl <<
		"SPACE:		Reset all parameters" << std::endl <<
		"x/X:		Increase/Decrease the rotate angle" << std::endl <<
		"y/Y:		Increase/Decrease the up angle" << std::endl <<
		"r/R:		Increase/Decrease the distance between camera and object" << std::endl <<
		"f/F:		Increase/Decrease FOV of perspective projection" << std::endl <<
		"a/A:		Increase/Decrease WIDTH/HEIGHT aspect of perspective projection" << std::endl <<
		"s/S:		Increase/Decrease the extent of orthogonal projection" << std::endl;
}



void mainWindow_key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	// 'ESC键退出'
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
	else if(key == GLFW_KEY_H && action == GLFW_PRESS) 
	{
		printHelp();
	}
	else
	{
		camera_1->keyboard(key,action,mode);
		camera_2->keyboard(key,action,mode);
		camera_3->keyboard(key,action,mode);
		camera_4->keyboard(key,action,mode);
	}
}


// void idle(void)
// {
// 	glutPostRedisplay();
// }


void cleanData() {
	cube->cleanData();


	delete camera_1;
	delete camera_2;
	delete camera_3;
	delete camera_4;

	camera_1 = NULL;
	camera_2 = NULL;
	camera_3 = NULL;
	camera_4 = NULL;


	// 释放内存
	delete cube;
	cube = NULL;

	// 删除绑定的对象
	glDeleteVertexArrays(1, &cube_object.vao);
	glDeleteBuffers(1, &cube_object.vbo);
	glDeleteProgram(cube_object.program);
}

int main(int argc, char **argv)
{	
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    
	GLFWwindow* mainwindow = glfwCreateWindow(WIDTH, HEIGHT, "3D Transfomations(1)", NULL, NULL);
	if (mainwindow == NULL)
	{
		std::cout << "Failed to create GLFW window!" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(mainwindow);
	glfwSetFramebufferSizeCallback(mainwindow, framebuffer_size_callback);
	glfwSetKeyCallback(mainwindow,mainWindow_key_callback);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
	glEnable(GL_DEPTH_TEST);
	init();
    printHelp();
	while (!glfwWindowShouldClose(mainwindow))
    {
        
        display();
        glfwSwapBuffers(mainwindow);
        glfwPollEvents();
		
    }
    glfwTerminate();
    return 0;
}

