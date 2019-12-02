#version 330 core

in vec3 Normal;
in vec3 FragPos;

out vec4 FragColor;

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 objectColor;

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct Material {
    vec3 ambient;   // 环境光照
    vec3 diffuse;   // 漫反射光照
    vec3 specular;  // 镜面光照
    float shininess;// 镜面高光的散射
};

uniform Material material;
uniform Light light;

uniform vec3 viewPos;

void main() {

    // 环境光
    vec3 ambient = light.ambient * material.ambient;

    // 漫反射
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm,lightDir),0.0f);
    vec3 diffuse = (diff * material.diffuse) * light.diffuse;

    //float specularStrength = 0.5f;

    // 镜面光
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir,norm);
    float spec = pow(max(dot(viewDir,reflectDir),0.0),material.shininess);
    vec3 specular = material.specular * spec * light.specular;

    vec3 result = (ambient + diffuse + specular);
    FragColor = vec4(result,1.0);
}