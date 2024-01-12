#version 330 core

out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    vec4 FragPosLightSpace;
} fs_in;

uniform sampler2D diffuseTexture;
uniform sampler2D shadowMap;

uniform vec3 lightPos;
uniform vec3 viewPos;

uniform bool pcf;
uniform bool blinn;
uniform bool gamma;

float ShadowCalculation(vec4 fragPosLightSpace, vec3 normal, vec3 lightDir) {
    // 透视除法
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;

    // 映射到 [0,1] 范围内
    projCoords = projCoords * 0.5 + 0.5;

    // 求透视光方向的最近深度
    float closestDepth = texture(shadowMap, projCoords.xy).r; 

    // 求透视光方向的当前片段的深度
    float currentDepth = projCoords.z;
    
    // 检查当前片段是否在阴影中
    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);  // 阴影偏移

    float shadow = 0.0;
    if (pcf) {
        // PCF : 从纹理像素四周对深度贴图采样, 对结果取平均
        vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
        for(int x = -1; x <= 1; ++x) {
            for(int y = -1; y <= 1; ++y) {
                float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
                shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;        
            }    
        }
        shadow /= 9.0;
    }
    else shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;

    // 若点比光的远平面还远, 则环绕方式 GL_CLAMP_TO_BORDER 无效, 此时需手动设置 shadow
    if (projCoords.z > 1.0) shadow = 0.0;
    return shadow;
}

void main() {   
    vec3 color = texture(diffuseTexture, fs_in.TexCoords).rgb;
    vec3 normal = normalize(fs_in.Normal);
    vec3 lightColor = vec3(1.0f);

    // 环境光
    vec3 ambient = 0.3 * lightColor;

    // 漫反射
    vec3 lightDir = normalize(lightPos - fs_in.FragPos);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * lightColor;

    // 镜面反射
    vec3 viewDir = normalize(viewPos - fs_in.FragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = 0.0;
    if (blinn) {  // Blinn-Phong 模型
        vec3 halfwayDir = normalize(lightDir + viewDir);  
        spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);
    }
    else {
        vec3 reflectDir = reflect(-lightDir, normal);
        spec = pow(max(dot(viewDir, reflectDir), 0.0), 8.0);
    }
    vec3 specular = spec * lightColor;

    // 阴影
    float shadow = ShadowCalculation(fs_in.FragPosLightSpace, normal, lightDir);                      
    vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * color;    
    
    FragColor = vec4(lighting, 1.0);

    // Gamma 校正
    if (gamma) {
        float Gamma = 1.2f;
        FragColor.rgb = pow(FragColor.rgb, vec3(1.0 / Gamma));
    }
}