#pragma once

#include "rigid.h"
#include "spring.h"

#include <vector>

// 布料类
struct Cloth {
// ---------= 成员变量 =---------
    const int pointsDensity = 4;  // 相邻两质点的间隔
    const int iterationFrequency = 25;  // 帧率
    const double structuralCoefficient = 1000.0;  // 结构系数
    const double shearCoefficient = 50.0;  // 剪切系数
    const double bendingCoefficient = 400.0;  // 弯曲系数

    enum DrawMode {
        DRAW_NODES,  // 绘制点
        DRAW_LINES,  // 绘制线
        DRAW_FACES  // 绘制面
    };
    DrawMode drawMode = DRAW_FACES;  // 绘制模式

    float width, height;  // 宽度, 高度
    int pointsPerRow, pointsPerColumn;  // 每行、每列的质点数

    glm::vec3 position;  // 位置坐标

    std::vector<MassPoint*> points;  // 质点
    std::vector<Spring*> springs;  // 弹簧
    std::vector<MassPoint*> faces;  // 面

    glm::vec2 pin1, pin2;  // 固定布料的两枚钉子

// ---------= 构造与析构 =---------
    Cloth(glm::vec3 _position, glm::vec2 _size) : position(_position) {
        width = _size.x, height = _size.y;
        
        init();
    }

    ~Cloth() {
        for (int i = 0; i < points.size(); i++) delete points[i];
        for (int i = 0; i < springs.size(); i++) delete springs[i];
        
        points.clear(), springs.clear(), faces.clear();
    }

// ---------= 成员函数 =---------
    // 求下标 (x, y) 处的质点
    MassPoint* getNode(int x, int y) {
        return points[y * pointsPerRow + x];
    }

    // 初始化成员变量
    void init() {
        pointsPerRow = width * pointsDensity;
        pointsPerColumn = height * pointsDensity;
        
        // 生成质点
        for (int i = 0; i < pointsPerRow; i++) {
            for (int j = 0; j < pointsPerColumn; j++) {
                MassPoint *point = new MassPoint(glm::vec3((double)j / pointsDensity, -((double)i / pointsDensity), 0));

                // 设置材质坐标
                point->textureCoordinate.x = (double)j / (pointsPerRow - 1);
                point->textureCoordinate.y = (double)i / (1 - pointsPerColumn);
                
                points.push_back(point);
            }
        }
        
        // 生成弹簧
        for (int i = 0; i < pointsPerRow; i++) {
            for (int j = 0; j < pointsPerColumn; j++) {
                // 结构系数
                if (i < pointsPerRow - 1) 
                    springs.push_back(new Spring(getNode(i, j), getNode(i + 1, j), structuralCoefficient));
                if (j < pointsPerColumn - 1) 
                    springs.push_back(new Spring(getNode(i, j), getNode(i, j + 1), structuralCoefficient));
               
                // 剪切系数
                if (i < pointsPerRow - 1 && j < pointsPerColumn - 1) {
                    springs.push_back(new Spring(getNode(i, j), getNode(i + 1, j + 1), shearCoefficient));
                    springs.push_back(new Spring(getNode(i + 1, j), getNode(i, j + 1), shearCoefficient));
                }
                
                // 弯曲系数
                if (i < pointsPerRow - 2) 
                    springs.push_back(new Spring(getNode(i, j), getNode(i + 2, j), bendingCoefficient));
                if (j < pointsPerColumn - 2) 
                    springs.push_back(new Spring(getNode(i, j), getNode(i, j + 2), bendingCoefficient));
            }
        }

        // 取布料上边的两顶点作为固定点
        pin1 = glm::vec2(0, 0), pin2 = glm::vec2(pointsPerRow - 1, 0);
        // 固定点
        pin(pin1, glm::vec3(1.0, 0.0, 0.0));
        pin(pin2, glm::vec3(-1.0, 0.0, 0.0));
        
		// 三角面片
        for (int i = 0; i < pointsPerRow - 1; i++) {
            for (int j = 0; j < pointsPerColumn - 1; j++) {
                //  *--* //
                //  | /  //
                //  *    //
                faces.push_back(getNode(i + 1, j));
                faces.push_back(getNode(i, j));
                faces.push_back(getNode(i, j + 1));

                //     * //
                //   / | //
                //  *--* //
                faces.push_back(getNode(i + 1, j + 1));
                faces.push_back(getNode(i + 1, j));
                faces.push_back(getNode(i, j + 1));
            }
        }
	}

