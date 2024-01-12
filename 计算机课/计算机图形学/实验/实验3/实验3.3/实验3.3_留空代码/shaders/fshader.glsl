#version 330 core

in vec4 color;
out vec4 fColor;

uniform int isShadow;

void main()
{
	if (isShadow == 1) {
		fColor = vec4(0.0, 0.0, 0.0, 1.0);
	}
	else {
		fColor = color;
	}
}
