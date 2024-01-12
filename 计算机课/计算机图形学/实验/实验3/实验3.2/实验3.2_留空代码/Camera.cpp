#include "Camera.h"

Camera::Camera() { updateCamera(); };
Camera::~Camera() {};

// 获取观察矩阵
glm::mat4 Camera::getViewMatrix()
{
	return this->lookAt(eye, at, up);
}

// 获取投影矩阵
glm::mat4 Camera::getProjectionMatrix(bool isOrtho)
{
	// 使用正交
	if (isOrtho) {
		return this->ortho(-scale, scale, -scale, scale, this->zNear, this->zFar);
	}
	// 使用透视
	else {
		return this->perspective(fov, aspect, this->zNear, this->zFar);
	}
}


glm::mat4 Camera::lookAt(const glm::vec4& eye, const glm::vec4& at, const glm::vec4& up)
{
	// @TODO: Task1:请按照实验课内容补全相机观察矩阵的计算
	glm::mat4 c = glm::mat4(1.0f);
	return c;
}

glm::mat4 Camera::ortho(const GLfloat left, const GLfloat right,
	const GLfloat bottom, const GLfloat top,
	const GLfloat zNear, const GLfloat zFar)
{	
	// @TODO: Task1:请按照实验课内容补全
	glm::mat4 c = glm::mat4(1.0f);
	return c;
}

glm::mat4 Camera::perspective(const GLfloat fovy, const GLfloat aspect,
	const GLfloat zNear, const GLfloat zFar)
{
	// @TODO: Task1:请按照实验课内容补全
	glm::mat4 c = glm::mat4(1.0f);
	return c;
}

glm::mat4 Camera::frustum(const GLfloat left, const GLfloat right,
	const GLfloat bottom, const GLfloat top,
	const GLfloat zNear, const GLfloat zFar)
{
	// 任意视锥体矩阵
	glm::mat4 c = glm::mat4(1.0f);
	c[0][0] = 2.0 * zNear / (right - left);
	c[0][2] = (right + left) / (right - left);
	c[1][1] = 2.0 * zNear / (top - bottom);
	c[1][2] = (top + bottom) / (top - bottom);
	c[2][2] = -(zFar + zNear) / (zFar - zNear);
	c[2][3] = -2.0 * zFar * zNear / (zFar - zNear);
	c[3][2] = -1.0;
	c[3][3] = 0.0;
	return c;
}


void Camera::updateCamera()
{
	up = glm::vec4(0.0, 1.0, 0.0, 0.0);
	if (upAngle > 90){
		up.y = -1;
	}
	else if (upAngle < -90){
		up.y = -1;
	}
	float eyex = radius * cos(upAngle * M_PI / 180.0) * sin(rotateAngle * M_PI / 180.0);
	float eyey = radius * sin(upAngle * M_PI / 180.0);
	float eyez = radius * cos(upAngle * M_PI / 180.0) * cos(rotateAngle * M_PI / 180.0);

	eye = glm::vec4(eyex, eyey, eyez, 1.0);
	at = glm::vec4(0.0, 0.0, 0.0, 1.0);
	// up = vec4(0.0, 1.0, 0.0, 0.0);

}

void Camera::keyboard(int key, int action, int mode)
{
	// 键盘事件处理

	if(key == GLFW_KEY_U && action == GLFW_PRESS && mode == 0x0000) 
	{
		rotateAngle += 5.0;
	}
	else if(key == GLFW_KEY_U && action == GLFW_PRESS && mode == GLFW_MOD_SHIFT) 
	{
		rotateAngle -= 5.0;
	}
	else if(key == GLFW_KEY_I && action == GLFW_PRESS && mode == 0x0000) 
	{
		upAngle += 5.0;
		if (upAngle > 180)
			upAngle = 180;
	}
	else if(key == GLFW_KEY_I && action == GLFW_PRESS && mode == GLFW_MOD_SHIFT) 
	{
		upAngle -= 5.0;
		if (upAngle < -180)
			upAngle = -180;
	}
	else if(key == GLFW_KEY_O && action == GLFW_PRESS && mode == 0x0000) 
	{
		scale += 0.1;
	}
	else if(key == GLFW_KEY_O && action == GLFW_PRESS && mode == GLFW_MOD_SHIFT) 
	{
		scale -= 0.1;
	}
	else if(key == GLFW_KEY_SPACE && action == GLFW_PRESS && mode == 0x0000) 
	{
		radius = 4.0;
		rotateAngle = 0.0;
		upAngle = 30.0;
		fov = 45.0;
		aspect = 1.0;
		scale = 1.5;
	}
}
