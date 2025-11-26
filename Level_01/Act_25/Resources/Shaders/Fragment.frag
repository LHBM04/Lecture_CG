#version 330 core

in vec3 FragPos;
in vec3 Normal;

out vec4 FragColor;

uniform vec3 lightPos;     
uniform vec3 lightColor;   
uniform vec3 objectColor;
uniform vec3 viewPos;      

void main()
{
    // 1. Ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    // 2. Diffuse
    vec3 norm = normalize(Normal);
    vec3 lightVec = lightPos - vec3(FragPos); // 거리 계산을 위해 정규화 전 벡터 저장
    float distance = length(lightVec);        // [추가] 광원과 파편 사이의 거리 계산
    vec3 lightDir = normalize(lightVec);      // 방향 벡터 정규화

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // 3. Specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - vec3(FragPos));
    vec3 reflectDir = reflect(-lightDir, norm); 
    
    int shininess = 32;
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular = specularStrength * spec * lightColor;

    // [핵심] 4. Attenuation (감쇠) 적용
    // 공식: 1.0 / (Kc + Kl * d + Kq * d^2)
    // 거리 50 정도를 커버하는 표준 값 (Constant=1.0, Linear=0.09, Quadratic=0.032)
    float constant = 1.0;
    float linear = 0.09;
    float quadratic = 0.032;

    float attenuation = 1.0 / (constant + linear * distance + quadratic * (distance * distance));

    // 각 요소에 감쇠 적용
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;

    // 최종 결과
    vec3 result = (ambient + diffuse + specular) * objectColor;
    FragColor = vec4(result, 1.0);
}