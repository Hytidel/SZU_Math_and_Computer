#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "camera.h"

#include <cmath>
#include <vector>

unsigned int cubeVAO = 0, cubeVBO = 0;  // 正方体的 VAO 和 VBO

// 在归一化坐标中渲染一个 2 * 2 * 2 的正方体
void renderCube() {
    if (cubeVAO == 0) {  // 初始化
        // 正方体的顶点
        float vertices[] = {
            // 顶点坐标           法向量        纹理坐标
            // 背面
            -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f,  // 左下角
             1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f,  // 右上角
             1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f,  // 右下角
             1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f,  // 右上角
            -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f,  // 左下角
            -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f,  // 左上角
            // 正面
            -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f,  // 左下角
             1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f,  // 右下角
             1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f,  // 右上角
             1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f,  // 右上角
            -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f,  // 左上角
            -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f,  // 左下角
            // 左面
            -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f,  // 右上角
            -1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f,  // 左上角
            -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f,  // 左下角
            -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f,  // 左下角
            -1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f,  // 右下角
            -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f,  // 右上角
            // 右面
             1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f,  // 左上角
             1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f,  // 右下角
             1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f,  // 右上角         
             1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f,  // 右下角
             1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f,  // 左上角
             1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f,  // 左下角     
            // 底面
            -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f,  // 右上角
             1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f,  // 左上角
             1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f,  // 左下角
             1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f,  // 左下角
            -1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f,  // 右下角
            -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f,  // 右上角
            // 顶面
            -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f,  // 左上角
             1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f,  // 右下角
             1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f,  // 右上角     
             1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f,  // 右下角
            -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f,  // 左上角
            -1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f   // 左下角        
        };

        glGenBuffers(1, &cubeVBO);
        glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glGenVertexArrays(1, &cubeVAO);
        glBindVertexArray(cubeVAO);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    // 绘制正方体
    glBindVertexArray(cubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}

// 车的移动方向
enum CarMovement {
    CAR_FORWARD,
    CAR_BACKWARD,
    CAR_LEFT,
    CAR_RIGHT
};

// 控制车朝向的变量
const float CAR_YAW = -90.0f;  // 偏航角, 注意不是初始化为 0.0f
const float CAR_PITCH = 0.0f;  // 俯仰角
const float CAR_SPEED = 5.0f;  // 速度
const float CAR_SENSITIVITY = 0.05f;  // 鼠标灵敏度

// 车类
struct Car {
// ---------= 成员变量 =---------
    // 车属性
    glm::vec3 position;  // 位置坐标
    glm::vec3 front; 
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp;

    // Euler 角
    float yaw;  // 偏航角
    float pitch;  // 俯仰角

    // 车参数
    float movementSpeed;  // 移动速度
    float mouseSensitivity;  // 鼠标灵敏度

    // 风车
    bool blowing;
    float currentTime;
    float theta = 60.0f;  // 起始角度
    float deltaTheta = 5.0f;  // 转速

// ---------= 构造与析构 =---------
    Car(glm::vec3 _position = glm::vec3(0.0f), glm::vec3 _up = glm::vec3(0.0f, 1.0f, 0.0f), 
        float _yaw = CAR_YAW, float _pitch = CAR_PITCH) 
        : position(_position), up(_up), worldUp(_up), yaw(_yaw), pitch(_pitch), 
        front(glm::vec3(0.0f, 0.0f, -1.0f)), movementSpeed(CAR_SPEED), blowing(false), 
        mouseSensitivity(CAR_SENSITIVITY) {
        updateCarVectors();
    }

// ---------= 成员函数 =---------
    // 用 Euler 角更新向量 Front 、Right 、Up
    void updateCarVectors() {
        glm::vec3 _front;
        _front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        _front.y = sin(glm::radians(pitch));
        _front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        front = glm::normalize(_front);
        
        // 单位化向量的原因: 向上或向下看会导致向量长度趋于 0 , 导致移动变慢
        right = glm::normalize(glm::cross(front, worldUp));
        up = glm::normalize(glm::cross(right, front));
    }

    // 处理键盘输入
    void processKeyboard(CarMovement direction, float deltaTime) {
        float velocity = movementSpeed * deltaTime;
        if (direction == CAR_FORWARD)  // 前移
            position += front * velocity;
        else if (direction == CAR_BACKWARD)  // 后移
            position -= front * velocity;
        else if (direction == CAR_LEFT)  // 左移
            position -= right * velocity;
        else if (direction == CAR_RIGHT)  // 右移
            position += right * velocity;
        
        if (position.y < -0.24) position.y = -0.24;
    }

    // 处理鼠标输入
    void processMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true) {
        xoffset *= mouseSensitivity;
        yoffset *= mouseSensitivity;

        yaw += xoffset;
        pitch += yoffset;

        // 将俯仰角控制在 [-89.0, 89.0] 范围内, 避免屏幕翻转
        if (constrainPitch) {
            if (pitch > 89.0f) pitch = 89.0f;
            if (pitch < -89.0f) pitch = -89.0f;
        }

        // 更新向量 Front 、Right 、Up
        updateCarVectors();
    }

    // 绘制车
    void draw(const Shader& shader) {
        // 车身
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, position);
        // model = glm::rotate(model, -glm::radians(camera.Yaw), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, -glm::radians(yaw), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.5f, 0.25f, 0.5f));
        shader.setMat4("model", model);
        renderCube();

        // 竖杆
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(position.x, position.y + 1.0f, position.z));
        // model = glm::rotate(model, -glm::radians(camera.Yaw), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, -glm::radians(yaw), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.1f, 0.75f, 0.1f));
        shader.setMat4("model", model);
        renderCube();

        // 连接处 1
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(position.x, position.y + 1.75f, position.z));
        // model = glm::rotate(model, glm::radians(camera.Pitch), glm::normalize(camera.Right));
        model = glm::rotate(model, glm::radians(pitch), glm::normalize(right));
        // model = glm::rotate(model, -glm::radians(camera.Yaw), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, -glm::radians(yaw), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.15f, 0.15f, 0.15f));
        shader.setMat4("model", model);
        renderCube();

        // 横杆
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(position.x, position.y + 1.75f, position.z));
        // model = glm::translate(model, camera.Front * 0.25f);
        model = glm::translate(model, front * 0.25f);
        // model = glm::rotate(model, glm::radians(camera.Pitch), glm::normalize(camera.Right));
        model = glm::rotate(model, glm::radians(pitch), glm::normalize(right));
        // model = glm::rotate(model, -glm::radians(camera.Yaw), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, -glm::radians(yaw), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.6f, 0.1f, 0.1f));
        shader.setMat4("model", model);
        renderCube();

        // 连接处 2
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(position.x, position.y + 1.75f, position.z));
        // model = glm::translate(model, camera.Front * 1.0f);
        model = glm::translate(model, front * 1.0f);
        // model = glm::rotate(model, glm::radians(camera.Pitch), glm::normalize(camera.Right));
        model = glm::rotate(model, glm::radians(pitch), glm::normalize(right));
        // model = glm::rotate(model, -glm::radians(camera.Yaw), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, -glm::radians(yaw), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.15f, 0.15f, 0.15f));
        shader.setMat4("model", model);
        renderCube();

        if (blowing) {
            theta += deltaTheta;
            if (theta >= 180.0f) theta = 0.0f;
        }

        // 扇叶(中)
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(position.x, position.y + 1.75f, position.z));
        // model = glm::translate(model, camera.Front * 1.0f);
        model = glm::translate(model, front * 1.0f);
        // model = glm::rotate(model, glm::radians(0.0f), camera.Front);
        model = glm::rotate(model, glm::radians(theta - 60.0f), front);
        // model = glm::rotate(model, glm::radians(camera.Pitch), glm::normalize(camera.Right));
        model = glm::rotate(model, glm::radians(pitch), glm::normalize(right));
        // model = glm::rotate(model, -glm::radians(camera.Yaw), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, -glm::radians(yaw), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.025f, 0.75f, 0.10f));
        shader.setMat4("model", model);
        renderCube();

        // 扇叶(右)
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(position.x, position.y + 1.75f, position.z));
        // model = glm::translate(model, glm::normalize(camera.Front) * 1.0f);
        model = glm::translate(model, glm::normalize(front) * 1.0f);
        // model = glm::rotate(model, glm::radians(60.0f), camera.Front);
        model = glm::rotate(model, glm::radians(theta), front);
        // model = glm::rotate(model, glm::radians(camera.Pitch), glm::normalize(camera.Right));
        model = glm::rotate(model, glm::radians(pitch), glm::normalize(right));
        // model = glm::rotate(model, -glm::radians(camera.Yaw), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, -glm::radians(yaw), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.025f, 0.75f, 0.10f));
        shader.setMat4("model", model);
        renderCube();

        // 扇叶(左)
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(position.x, position.y + 1.75f, position.z));
        // model = glm::translate(model, glm::normalize(camera.Front) * 1.0f);
        model = glm::translate(model, glm::normalize(front) * 1.0f);
        // model = glm::rotate(model, glm::radians(-60.0f), camera.Front);
        model = glm::rotate(model, glm::radians(theta + 60.0f), front);
        // model = glm::rotate(model, glm::radians(camera.Pitch), glm::normalize(camera.Right));
        model = glm::rotate(model, glm::radians(pitch), glm::normalize(right));
        // model = glm::rotate(model, -glm::radians(camera.Yaw), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, -glm::radians(yaw), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.025f, 0.75f, 0.10f));
        shader.setMat4("model", model);
        renderCube();
    }



};

