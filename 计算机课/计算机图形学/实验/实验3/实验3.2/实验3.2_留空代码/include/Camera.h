#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "Angel.h"

class Camera
{
public:
	Camera();
	~Camera();

	glm::mat4 getViewMatrix();
	glm::mat4 getProjectionMatrix(bool isOrtho);

	glm::mat4 lookAt(const glm::vec4& eye, const glm::vec4& at, const glm::vec4& up);

	glm::mat4 ortho(const GLfloat left, const GLfloat right,
		const GLfloat bottom, const GLfloat top,
		const GLfloat zNear, const GLfloat zFar);

	glm::mat4 perspective(const GLfloat fov, const GLfloat aspect,
		const GLfloat zNear, const GLfloat zFar);

	glm::mat4 frustum(const GLfloat left, const GLfloat right,
		const GLfloat bottom, const GLfloat top,
		const GLfloat zNear, const GLfloat zFar);

	// 每次更改相机参数后更新一下相关的数值
	void updateCamera();
	// 处理相机的键盘操作
	void keyboard(int key, int x, int y);

	// 模视矩阵
	glm::mat4 viewMatrix;
	glm::mat4 projMatrix;

	// 相机位置参数
	float radius = 4.0;
	float rotateAngle = 0.0;
	float upAngle = 30.0;
	glm::vec4 eye;
	glm::vec4 at;
	glm::vec4 up;

	// 投影参数
	float zNear = 0.1;
	float zFar = 100.0;
	// 透视投影参数
	float fov = 45.0;
	float aspect = 1.0;
	// 正交投影参数
	float scale = 1.5;

};
#endif