#version 330 core

// 给光源数据一个结构体
struct Light{
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;

	vec3 position;
};

// 给物体材质数据一个结构体
struct Material{
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;

	float shininess;
};

in vec3 vPosition;
in vec3 vColor;
// 顶点法向量
in vec3 vNormal;

// 传给片元着色器的变量
out vec4 color;
out vec3 norm;
out vec3 pos;

// 模型变换矩阵、相机观察矩阵、投影矩阵
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

// 相机坐标
uniform vec3 eye_position;
// 光源
uniform Light light;
// 物体材质
uniform Material material;



void main() 
{
	vec4 v1 = model * vec4(vPosition, 1.0);  
	// 由于model矩阵有可能为阴影矩阵，为了得到正确位置，我们需要做一次透视除法
	vec4 v2 = vec4(v1.xyz / v1.w, 1.0);
	// 考虑相机和投影
	vec4 v3 = projection * view * v2;
    
	gl_Position = v3;

    norm = vNormal;
    pos = vPosition;

    // 计算模视矩阵
    mat4 ModelView = view * model;

    // 将顶点坐标、光源坐标和法向量转换到相机坐标系
    vec3 pos = (ModelView * vec4(vPosition, 1.0) ).xyz;
    vec3 l_pos = (view * vec4(light.position, 1.0) ).xyz;
    vec3 norm = (ModelView * vec4(vNormal, 0.0)).xyz;

    // @TODO: 计算四个归一化的向量 N,V,L,R(或半角向量H)
    // vec3 N=
    // vec3 V=
    // vec3 L=
    // vec3 R=
    
    vec3 H = normalize( L + V );

    // 环境光分量I_a
    vec4 I_a = light.ambient * material.ambient;

    // @TODO: Task2 计算漫反射系数alpha和漫反射分量I_d
    float alpha = clamp( dot(L, N), 0.0, 1.0 );
    vec4 I_d = alpha *  light.diffuse * material.diffuse;

    // @TODO: Task2 计算高光系数beta和镜面反射分量I_s
    

    // 注意如果光源在背面则去除高光
    if( dot(L, N) < 0.0 ) {
	    I_s = vec4(0.0, 0.0, 0.0, 1.0);
    } 

    // 合并三个分量的颜色，修正透明度
    color = I_a + I_d + I_s;
    color.a = 1.0;

    //color = vec4(vNormal, 1.0);
}

