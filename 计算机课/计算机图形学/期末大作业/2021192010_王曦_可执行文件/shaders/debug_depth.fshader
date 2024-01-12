#version 330 core

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D depthMap;
uniform float near_plane;
uniform float far_plane;

// 使用透视投影时需该函数
float LinearizeDepth(float depth) {
    float z = depth * 2.0 - 1.0;  // 转回归一化坐标
    return (2.0 * near_plane * far_plane) / (far_plane + near_plane - z * (far_plane - near_plane));
}

void main() {
    float depthValue = texture(depthMap, TexCoords).r;

    // 透视投影
    // FragColor = vec4(vec3(LinearizeDepth(depthValue) / far_plane), 1.0);

    // 正交投影
    FragColor = vec4(vec3(depthValue), 1.0);
}