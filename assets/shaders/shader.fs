#version 330 core
out vec4 FragColor;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;    
    float shininess;
}; 

struct Light {
    float constant;
    float linear;
    float quadratic;

    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 FragPos;  
in vec3 Normal;  

#define MAX_LIGHTS 100

uniform int numLights;
uniform vec3 viewPos;
uniform Material material;
uniform Light light[MAX_LIGHTS];

vec3 calcPointLights(Light light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main() {
    
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    vec3 result = vec3(0,0,0);
    for (int i = 0; i < numLights; i++){
        result += calcPointLights(light[i], norm, FragPos, viewDir);
    }

    FragColor = vec4(result, 1.0);
} 

vec3 calcPointLights(Light light, vec3 normal, vec3 fragPos, vec3 viewDir){
    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    
    vec3 reflectDir = reflect(-lightDir, normal);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    vec3 ambient = light.ambient * material.ambient;
    vec3 diffuse = light.diffuse * (diff * material.diffuse);
    vec3 specular = light.specular * (spec * material.specular);

    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    ambient *= attenuation;  
    diffuse *= attenuation;
    specular *= attenuation;

    return (ambient + diffuse + specular);
}
