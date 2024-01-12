#include "Angel.h"
#include <string>


const int MENU_CHOICE_WHITE = 0;
const int MENU_CHOICE_BLACK = 1;
const int MENU_CHOICE_RED = 2;
const int MENU_CHOICE_GREEN = 3;
const int MENU_CHOICE_BLUE = 4;
const int MENU_CHOICE_YELLOW = 5;
const int MENU_CHOICE_ORANGE = 6;
const int MENU_CHOICE_PURPLE = 7;

const int MENU_START_ANIMATION = 8;
const int MENU_STOP_ANIMATION = 9;

const glm::vec3 WHITE(1.0, 1.0, 1.0);
const glm::vec3 BLACK(0.0, 0.0, 0.0);
const glm::vec3 RED(1.0, 0.0, 0.0);
const glm::vec3 GREEN(0.0, 1.0, 0.0);
const glm::vec3 BLUE(0.0, 0.0, 1.0);
const glm::vec3 YELLOW(1.0, 1.0, 0.0);
const glm::vec3 ORANGE(1.0, 0.65, 0.0);
const glm::vec3 PURPLE(0.8, 0.0, 0.8);

// 主窗口变量
const int SQUARE_NUM = 6;
const int SQUARE_NUM_POINTS = 4 * SQUARE_NUM;
int mainWindow;
int mainWindowMenu;
int mainWindowSubmenu;
int width = 600;		// 主窗口宽度
int height = 600;		// 主窗口高度
double offsetAngle = 0;	// 角度偏移量
double delta = 0.01;	// 每次改变角度偏移的变化量
glm::vec3 mainWindowSquareColor = WHITE;

GLuint square_vao, ellipse_vao, program;

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// 获得正方形的每个角度
double getSquareAngle(int point)
{
    return (M_PI / 4 + (M_PI / 2 * point)) + offsetAngle;
}

// 生成正方形上顶点的属性
void generateSquarePoints(glm::vec2 vertices[], glm::vec3 colors[], int squareNum, int startVertexIndex) {
	double scale = 0.90;
	double scaleAdjust = scale / squareNum;
	glm::vec2 center(0.0, 0.0);

	int vertexIndex = startVertexIndex;

	for (int i = 0; i < squareNum; i++) {
		glm::vec3 currentColor = 0 == i % 2 ? mainWindowSquareColor : BLACK;

		for (int j = 0; j < 4; j++) {
			double currentAngle = getSquareAngle(j);
			vertices[vertexIndex] = glm::vec2(cos(currentAngle), sin(currentAngle)) * glm::vec2(scale, scale) + center;
			colors[vertexIndex] = currentColor;
			vertexIndex++;
		}

		scale -= scaleAdjust;
	}
}

void mainWindowInit()
{
	glm::vec2 vertices[SQUARE_NUM * 4];
	glm::vec3 colors[SQUARE_NUM * 4];

	// 创建主窗口中多个正方形
	generateSquarePoints(vertices, colors, SQUARE_NUM, 0);

	// 创建顶点数组对象
    glGenVertexArrays(1, &square_vao);  	// 分配1个顶点数组对象
    glBindVertexArray(square_vao);  	// 绑定顶点数组对象

	// 创建并初始化顶点缓存对象。
	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) + sizeof(colors), NULL, GL_STATIC_DRAW);

	// 分别读取数据
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices), sizeof(colors), colors);

	// 读取着色器并使用
	std::string vshader, fshader;
	
    vshader = "shaders/vshader.glsl";
    fshader = "shaders/fshader.glsl";
	
	// 创建着色器程序对象并使用。
	program = InitShader(vshader.c_str(), fshader.c_str());
	glUseProgram(program);

	// 从顶点着色器中初始化顶点的位置
	GLuint pLocation = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(pLocation);
	glVertexAttribPointer(pLocation, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	// 从顶点着色器中初始化顶点的颜色
	GLuint cLocation = glGetAttribLocation(program, "vColor");
	glEnableVertexAttribArray(cLocation);
	glVertexAttribPointer(cLocation, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(vertices)));
	
	// 黑色背景
	glClearColor(0.0, 0.0, 0.0, 1.0);
}

void mainWindowDisplay()
{
	mainWindowInit();    // 重绘时写入新的颜色数据
	glClear(GL_COLOR_BUFFER_BIT);
	
	glUseProgram(program);

	// 创建顶点数组对象
    glBindVertexArray(square_vao);  	// 绑定顶点数组对象
	

	for (int i = 0; i < SQUARE_NUM; i++) {
		glDrawArrays(GL_TRIANGLE_FAN, (i * 4), 4);
	}

}

// 主窗口键盘回调函数。
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
	else if (key == GLFW_KEY_R && action == GLFW_PRESS)
	{
		mainWindowSquareColor = RED;
	}
	else if (key == GLFW_KEY_B && action == GLFW_PRESS)
	{
		mainWindowSquareColor = BLUE;
	}
	else if (key == GLFW_KEY_W && action == GLFW_PRESS)
	{
		mainWindowSquareColor = WHITE;
	}
}


// 主窗口鼠标点击回调函数。
void mouse_button_callback(GLFWwindow* window, int button, int action, int mode)
{
	// 按下鼠标左键，图形变为绿色。
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		mainWindowSquareColor = GREEN;
	}
	// 按下鼠标右键，图形变为黄色。
	else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
	{	
		mainWindowSquareColor = YELLOW;
	}
}

void printHelp() {
    printf("%s\n\n", "Interaction");
    printf("Keys to update the background color:\n");
    printf("'r' - red\n'b' - blue\n'w' - white\n");
    printf("Mouse click to update color:\n");
    printf("'left' - green\n'right' - yellow\n");
    printf("Mouse sroll to rotate:\n");
    printf("'up' - clockwise\n");
    printf("'down' - anticlockwise\n");
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

	// 创建主窗口。
	GLFWwindow* mainwindow = glfwCreateWindow(width, height, "mainWindow", NULL, NULL);
	if (mainwindow == NULL)
	{
		std::cout << "Failed to create GLFW window!" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(mainwindow);
	glfwSetFramebufferSizeCallback(mainwindow, framebuffer_size_callback);
	glfwSetKeyCallback(mainwindow, key_callback);
	glfwSetMouseButtonCallback(mainwindow, mouse_button_callback);
	// @TODO: 创建鼠标滚轮回调函数函数并绑定。
	
	
	
	

	// 配置glad。
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
	mainWindowInit();
    printHelp();
	// 循环渲染。
	while (!glfwWindowShouldClose(mainwindow))
    {
        
        mainWindowDisplay();
        glfwSwapBuffers(mainwindow);
        glfwPollEvents();
		
    }
    glfwTerminate();
    return 0;
}
