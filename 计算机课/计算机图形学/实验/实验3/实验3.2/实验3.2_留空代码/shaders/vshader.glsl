// 顶点着色器
#version 330 core

in vec3 vPosition;
in vec3 vColor;
out vec3 color;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


void main() 
{
	vec4 v1 = model * vec4(vPosition, 1.0);  
	// 由于model矩阵有可能为阴影矩阵，为了得到正确位置，我们需要做一次透视除法
	vec4 v2 = vec4(v1.xyz / v1.w, 1.0);
	// 考虑相机和投影
	vec4 v3 = projection* view *v2;

	gl_Position = v3;

	color = vColor;
}