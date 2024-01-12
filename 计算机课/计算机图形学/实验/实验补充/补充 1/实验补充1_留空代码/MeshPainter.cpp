#include "MeshPainter.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

MeshPainter::MeshPainter(){};
MeshPainter::~MeshPainter(){};

std::vector<std::string> MeshPainter::getMeshNames(){ return mesh_names;};
std::vector<TriMesh *> MeshPainter::getMeshes(){ return meshes;};
std::vector<openGLObject> MeshPainter::getOpenGLObj(){ return opengl_objects;};

void MeshPainter::bindObjectAndData(TriMesh *mesh, openGLObject &object, const std::string &texture_image, const std::string &vshader, const std::string &fshader){
    // 初始化各种对象

    const std::vector<glm::vec3>& points = mesh->getPoints();
    const std::vector<glm::vec3>& normals = mesh->getNormals();
    const std::vector<glm::vec3>& colors = mesh->getColors();
    const std::vector<glm::vec2>& textures = mesh->getTextures();

	// 创建顶点数组对象
	glGenVertexArrays(1, &object.vao);  	// 分配1个顶点数组对象
	glBindVertexArray(object.vao);  	// 绑定顶点数组对象

	// 创建并初始化顶点缓存对象
	glGenBuffers(1, &object.vbo);
	glBindBuffer(GL_ARRAY_BUFFER, object.vbo);
    glBufferData(GL_ARRAY_BUFFER,
                 points.size() * sizeof(glm::vec3) +
                     normals.size() * sizeof(glm::vec3) +
                     colors.size() * sizeof(glm::vec3) +
                     textures.size() * sizeof(glm::vec2),
                 NULL, GL_STATIC_DRAW);

    // 绑定顶点数据
    glBufferSubData(GL_ARRAY_BUFFER, 0, points.size() * sizeof(glm::vec3), points.data());
    // 绑定法向量数据
    glBufferSubData(GL_ARRAY_BUFFER, points.size() * sizeof(glm::vec3), normals.size() * sizeof(glm::vec3), normals.data());
    // 绑定颜色数据
    glBufferSubData(GL_ARRAY_BUFFER, (points.size() + normals.size()) * sizeof(glm::vec3), colors.size() * sizeof(glm::vec3), colors.data());
    // 绑定纹理数据
    glBufferSubData(GL_ARRAY_BUFFER, (points.size() + normals.size() + colors.size()) * sizeof(glm::vec3), textures.size() * sizeof(glm::vec2), textures.data());


	object.vshader = vshader;
	object.fshader = fshader;
	object.program = InitShader(object.vshader.c_str(), object.fshader.c_str());

    // 将顶点传入着色器
	object.pLocation = glGetAttribLocation(object.program, "vPosition");
	glEnableVertexAttribArray(object.pLocation);
	glVertexAttribPointer(object.pLocation, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

    // 将法向量传入着色器
	object.nLocation = glGetAttribLocation(object.program, "vNormal");
	glEnableVertexAttribArray(object.nLocation);
	glVertexAttribPointer(object.nLocation, 3, 
		GL_FLOAT, GL_FALSE, 0, 
		BUFFER_OFFSET( (points.size() )  * sizeof(glm::vec3)));

    // 将颜色传入着色器
	object.cLocation = glGetAttribLocation(object.program, "vColor");
	glEnableVertexAttribArray(object.cLocation);
	glVertexAttribPointer(object.cLocation, 3, GL_FLOAT, GL_FALSE, 0, 
        BUFFER_OFFSET((points.size() + normals.size() ) * sizeof(glm::vec3)));

	object.tLocation = glGetAttribLocation(object.program, "vTexture");
	glEnableVertexAttribArray(object.tLocation);
	glVertexAttribPointer(object.tLocation, 2, 
		GL_FLOAT, GL_FALSE, 0, 
		BUFFER_OFFSET( ( points.size() + colors.size() + normals.size())  * sizeof(glm::vec3)));


	// 获得矩阵位置
	object.modelLocation = glGetUniformLocation(object.program, "model");
	object.viewLocation = glGetUniformLocation(object.program, "view");
	object.projectionLocation = glGetUniformLocation(object.program, "projection");

	object.shadowLocation = glGetUniformLocation(object.program, "isShadow");

    object.texture_image = texture_image;
    // 读取纹理图片数
    // 创建纹理的缓存对象
    glGenTextures(1, &object.texture);
    // 调用stb_image生成纹理
    load_texture_STBImage(object.texture_image, object.texture);
    // 传递纹理数据 将生成的纹理传给shader
    glUniform1i(glGetUniformLocation(object.program, "texture"), 0);
    // Clean up
    glUseProgram(0);

    glBindVertexArray(0);


};

void MeshPainter::bindLightAndMaterial( TriMesh* mesh, openGLObject &object, Light* light, Camera* camera ) {
    // 传递材质、光源、相机等数据给着色器
    
	// 传递相机的位置
	glUniform3fv(glGetUniformLocation(object.program, "eye_position"), 1, &camera->eye[0]);

	// 传递物体的材质
	glm::vec4 meshAmbient = mesh->getAmbient();
	glm::vec4 meshDiffuse = mesh->getDiffuse();
	glm::vec4 meshSpecular = mesh->getSpecular();
	float meshShininess = mesh->getShininess();

	glUniform4fv(glGetUniformLocation(object.program, "material.ambient"), 1, &meshAmbient[0]);
	glUniform4fv(glGetUniformLocation(object.program, "material.diffuse"), 1, &meshDiffuse[0]);
	glUniform4fv(glGetUniformLocation(object.program, "material.specular"), 1, &meshSpecular[0]);
	glUniform1f(glGetUniformLocation(object.program, "material.shininess"), meshShininess);


	// 传递光源信息
	glm::vec4 lightAmbient = light->getAmbient();
	glm::vec4 lightDiffuse = light->getDiffuse();
	glm::vec4 lightSpecular = light->getSpecular();
	glm::vec3 lightPosition = light->getTranslation();
	glUniform4fv(glGetUniformLocation(object.program, "light.ambient"), 1, &lightAmbient[0]);
	glUniform4fv(glGetUniformLocation(object.program, "light.diffuse"), 1, &lightDiffuse[0]);
	glUniform4fv(glGetUniformLocation(object.program, "light.specular"), 1, &lightSpecular[0]);
	glUniform3fv(glGetUniformLocation(object.program, "light.position"), 1, &lightPosition[0]);

	glUniform1f(glGetUniformLocation(object.program, "light.constant"), light->getConstant());
	glUniform1f(glGetUniformLocation(object.program, "light.linear"), light->getLinear());
	glUniform1f(glGetUniformLocation(object.program, "light.quadratic"), light->getQuadratic());

}

void MeshPainter::addMesh( TriMesh* mesh, const std::string &name, const std::string &texture_image, const std::string &vshader, const std::string &fshader ){
	mesh_names.push_back(name);
    meshes.push_back(mesh);

    openGLObject object;
    // 绑定openGL对象，并传递顶点属性的数据
    bindObjectAndData(mesh, object, texture_image, vshader, fshader);

    opengl_objects.push_back(object);
};

void MeshPainter::drawMesh(TriMesh* mesh, openGLObject &object, Light *light, Camera* camera){
    
    // 相机矩阵计算
	camera->updateCamera();
	camera->viewMatrix = camera->getViewMatrix();
	camera->projMatrix = camera->getProjectionMatrix(false);


	glBindVertexArray(object.vao);

	glUseProgram(object.program);

	// 物体的变换矩阵
	glm::mat4 modelMatrix = mesh->getModelMatrix();

	// 传递矩阵
	glUniformMatrix4fv(object.modelLocation, 1, GL_TRUE, &modelMatrix[0][0]);
	glUniformMatrix4fv(object.viewLocation, 1, GL_TRUE, &camera->viewMatrix[0][0]);
	glUniformMatrix4fv(object.projectionLocation, 1, GL_TRUE, &camera->projMatrix[0][0]);
	// 将着色器 isShadow 设置为0，表示正常绘制的颜色，如果是1着表示阴影
	glUniform1i(object.shadowLocation, 0);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, object.texture);// 该语句必须，否则将只使用同一个纹理进行绘制

    
	// 将材质和光源数据传递给着色器
	// bindLightAndMaterial(mesh, object, light, camera);
	// 绘制
	glDrawArrays(GL_TRIANGLES, 0, mesh->getPoints().size());


	glBindVertexArray(0);

	glUseProgram(0);

};


