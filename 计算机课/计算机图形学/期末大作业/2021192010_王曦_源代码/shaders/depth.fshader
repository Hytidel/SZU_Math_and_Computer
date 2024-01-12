#version 330 core

void main() {
    // 因无颜色缓冲, 则片段无需处理, 用空的 fshader 即可
    // 也可取消注释, 显式设置深度, 但底层默认设置深度缓冲, 故注释更高效
    // gl_FragDepth = gl_FragCoord.z;
}