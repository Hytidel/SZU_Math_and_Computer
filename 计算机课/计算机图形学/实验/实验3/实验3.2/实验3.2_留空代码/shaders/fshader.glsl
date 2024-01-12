#version 330 core

in vec3 color;
out vec4 fColor;

uniform int isShadow;

void main()
{
    // 如果是阴影着只有黑色
	if (isShadow == 0) {
		fColor = vec4(0.0, 0.0, 0.0, 1.0);
	}
	else {
		fColor = vec4(color, 1.0);
	}
}
