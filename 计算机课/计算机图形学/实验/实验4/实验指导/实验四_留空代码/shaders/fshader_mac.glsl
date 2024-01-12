// 给光源数据一个结构体
struct Light{
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;

	vec3 position;

    // 光源衰减系数的三个参数
    float constant; // 常数项
    float linear;	// 一次项
    float quadratic;// 二次项

};

// 给物体材质数据一个结构体
struct Material{
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;

	float shininess;
};

varying vec3 position;
varying vec3 normal;
varying vec2 texCoord;

// 相机坐标
uniform vec3 eye_position;
// 光源
uniform Light light;
// 物体材质
uniform Material material;

// 纹理数据
uniform sampler2D texture;

uniform int isShadow;

void main()
{
	if (isShadow == 1) {
		gl_FragColor = vec4(0.0, 0.0, 0.0, 1.0);
	}
	else {

		gl_FragColor = texture2D( texture, texCoord );

	}
}
