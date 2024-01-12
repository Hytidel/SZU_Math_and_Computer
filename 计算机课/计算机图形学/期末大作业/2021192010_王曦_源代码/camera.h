#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

// 摄像机移动的方向
enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT, 
    UP, 
    DOWN
};

// 控制摄像机视角的变量
const float YAW = -90.0f;  // 偏航角, 注意不是初始化为 0.0f
const float PITCH = 0.0f;  // 俯仰角
const float SPEED = 7.5f;  // 速度
const float SENSITIVITY = 0.05f;  // 鼠标灵敏度
const float ZOOM = 45.0f;  // 视野

class Camera {
public:
    // 摄像机属性
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;

    // Euler 角
    float Yaw;  // 偏航角
    float Pitch;  // 俯仰角

    // 摄像机参数
    float MovementSpeed;  // 移动速度
    float MouseSensitivity;  // 鼠标灵敏度
    float Zoom;  // 视野

    // 用向量初始化
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH) 
        : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM) {
        Position = position;
        WorldUp = up;
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
    }
    // 用视角初始化
    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) 
        : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM) {
        Position = glm::vec3(posX, posY, posZ);
        WorldUp = glm::vec3(upX, upY, upZ);
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
    }

    // 求观察矩阵
    glm::mat4 GetViewMatrix() {
        return glm::lookAt(Position, Position + Front, Up);
    }

    // 处理键盘输入
    void ProcessKeyboard(Camera_Movement direction, float deltaTime) {
        float velocity = MovementSpeed * deltaTime;
        if (direction == FORWARD)  // 前移
            Position += Front * velocity;
        else if (direction == BACKWARD)  // 后移
            Position -= Front * velocity;
        else if (direction == LEFT)  // 左移
            Position -= Right * velocity;
        else if (direction == RIGHT)  // 右移
            Position += Right * velocity;
        else if (direction == UP)  // 上移
            Position.y += velocity;
        else if (direction == DOWN)  // 下移
            Position.y -= velocity;
    }

    // 处理鼠标输入
    void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true) {
        xoffset *= MouseSensitivity;
        yoffset *= MouseSensitivity;

        Yaw += xoffset;
        Pitch += yoffset;

        // 将俯仰角控制在 [-89.0, 89.0] 范围内, 避免屏幕翻转
        if (constrainPitch) {
            if (Pitch > 89.0f) Pitch = 89.0f;
            if (Pitch < -89.0f) Pitch = -89.0f;
        }

        // 更新向量 Front 、Right 、Up
        updateCameraVectors();
    }

    // 处理滚轮输入
    void ProcessMouseScroll(float yoffset) {
        // 滚轮控制摄像机缩放, 视野在 [1.0, 45.0] 范围内
        Zoom -= (float)yoffset;
        if (Zoom < 1.0f) Zoom = 1.0f;
        if (Zoom > 45.0f) Zoom = 45.0f;
    }

private:
    // 用 Euler 角更新向量 Front 、Right 、Up
    void updateCameraVectors() {
        glm::vec3 front;
        front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        front.y = sin(glm::radians(Pitch));
        front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        Front = glm::normalize(front);
        
        // 单位化向量的原因: 向上或向下看会导致向量长度趋于 0 , 导致移动变慢
        Right = glm::normalize(glm::cross(Front, WorldUp));
        Up = glm::normalize(glm::cross(Right, Front));
    }
};
#endif