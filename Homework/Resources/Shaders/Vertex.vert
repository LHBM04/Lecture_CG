#version 450 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aColor;
layout(location = 2) in vec3 aNormal;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;

out vec3 vFragPos; // [추가] 월드 좌표계 상의 위치
out vec3 vNormal;  // [추가] 변환된 법선 벡터
out vec3 vColor;

void main()
{
    vColor = aColor;
    
    vFragPos = vec3(uModel * vec4(aPosition, 1.0));
    vNormal = mat3(transpose(inverse(uModel))) * aNormal;

    gl_Position = uProjection * uView * vec4(vFragPos, 1.0);
}