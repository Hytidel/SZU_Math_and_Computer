#version 330 core

in vec3 vColor;
in vec3 vPosition;

out vec3 color;

void main()
{
    gl_Position = vec4(vPosition, 1.0);
    color = vColor;
}

