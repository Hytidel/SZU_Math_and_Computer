#version 330 core

in vec3 position;
in vec3 normal;
in vec3 color;

uniform int isShadow;

out vec4 fColor;

void main()
{
	if (isShadow == 1) {
		fColor = vec4(0.0, 0.0, 0.0, 1.0);
	}
	else {
		fColor = vec4(color, 1.0);
	}
}
