#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

glm::vec3 zero(0.0, 0.0, 0.0);

// 点类
struct Point {
// ---------= 成员变量 =---------
    glm::vec3 position;  // 位置坐标
    glm::vec3 normal;  // 法向量

// ---------= 构造与析构 =---------
    Point() {}
    Point(glm::vec3 _position) : position(_position) {}
};

// 质点类
struct MassPoint {
// ---------= 成员变量 =---------
    float mass;  // 质量
    
    glm::vec3 position;  // 位置坐标
    glm::vec3 normal;  // 法向量
    glm::vec2 textureCoordinate;  // 材质坐标

    bool isFixed;  // 是否固定

    glm::vec3 force;  // 力
    glm::vec3 acceleration;  // 加速度
    glm::vec3 velocity;  // 速度

// ---------= 构造与析构 =---------
    MassPoint() : mass(1.0), isFixed(false) {
        force = acceleration = velocity = zero;
    }
    MassPoint(glm::vec3 _position) : mass(1.0), isFixed(false), position(_position) {
        force = acceleration = velocity = zero;
    }

// ---------= 成员函数 =---------
    // 施加力
    void applyForce(glm::vec3 f) {
        force += f;
    }

    // 显式积分
    void integrate(float timeInterval) {
        // 只更新未固定的质点
        if (!isFixed) {
            acceleration = force / mass;
            velocity += acceleration * timeInterval;
            position += velocity * timeInterval;
        }

        // 更新后清空力
        force = zero;
    }
};
