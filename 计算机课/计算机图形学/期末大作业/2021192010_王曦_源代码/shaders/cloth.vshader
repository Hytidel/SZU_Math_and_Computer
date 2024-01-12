#version 330 core

layout (location = 0) in vec3 vsPosition;
layout (location = 1) in vec2 vsTexCoord;
layout (location = 2) in vec3 vsNormal;

out vec3 position;
out vec3 normal;
out vec2 textureCoordinate;

uniform mat4 uniform_modelMatrix;
uniform mat4 uniform_viewMatrix;
uniform mat4 uniform_projectionMatrix;

void main() {
    position = vsPosition;
    normal = vsNormal;
    textureCoordinate = vec2(vsTexCoord.x, vsTexCoord.y);

    gl_Position = uniform_projectionMatrix * uniform_viewMatrix * uniform_modelMatrix * vec4(vsPosition, 1.0f);
}
