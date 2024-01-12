#version 330 core

layout (location = 0) in vec3 vsPosition;
layout (location = 1) in vec3 vsNormal;

out vec3 position;
out vec3 normal;

uniform mat4 uniform_modelMatrix;
uniform mat4 uniform_viewMatrix;
uniform mat4 uniform_projectionMatrix;

void main() {
    position = vsPosition;
    normal = vsNormal;
    
    gl_Position = uniform_projectionMatrix * uniform_viewMatrix * uniform_modelMatrix * vec4(vsPosition, 1.0f);
}
