// 声明 GLSL 版本号以及使用的模式。
#version 330 core
// 声明着色器的输入。
in vec3 vColor;
in vec3 vPosition;
// 声明着色器的输出。
out vec3 color;
// 进行数据处理。
void main()
{
    gl_Position = vec4(vPosition, 1.0);
    color = vColor;
}

