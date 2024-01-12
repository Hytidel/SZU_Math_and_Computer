#version 330 core

out vec4 color;
out vec4 FragColor;

in vec3 position;
in vec3 normal;

uniform vec4 uniform_rigidColor;
uniform vec3 uniform_lightPosition;
uniform vec3 uniform_lightColor;

void main() {
    // 环境光
    float ambientStrength = 0.5f;
    vec3 ambient = ambientStrength * uniform_lightColor;

    // 漫反射
    vec3 lightDir = normalize(uniform_lightPosition - position);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * uniform_lightColor;

    vec3 objectColor = vec3(uniform_rigidColor.x, uniform_rigidColor.y, uniform_rigidColor.z);
    vec3 result = (ambient + diffuse) * objectColor;
    color = vec4(result, uniform_rigidColor.w);

    FragColor = vec4(1.0);
}
