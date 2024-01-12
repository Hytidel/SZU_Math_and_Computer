#include "Angel.h"
#include "TriMesh.h"
#include "Camera.h"

#include <vector>
#include <string>
#include <algorithm>

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
	GLuint nLocation;

	// 投影变换变量
	GLuint modelLocation;
	GLuint viewLocation;
	GLuint projectionLocation;

	// 阴影变量
	GLuint shadowLocation;
};


openGLObject mesh_object;

Light* light = new Light();

TriMesh* mesh = new TriMesh();

Camera* camera = new Camera();


void bindObjectAndData(TriMesh* mesh, openGLObject& object, const std::string &vshader, const std::string &fshader) {

	// 创建顶点数组对象
	glGenVertexArrays(1, &object.vao);  	// 分配1个顶点数组对象
	glBindVertexArray(object.vao);  	// 绑定顶点数组对象


	// 创建并初始化顶点缓存对象
	glGenBuffers(1, &object.vbo);
	glBindBuffer(GL_ARRAY_BUFFER, object.vbo);
	glBufferData(GL_ARRAY_BUFFER, 
		( mesh->getPoints().size() + mesh->getColors().size() + mesh->getNormals().size() ) * sizeof(glm::vec3),
		NULL, 
		GL_STATIC_DRAW);

	glBufferSubData(GL_ARRAY_BUFFER, 0, mesh->getPoints().size() * sizeof(glm::vec3), &mesh->getPoints()[0]);
	glBufferSubData(GL_ARRAY_BUFFER, mesh->getPoints().size() * sizeof(glm::vec3), mesh->getColors().size() * sizeof(glm::vec3), &mesh->getColors()[0]);
	// @TODO: Task1 修改完TriMesh.cpp的代码成后再打开下面注释，否则程序会报错
	// glBufferSubData(GL_ARRAY_BUFFER, (mesh->getPoints().size() + mesh->getColors().size()) * sizeof(glm::vec3), mesh->getNormals().size() * sizeof(glm::vec3), &mesh->getNormals()[0]);

	object.vshader = vshader;
	object.fshader = fshader;
	object.program = InitShader(object.vshader.c_str(), object.fshader.c_str());

	// 从顶点着色器中初始化顶点的坐标
	object.pLocation = glGetAttribLocation(object.program, "vPosition");
	glEnableVertexAttribArray(object.pLocation);
	glVertexAttribPointer(object.pLocation, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	// 从顶点着色器中初始化顶点的颜色
	object.cLocation = glGetAttribLocation(object.program, "vColor");
	glEnableVertexAttribArray(object.cLocation);
	glVertexAttribPointer(object.cLocation, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(mesh->getPoints().size() * sizeof(glm::vec3)));

	// @TODO: Task1 从顶点着色器中初始化顶点的法向量



	// 获得矩阵位置
	object.modelLocation = glGetUniformLocation(object.program, "model");
	object.viewLocation = glGetUniformLocation(object.program, "view");
	object.projectionLocation = glGetUniformLocation(object.program, "projection");

	object.shadowLocation = glGetUniformLocation(object.program, "isShadow");
}


void bindLightAndMaterial(TriMesh* mesh, openGLObject& object, Light* light, Camera* camera) {

	// 传递相机的位置
	glUniform3fv( glGetUniformLocation(object.program, "eye_position"), 1, &camera->eye[0] );

	// 传递物体的材质
	glm::vec4 meshAmbient = mesh->getAmbient();
	glm::vec4 meshDiffuse = mesh->getDiffuse();
	glm::vec4 meshSpecular = mesh->getSpecular();
	float meshShininess = mesh->getShininess();
	glUniform4fv(glGetUniformLocation(object.program, "material.ambient"), 1, &meshAmbient[0]);
	glUniform4fv(glGetUniformLocation(object.program, "material.diffuse"), 1, &meshDiffuse[0]);
	glUniform4fv(glGetUniformLocation(object.program, "material.specular"), 1, &meshSpecular[0]);
	glUniform1f(glGetUniformLocation(object.program, "material.shininess"), meshShininess);

	// 传递光源信息
	glm::vec4 lightAmbient = light->getAmbient();
	glm::vec4 lightDiffuse = light->getDiffuse();
	glm::vec4 lightSpecular = light->getSpecular();
	glm::vec3 lightPosition = light->getTranslation();

	glUniform4fv(glGetUniformLocation(object.program, "light.ambient"), 1, &lightAmbient[0]);
	glUniform4fv(glGetUniformLocation(object.program, "light.diffuse"), 1, &lightDiffuse[0]);
	glUniform4fv(glGetUniformLocation(object.program, "light.specular"), 1, &lightSpecular[0]);
	glUniform3fv(glGetUniformLocation(object.program, "light.position"), 1, &lightPosition[0]);

}


void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		double x, y;
		glfwGetCursorPos(window, &x, &y);
		// if (!is_click)
		// {
			float half_winx = WIDTH / 2.0;
			float half_winy = HEIGHT / 2.0;
			float lx = float(x - half_winx) / half_winx;
			float ly = float(HEIGHT - y - half_winy) / half_winy;

			glm::vec3 pos = light->getTranslation();
			
			pos.x = lx;
			pos.y = ly;

			light->setTranslation(pos);

		// }
	}
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

	// 设置光源位置
	light->setTranslation(glm::vec3(0.0, 0.0, 2.0));
	light->setAmbient(glm::vec4(1.0, 1.0, 1.0, 1.0)); // 环境光
	light->setDiffuse(glm::vec4(1.0, 1.0, 1.0, 1.0)); // 漫反射
	light->setSpecular(glm::vec4(1.0, 1.0, 1.0, 1.0)); // 镜面反射

	// 设置物体的旋转位移
	mesh->setTranslation(glm::vec3(0.0, 0.0, 0.0));
	mesh->setRotation(glm::vec3(0.0, 0.0, 0.0));
	mesh->setScale(glm::vec3(1.0, 1.0, 1.0));

	// 设置材质
	mesh->setAmbient(glm::vec4(0.2, 0.2, 0.2, 1.0)); // 环境光
	mesh->setDiffuse(glm::vec4(0.7, 0.7, 0.7, 1.0)); // 漫反射
	mesh->setSpecular(glm::vec4(0.2, 0.2, 0.2, 1.0)); // 镜面反射
	mesh->setShininess(1.0); //高光系数
	
	// 将物体的顶点数据传递
	bindObjectAndData(mesh, mesh_object, vshader, fshader);

	glClearColor(0.0, 0.0, 0.0, 0.0);
}



