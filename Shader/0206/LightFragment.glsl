#version 330 core
out vec4 FragColor;

struct Material {
    //vec3 ambient;   // 环境光照
    sampler2D diffuse;   // 漫反射光照
    sampler2D specular;  // 镜面光照
    float shininess;// 镜面高光的散射
};


struct Light {

    vec3 position;      // 光源位置
    vec3 direction;     // 光线方向
    float cutOff;       // 切光角
    float outerCutOff;

    vec3 ambient;       // 环境光
    vec3 diffuse;       // 漫反射
    vec3 specular;      // 镜面光

    float constant;     // 光线衰减常数项
    float linear;       // 光线衰减线性项
    float quadratic;    // 光线衰减二次项
};


in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

uniform Material material;
uniform Light light;
uniform vec3 viewPos;

void main() {

    //vec3 lightDir = normalize(light.position - FragPos);


//    if(theta > light.cutOff)
  //  {
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
        vec3 specular = vec3(texture(material.specular,TexCoords)) * spec * light.specular;

    float theta =  dot(lightDir,normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff)/epsilon,0.0,1.0);
    diffuse *= intensity;
    specular *= intensity;

        float distance = length(light.position - FragPos);
        float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

        ambient *= attenuation;
        diffuse *= attenuation;
        specular *= attenuation;

        vec3 result = (ambient + diffuse + specular);
        FragColor = vec4(result,1.0);
  //  }
    //else
    //{
    //    FragColor = vec4(light.ambient * vec3(texture(material.specular,TexCoords)),1.0);
    //}


}
