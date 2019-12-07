#version 330 core
out vec4 FragColor;

struct Material {
    //vec3 ambient;   // 环境光照
    sampler2D diffuse;   // 漫反射光照
    sampler2D specular;  // 镜面光照
    float shininess;// 镜面高光的散射
};


struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};


in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

uniform Material material;
uniform Light light;
uniform vec3 viewPos;

void main() {

    // 环境光
    vec3 ambient = light.ambient * vec3(texture(material.diffuse,TexCoords));

    // 漫反射
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm,lightDir),0.0f);
    vec3 diffuse = (diff * light.diffuse) * vec3(texture(material.diffuse,TexCoords));

    //float specularStrength = 0.5f;

    // 镜面光
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir,norm);
    float spec = pow(max(dot(viewDir,reflectDir),0.0),material.shininess);
    vec3 specular = vec3(texture(material.diffuse,TexCoords)) * spec * light.specular;

    vec3 result = (ambient + diffuse + specular);
    FragColor = vec4(result,1.0);
}
