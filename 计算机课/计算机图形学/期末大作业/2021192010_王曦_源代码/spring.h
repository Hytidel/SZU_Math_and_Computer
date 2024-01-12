#pragma once

#include "massPoint.h"

struct Spring {
// ---------= 成员变量 =---------
    MassPoint *point1, *point2;  // 弹簧的两端点

    float originalLength;  // 原长
    float hookCoefficient;  // 劲度系数
    float dampCoefficient;  // 阻尼系数

// ---------= 构造与析构 =---------
    Spring(MassPoint *_point1, MassPoint *_point2, float _hookCoefficient) 
        : point1(_point1), point2(_point2), 
        hookCoefficient(_hookCoefficient), dampCoefficient(5.0) {
        glm::vec3 currentVector = point2->position - point1->position;
        originalLength = sqrt(dot(currentVector, currentVector));
    }

// ---------= 成员函数 =---------
    // 计算内力
    void applyInternalForce(float timeInterval) {
        glm::vec3 currentVector = point2->position - point1->position;
        float currentLength = sqrt(dot(currentVector, currentVector));

        glm::vec3 fDirection = (point2->position - point1->position) / currentLength;
        glm::vec3 vDelta = point2->velocity - point1->velocity;
        glm::vec3 f = fDirection * ((currentLength - originalLength) * hookCoefficient
            + dot(vDelta, fDirection) * dampCoefficient);
        point1->applyForce(f), point2->applyForce(-f);
    }
};