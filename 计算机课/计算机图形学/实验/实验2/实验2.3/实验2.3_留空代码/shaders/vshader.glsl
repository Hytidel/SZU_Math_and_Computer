#version 330 core

in vec3 vColor;
in vec3 vPosition;

out vec3 color;

// TODO: 定义用于接受变换矩阵的 uniform 变量，利用变换矩阵完成变换

void main()
{
    gl_Position = vec4(vPosition, 1.0);
    color = vColor;
}

