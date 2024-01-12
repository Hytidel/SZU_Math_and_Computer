attribute vec3 vPosition;
varying vec4 color;

void main()
{
    gl_Position = vec4(vPosition, 1.0);
    color = vec4(vPosition + vec3(0.5, 0.5, 0.5), 1.0);
}