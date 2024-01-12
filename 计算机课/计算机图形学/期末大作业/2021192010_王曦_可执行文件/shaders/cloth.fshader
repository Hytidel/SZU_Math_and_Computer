#version 330 core

out vec4 color;

in vec3 position;
in vec2 textureCoordinate;
in vec3 normal;

// 纹理采样器
uniform sampler2D uniform_texture;

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

    color = texture(uniform_texture, textureCoordinate);
    vec3 objectColor = vec3(color.x, color.y, color.z);
    vec3 result = (ambient + diffuse) * objectColor;
    color = vec4(result, 1.0f);
}
