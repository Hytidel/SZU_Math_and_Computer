
attribute vec3 vPosition;
attribute vec3 vColor;
attribute vec3 vNormal;
attribute vec2 vTexture;

// 传给片元着色器的变量
varying vec3 position;
varying vec3 normal;
varying vec2 texCoord;

// 模型变换矩阵、相机观察矩阵、投影矩阵
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


void main() 
{
	vec4 v1 = model * vec4(vPosition, 1.0);  
	// 由于model矩阵有可能为阴影矩阵，为了得到正确位置，我们需要做一次透视除法
	vec4 v2 = vec4(v1.xyz / v1.w, 1.0);
	// 考虑相机和投影
	vec4 v3 = projection* view * v2;
	
	gl_Position = v3;

    position = vec3(v2.xyz);
    normal = vec3( (model *  vec4(vNormal, 0.0)).xyz );
	texCoord = vTexture;
}
