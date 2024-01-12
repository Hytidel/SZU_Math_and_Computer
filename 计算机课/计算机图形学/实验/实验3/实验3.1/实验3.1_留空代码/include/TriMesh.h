#ifndef _TRI_MESH_H_
#define _TRI_MESH_H_

#include "Angel.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <string>


// 三角面片中的顶点序列
typedef struct vIndex {
	unsigned int x, y, z;
	vIndex(int ix, int iy, int iz) : x(ix), y(iy), z(iz) {}
} vec3i;

class TriMesh
{
public:
	TriMesh();
	~TriMesh();

	std::vector<glm::vec3> getVertexPositions();
	std::vector<glm::vec3> getVertexColors();
	std::vector<vec3i> getFaces();
	std::vector<glm::vec3> getPoints();
	std::vector<glm::vec3> getColors();

	// 这里添加各种形状的、模型的读取顶点的函数
	void generateCube();
	void readOff(const std::string& filename);

	// 将读取的顶点根据三角面片上的顶点下标逐个加入
	// 要传递给GPU的points等容器内
	void storeFacesPoints();

	// 清除数据
	void cleanData();

private:
	std::vector<glm::vec3> vertex_positions;	// 顶点坐标
	std::vector<glm::vec3> vertex_colors;	// 顶点颜色

	std::vector<vec3i> faces;	// 三角面片上每个顶点对应的下标

	std::vector<glm::vec3> points;	// 传入着色器的绘制点
	std::vector<glm::vec3> colors;	// 传入着色器的颜色
};

#endif