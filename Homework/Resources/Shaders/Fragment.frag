#version 450 core

in vec3 vFragPos;
in vec3 vNormal;
in vec3 vColor;

out vec4 FragColor;

struct Light 
{
    vec3 position;
    vec3 color;
    float ambient;
    float specular;
};

uniform Light uLight;
uniform vec3  uViewPos; 

void main()
{
    vec3 ambient = uLight.ambient * uLight.color;
  
    vec3 norm = normalize(vNormal);
    vec3 lightDir = normalize(uLight.position - vFragPos);
    
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * uLight.color;
    
    vec3 viewDir = normalize(uViewPos - vFragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = uLight.specular * spec * uLight.color;  
    
    vec3 result = (ambient + diffuse + specular) * vColor;
    FragColor = vec4(result, 1.0);
}