    // 给下标 index 的质点的位置加一个偏移量后固定
    void pin(glm::vec2 index, glm::vec3 offset) {
        if (index.x < 0 || index.x >= pointsPerRow || index.y < 0 || index.y >= pointsPerColumn) return;

        getNode(index.x, index.y)->position += offset;
        getNode(index.x, index.y)->isFixed = true;
    }

    // 取消下标 index 的质点的固定
    void unpin(glm::vec2 index) {
        if (index.x < 0 || index.x >= pointsPerRow || index.y < 0 || index.y >= pointsPerColumn) return;

        getNode(index.x, index.y)->isFixed = false;
    }

    // 求三个顶点 point1, point2, point3 构成的面的法向量
    glm::vec3 getFaceNormal(MassPoint *point1, MassPoint *point2, MassPoint *point3) {
        return cross(point2->position - point1->position, point3->position - point1->position);
    }

    // 求各面的法向量
    void getNormal() {
        glm::vec3 normal(0.0, 0.0, 0.0);
        for (int i = 0; i < points.size(); i++) points[i]->normal = normal;

        for (int i = 0; i < faces.size() / 3; i++) {
            MassPoint *point1 = faces[3 * i + 0];
            MassPoint *point2 = faces[3 * i + 1];
            MassPoint *point3 = faces[3 * i + 2];

            normal = getFaceNormal(point1, point2, point3);
            point1->normal += normal;
            point2->normal += normal;
            point3->normal += normal;
        }

        for (int i = 0; i < points.size(); i++)
            points[i]->normal = glm::normalize(points[i]->normal);
    }

    // 施加力
    void applyForce(glm::vec3 force) {
        for (int i = 0; i < points.size(); i++) points[i]->applyForce(force);
    }

    // 计算力
    void getForce(float timeInterval, glm::vec3 gravity) {
        // 质点受重力
        for (int i = 0; i < points.size(); i++)
            points[i]->applyForce(gravity * points[i]->mass);

        // 弹簧受内力
        for (int i = 0; i < springs.size(); i++)
            springs[i]->applyInternalForce(timeInterval);
    }

    // 显式积分
    void integrate(float timeInterval) {
        for (int i = 0; i < points.size(); i++) 
            points[i]->integrate(timeInterval);
    }

    // 求质点在世界坐标中的坐标
    glm::vec3 getWorldPosition(MassPoint *point) {
        return position + point->position;
    }

    // 设置质点在世界坐标中的坐标
    void setWorldPosition(MassPoint *point, glm::vec3 pos) {
        point->position = pos - position;
    }

    // 碰撞检测
    void collisionResponse(Ground *ground, Ball *ball) {
        for (int i = 0; i < points.size(); i++) {
            // 布料与地面的碰撞
            if (getWorldPosition(points[i]).y < ground->position.y) {
                points[i]->position.y = ground->position.y - position.y + 0.01;  // + 0.01 避免深度冲突
                points[i]->velocity *= ground->friction;  // 布料受地面的摩擦力
            }

            // 布料与球的碰撞
            glm::vec3 distanceVector = getWorldPosition(points[i]) - ball->center;
            float distance = sqrt(dot(distanceVector, distanceVector));
            float safeDistance = ball->radius * 1.05;
            if (distance < safeDistance) {
                if (distance >= 0.0001) distanceVector = glm::normalize(distanceVector);
                setWorldPosition(points[i], distanceVector * safeDistance + ball->center);
                points[i]->velocity *= ball->friction;  // 布料受球的摩擦力
            }
        }
    }
};

