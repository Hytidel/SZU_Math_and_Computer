#pragma once

#define _USE_MATH_DEFINES
#include <math.h>
#include <cmath>
#include <vector>

#include <iostream>

#include "massPoint.h"

// 地面类
struct Ground {
// ---------= 成员变量 =---------
    const float friction = 0.9;  // 摩擦系数

    int width, height;  // 宽度、高度
    glm::vec3 position;  // 位置坐标
    glm::vec4 color;  // 颜色

    std::vector<Point*> vertices;  // 顶点
    std::vector<Point*> faces;  // 面

// ---------= 构造与析构 =---------
    Ground(glm::vec3 _position, glm::vec2 _size, glm::vec4 _color) 
        : position(_position), color(_color) {
        width = _size.x, height = _size.y;

        init();
    }

    ~Ground() {
        for (int i = 0; i < vertices.size(); i++) delete vertices[i];
        vertices.clear(), faces.clear();
    }

// ---------= 成员函数 =---------
    // 初始化 vertices[] 和 faces[] 
    void init() {
        vertices.push_back(new Point(glm::vec3(0.0, 0.0, 0.0)));
        vertices.push_back(new Point(glm::vec3(width, 0.0, 0.0)));
        vertices.push_back(new Point(glm::vec3(0.0, 0.0, -height)));
        vertices.push_back(new Point(glm::vec3(width, 0.0, -height)));
        
        for (int i = 0; i < vertices.size(); i++) 
            vertices[i]->normal = glm::vec3(0.0, 1.0, 0.0);
        
        faces.push_back(vertices[0]);
        faces.push_back(vertices[1]);
        faces.push_back(vertices[2]);
        faces.push_back(vertices[1]);
        faces.push_back(vertices[2]);
        faces.push_back(vertices[3]);
    }
};

// 球类(构造)
struct Sphere {
// ---------= 成员变量 =---------
    const int parallelCnt = 250, meridianCnt = 24;  // 横竖方向的采样点数

    float radius;  // 半径

    std::vector<Point*> vertices;  // 顶点
    std::vector<Point*> faces;  // 面

// ---------= 构造与析构 =---------
    Sphere(float _radius) : radius(_radius) {
        init();
    }

    ~Sphere() {
        for (int i = 0; i < vertices.size(); i++) delete vertices[i];
        vertices.clear(), faces.clear();
    }

// ---------= 成员函数 =---------
    // 获取球的最高点
    Point* getTopPoint() {
        return vertices[0];
    }

    // 获取球的最低点
    Point* getBottomPoint() {
        return vertices.back();
    }
    
    // 获取下标 (x, y) 处的点
    Point* getPoint(int x, int y) {
        if (x < 0 || x >= parallelCnt || y < 0 || y >= meridianCnt) {
            std::cout << "Point index out of range." << std::endl;
            exit(-1);
        }

        return vertices[1 + x * meridianCnt + y];
    }

    // 求三个顶点 v1, v2, v3 构成的面的法向量
    glm::vec3 getFaceNormal(Point* v1, Point* v2, Point* v3) {
        return cross(v2->position - v1->position, v3->position - v1->position);
    }

    // 求球的各面的法向量
    void getSphereNormal() {
        glm::vec3 normal(0.0, 0.0, 0.0);
        for (int i = 0; i < vertices.size(); i++) vertices[i]->normal = normal;
        
        // 顶部截面和底部截面的圆的面的法向量后续特别计算
        for (int i = 0; i < faces.size() / 3; i++) {
            Point* v1 = faces[i * 3 + 0];
            Point* v2 = faces[i * 3 + 1];
            Point* v3 = faces[i * 3 + 2];
            
            normal = getFaceNormal(v1, v3, v2);
            v1->normal += normal;
            v2->normal += normal;
            v3->normal += normal;
        }
        
        for (int i = 0; i < vertices.size(); i++) 
            vertices[i]->normal = glm::normalize(vertices[i]->normal);
    }
    
    // 初始化 vertices[] 和 faces[] 
    void init() {
        // 精度
        double cycleInterval = radius * 2.0 / (parallelCnt + 1);  // 截面距离
        double radianInterval = 2.0 * M_PI / meridianCnt;  // 弧度差
        
        // 最高点
        glm::vec3 pos(0.0, radius, 0.0);
        vertices.push_back(new Point(pos));
        
        // 中间点
        for (int i = 0; i < parallelCnt; i++) {
            pos.y -= cycleInterval;

            for (int j = 0; j < meridianCnt; j++) {
                double xzLen = radius * sqrt(1.0 - pow(pos.y / radius, 2));
                double xRadian = j * radianInterval;  // 在 xz 平面的投影的弧长
                pos.x = xzLen * sin(xRadian), pos.z = xzLen * cos(xRadian);  // 圆上的点的坐标
                vertices.push_back(new Point(pos));
            }
        }

        // 最低点
        pos = glm::vec3(0.0, -radius, 0.0);
        vertices.push_back(new Point(pos));
        
        // 截面
        // 顶部截面
        for (int i = 0; i < meridianCnt; i++) {
            //   *   //
            //  / \  //
            // *---* //
            faces.push_back(getPoint(0, i));
            faces.push_back(getTopPoint());
            faces.push_back(getPoint(0, (i + 1) % meridianCnt));
        }
        // 中间截面
        for (int i = 0; i < parallelCnt - 1; i++) {
            for (int j = 0; j < meridianCnt; j++) {
                //  *--* //
                //  | /  //
                //  *    //
                faces.push_back(getPoint(i, j));
                faces.push_back(getPoint(i, (j + 1) % meridianCnt));
                faces.push_back(getPoint(i + 1, j));
                
                //     * //
                //   / | //
                //  *--* //
                faces.push_back(getPoint(i + 1, (j + 1) % meridianCnt));
                faces.push_back(getPoint(i + 1, j));
                faces.push_back(getPoint(i, (j + 1) % meridianCnt));
            }
        }
        // 底部截面
        for (int i = 0; i < meridianCnt; i ++) {
            // *---* //
            //  \ /  //
            //   *   //
            faces.push_back(getBottomPoint());
            faces.push_back(getPoint(parallelCnt - 1, i));
            faces.push_back(getPoint(parallelCnt - 1, (i + 1) % meridianCnt));
        }
        
        // 求各面的法向量
        getSphereNormal();
    }
};

// 球类
struct Ball {
    const float friction = 0.8;  // 摩擦系数

    glm::vec3 center;  // 球心
    float radius;  // 半径
    glm::vec4 color;  // 颜色
    Sphere *sphere;  // 球

    Ball(glm::vec3 _center, float _radius, glm::vec4 _color) 
        : center(_center), radius(_radius), color(_color) {
        sphere = new Sphere(radius);
    }
};
