#version 330 core

out vec4 FragColor;

struct Material {
    sampler2D diffuse;  // 只保留漫反射, 因为环境光几乎都等于漫反射
    sampler2D specular;  // 镜面反射
    float shininess;  // 反光度
};

struct Light {
    vec3 position;  // 光源位置
    vec3 direction;  // 光源方向

    // 切光角
    float cutOff;  // 内圆锥
    float outerCutOff;  // 外圆锥

    vec3 ambient;  // 环境光
    vec3 diffuse;  // 漫反射
    vec3 specular;  // 镜面反射

    float constant;  // 常数项
    float linear;  // 一次项
    float quadratic;  // 二次项
};

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

uniform vec3 viewPos; 
uniform Material material;
uniform Light light;

uniform bool torch;  // 控制聚光是否开启

void main() {
    // 环境光
    vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;
    
    // 漫反射
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoords).rgb;  
    
    // 镜面反射
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * texture(material.specular, TexCoords).rgb;  
    
    // 聚光
    float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = (light.cutOff - light.outerCutOff);
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    diffuse *= intensity, specular *= intensity;
    
    // 衰减系数
    float distance = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    ambient *= attenuation; 
    diffuse *= attenuation;
    specular *= attenuation;   
        
    vec3 result = ambient + diffuse + specular;
    if (torch) FragColor = vec4(result, 1.0);
    else FragColor = vec4(ambient, 1.0);
}