void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// 相机矩阵计算
	camera->updateCamera();
	camera->viewMatrix = camera->getViewMatrix();
	camera->projMatrix = camera->getProjectionMatrix(false);


	glBindVertexArray(mesh_object.vao);
    
	glUseProgram(mesh_object.program);

	// 物体的变换矩阵
	glm::mat4 modelMatrix = mesh->getModelMatrix();

	// 传递矩阵
	glUniformMatrix4fv(mesh_object.modelLocation, 1, GL_TRUE, &modelMatrix[0][0]);
	glUniformMatrix4fv(mesh_object.viewLocation, 1, GL_TRUE, &camera->viewMatrix[0][0]);
	glUniformMatrix4fv(mesh_object.projectionLocation, 1, GL_TRUE, &camera->projMatrix[0][0]);
	// 将着色器 isShadow 设置为0，表示正常绘制的颜色，如果是1着表示阴影
	glUniform1i(mesh_object.shadowLocation, 0);

	// 将材质和光源数据传递给着色器
	bindLightAndMaterial(mesh, mesh_object, light, camera);
	// 绘制
	glDrawArrays(GL_TRIANGLES, 0, mesh->getPoints().size());

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
		"[Model]" << std::endl <<
		"-:		Reset material parameters" << std::endl <<
		"1/2/3/!/@/#:	Change ambient parameters" << std::endl <<
		"4/5/6/$/%/^:	(TODO) Change diffuse parameters" << std::endl <<
		"7/8/9/&/*/(:	(TODO) Change specular parameters" << std::endl <<
		"0/):		(TODO) Change shininess parameters" << std::endl <<
		std::endl <<
		"q:		Load sphere model" << std::endl << 
		"a:		Load Pikachu model" << std::endl << 
		"w:		Load Squirtle model" << std::endl << 
		"s:		Load sphere_coarse model" << std::endl << 
		std::endl <<
		"[Camera]" << std::endl <<
		"SPACE:		Reset camera parameters" << std::endl <<
		"u/U:		Increase/Decrease the rotate angle" << std::endl <<
		"i/I:		Increase/Decrease the up angle" << std::endl <<
		"o/O:		Increase/Decrease the camera radius" << std::endl << std::endl;

}

