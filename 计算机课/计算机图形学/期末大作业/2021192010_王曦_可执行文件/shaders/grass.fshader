#version 330 core

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture1;

void main() {    
    vec4 texColor = texture(texture1, TexCoords);
    if (texColor.a < 0.1) discard;  // 丢弃不透明度 < 0.1 的片段

    FragColor = texture(texture1, TexCoords);
}