void MeshPainter::drawMesh(int i, glm::mat4 modelMatrix, Light *light, Camera* camera){
    
	openGLObject &object = opengl_objects[i];
    // 相机矩阵计算
	camera->updateCamera();
	camera->viewMatrix = camera->getViewMatrix();
	camera->projMatrix = camera->getProjectionMatrix(false);


	glBindVertexArray(object.vao);

	glUseProgram(object.program);

	// 传递矩阵
	glUniformMatrix4fv(object.modelLocation, 1, GL_FALSE, &modelMatrix[0][0]);
	glUniformMatrix4fv(object.viewLocation, 1, GL_TRUE, &camera->viewMatrix[0][0]);
	glUniformMatrix4fv(object.projectionLocation, 1, GL_TRUE, &camera->projMatrix[0][0]);
	// 将着色器 isShadow 设置为0，表示正常绘制的颜色，如果是1着表示阴影
	glUniform1i(object.shadowLocation, 0);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, object.texture);// 该语句必须，否则将只使用同一个纹理进行绘制

    
	// 将材质和光源数据传递给着色器
	// bindLightAndMaterial(mesh, object, light, camera);
	// 绘制
	// drawMesh(meshes[i], opengl_objects[i], light, camera);

	glDrawArrays(GL_TRIANGLES, 0,  meshes[i]->getPoints().size());


	glBindVertexArray(0);

	glUseProgram(0);

};
void MeshPainter::drawMeshes(Light *light, Camera* camera){
    for (int i = 0; i < meshes.size(); i++)
    {
        drawMesh(meshes[i], opengl_objects[i], light, camera);
    }
};