void mainWindow_key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	float tmp;
	glm::vec4 ambient;
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
	else if(key == GLFW_KEY_H && action == GLFW_PRESS) 
	{
		printHelp();
	}
	else if(key == GLFW_KEY_Q && action == GLFW_PRESS) 
	{
		std::cout << "read sphere.off" << std::endl;
		mesh = new TriMesh();
		mesh->readOff("./assets/sphere.off");
		init();
	}
	else if(key == GLFW_KEY_A && action == GLFW_PRESS) 
	{
		std::cout << "read Pikachu.off" << std::endl;
		mesh = new TriMesh();
		mesh->readOff("./assets/Pikachu.off");
		init();
	}
	else if(key == GLFW_KEY_W && action == GLFW_PRESS) 
	{
		std::cout << "read Squirtle.off" << std::endl;
		mesh = new TriMesh();
		mesh->readOff("./assets/Squirtle.off");
		init();
	}
	else if(key == GLFW_KEY_S && action == GLFW_PRESS) 
	{
		std::cout << "read sphere_coarse.off" << std::endl;
		mesh = new TriMesh();
		mesh->readOff("./assets/sphere_coarse.off");
		init();
	}
	else if(key == GLFW_KEY_1 && action == GLFW_PRESS && mode == 0x0000) 
	{
		ambient = mesh->getAmbient();
		tmp = ambient.x;
		ambient.x = std::min(tmp + 0.1, 1.0); 
		mesh->setAmbient(ambient);
	}
	else if(key == GLFW_KEY_1 && action == GLFW_PRESS && mode == GLFW_MOD_SHIFT) 
	{
		ambient = mesh->getAmbient();
		tmp = ambient.x;
		ambient.x = std::min(tmp - 0.1, 1.0); 
		mesh->setAmbient(ambient);
	}
	else if(key == GLFW_KEY_2 && action == GLFW_PRESS && mode == 0x0000) 
	{
		ambient = mesh->getAmbient();
		tmp = ambient.y;
		ambient.y = std::min(tmp + 0.1, 1.0);
		mesh->setAmbient(ambient);
	}
	else if(key == GLFW_KEY_2 && action == GLFW_PRESS && mode == GLFW_MOD_SHIFT) 
	{
		ambient = mesh->getAmbient();
		tmp = ambient.y;
		ambient.y = std::min(tmp - 0.1, 1.0);
		mesh->setAmbient(ambient);
	}
	else if(key == GLFW_KEY_3 && action == GLFW_PRESS && mode == 0x0000) 
	{
		ambient = mesh->getAmbient();
		tmp = ambient.z;
		ambient.z = std::min(tmp + 0.1, 1.0);
		mesh->setAmbient(ambient);
	}
	else if(key == GLFW_KEY_3 && action == GLFW_PRESS && mode == GLFW_MOD_SHIFT) 
	{
		ambient = mesh->getAmbient();
		tmp = ambient.z;
		ambient.z = std::min(tmp - 0.1, 1.0);
		mesh->setAmbient(ambient);
	}
	else if(key == GLFW_KEY_4 && action == GLFW_PRESS && mode == 0x0000)
	{
		mesh->setAmbient(glm::vec4(0.2, 0.2, 0.2, 1.0));
		mesh->setDiffuse(glm::vec4(0.7, 0.7, 0.7, 1.0));
		mesh->setSpecular(glm::vec4(0.2, 0.2, 0.2, 1.0));
		mesh->setShininess(1.0);
	}
	else
	{
		camera->keyboard(key, action, mode);
	}

}

// 重新设置窗口
void reshape(GLsizei w, GLsizei h)
{
	glViewport(0, 0, w, h);
}

void cleanData() {
	mesh->cleanData();
	
	delete camera;
	camera = NULL;

	// 释放内存
	delete mesh;
	mesh = NULL;

	// 删除绑定的对象
	glDeleteVertexArrays(1, &mesh_object.vao);
	glDeleteBuffers(1, &mesh_object.vbo);
	glDeleteProgram(mesh_object.program);

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
	
	GLFWwindow* mainwindow = glfwCreateWindow(600, 600, "light and material", NULL, NULL);
	if (mainwindow == NULL)
	{
		std::cout << "Failed to create GLFW window!" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(mainwindow);
	glfwSetFramebufferSizeCallback(mainwindow, framebuffer_size_callback);
	glfwSetKeyCallback(mainwindow,mainWindow_key_callback);
	glfwSetMouseButtonCallback(mainwindow, mouse_button_callback);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
	glEnable(GL_DEPTH_TEST);
	mesh->readOff("./assets/sphere.off");
	mesh->generateCube();
	// Init mesh, shaders, buffer
	init();
    printHelp();
	// bind callbacks
	while (!glfwWindowShouldClose(mainwindow))
    {
        
        display();
        glfwSwapBuffers(mainwindow);
        glfwPollEvents();
		
    }
    glfwTerminate();
    return 0;
}
