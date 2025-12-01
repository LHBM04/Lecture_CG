#version 330 core

in vec3 vNormal;   // [추가]
in vec3 vFragPos;  // [추가]
in vec2 vTexCoord;

out vec4 FragColor;

uniform sampler2D uTexture;
uniform vec3 uLightPos;    // [추가] 조명 위치
uniform vec3 uLightColor;  // [추가] 조명 색상
uniform vec3 uViewPos;     // [추가] 카메라 위치

void main()
{
    vec4 texColor = texture(uTexture, vTexCoord);

    // 1. Ambient (주변광 - 기본 밝기)
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * uLightColor;

    // 2. Diffuse (확산광 - 빛의 각도에 따른 명암)
    vec3 norm = normalize(vNormal);
    vec3 lightDir = normalize(uLightPos - vFragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * uLightColor;

    // 3. Specular (반사광 - 하이라이트)
    float specularStrength = 0.5;
    vec3 viewDir = normalize(uViewPos - vFragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * uLightColor;

    // 최종 색상 = (조명 합) * 텍스쳐 색상
    vec3 result = (ambient + diffuse + specular) * vec3(texColor);

    FragColor = vec4(result, texColor.a);
}