void MeshPainter::cleanMeshes(){
    // 将数据都清空释放
    mesh_names.clear();

    for (int i = 0; i < meshes.size(); i++)
    {
        meshes[i]->cleanData();

        delete meshes[i];
        meshes[i] = NULL;


        glDeleteVertexArrays(1, &opengl_objects[i].vao);

        glDeleteBuffers(1, &opengl_objects[i].vbo);
        glDeleteProgram(opengl_objects[i].program);
    }

    meshes.clear();
    opengl_objects.clear();
};

void MeshPainter::controlMesh(unsigned char key, int x, int y, const std::string &selected_mesh_name) {
	// xxx
}


void MeshPainter::load_texture_STBImage(const std::string& file_name, GLuint& m_texName)
{
	int width, height, channels = 0;
	unsigned char *pixels = NULL;
	stbi_set_flip_vertically_on_load(true);
	pixels = stbi_load(file_name.c_str(), &width, &height, &channels, 0);

	// 调整行对齐格式
	if(width*channels%4!=0) glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	GLenum format = GL_RGB;
	// 设置通道格式
	switch (channels){
	case 1: format=GL_RED;break;
	case 3: format=GL_RGB;break;
	case 4: format=GL_RGBA;break;
	default: format=GL_RGB;break;
	}

	// 绑定纹理对象
	glBindTexture(GL_TEXTURE_2D, m_texName);

	// 指定纹理的放大，缩小滤波，使用线性方式，即当图片放大的时候插值方式
	// 将图片的rgb数据上传给opengl
	glTexImage2D(
		GL_TEXTURE_2D,	// 指定目标纹理，这个值必须是GL_TEXTURE_2D
		0,				// 执行细节级别，0是最基本的图像级别，n表示第N级贴图细化级别
		format,			// 纹理数据的颜色格式(GPU显存)
		width,			// 宽度。早期的显卡不支持不规则的纹理，则宽度和高度必须是2^n
		height,			// 高度。早期的显卡不支持不规则的纹理，则宽度和高度必须是2^n
		0,				// 指定边框的宽度。必须为0
		format,			// 像素数据的颜色格式(CPU内存)
		GL_UNSIGNED_BYTE,	// 指定像素数据的数据类型
		pixels			// 指定内存中指向图像数据的指针
	);
	
	// 生成多级渐远纹理，多消耗1/3的显存，较小分辨率时获得更好的效果
	// glGenerateMipmap(GL_TEXTURE_2D);

	// 指定插值方法
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	// 恢复初始对齐格式
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	// 释放图形内存
	stbi_image_free(pixels);
};
