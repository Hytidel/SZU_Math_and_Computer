#ifndef _MESH_PAINTER_H_
#define _MESH_PAINTER_H_

#include "TriMesh.h"
#include "Angel.h"

#include "Camera.h"

#include <vector>
#include <algorithm>


struct openGLObject
{
	// 顶点数组对象
	GLuint vao;
	// 顶点缓存对象
	GLuint vbo;

	// 着色器程序
	GLuint program;
	// 着色器文件
	std::string vshader;
	std::string fshader;
	// 着色器变量
	GLuint pLocation;
	GLuint cLocation;
	GLuint nLocation;
	GLuint tLocation;

    // 纹理
    std::string texture_image;
    GLuint texture;

	// 投影变换变量
	GLuint modelLocation;
	GLuint viewLocation;
	GLuint projectionLocation;

	// 阴影变量
	GLuint shadowLocation;
};


class MeshPainter
{

public:
    MeshPainter();
    ~MeshPainter();

    std::vector<std::string> getMeshNames();

    std::vector<TriMesh *> getMeshes();
    std::vector<openGLObject> getOpenGLObj();

	// 读取纹理文件
    void load_texture_STBImage(const std::string &file_name, GLuint& texture);

	// 传递光线材质数据的
    // void bindLightAndMaterial( int mesh_id, int light_id, Camera* camera );
    void bindLightAndMaterial(TriMesh* mesh, openGLObject& object, Light* light, Camera* camera);

    void bindObjectAndData(TriMesh *mesh, openGLObject &object, const std::string &texture_image, const std::string &vshader, const std::string &fshader);

	// 添加物体
    void addMesh( TriMesh* mesh, const std::string &name, const std::string &texture_image, const std::string &vshader, const std::string &fshader );

	// 绘制物体
    void drawMesh(TriMesh* mesh, openGLObject &object, Light *light, Camera* camera);
	void drawMesh(int i, glm::mat4 modelMatrix, Light *light, Camera* camera);

	// 绘制多个物体
    void drawMeshes(Light *light, Camera* camera);

	// 清空数据
    void cleanMeshes();

	// 键盘事件
	void controlMesh(unsigned char key, int x, int y, const std::string &selected_mesh_name);


private:
    std::vector<std::string> mesh_names;
    std::vector<TriMesh *> meshes;
    std::vector<openGLObject> opengl_objects;

};

#endif