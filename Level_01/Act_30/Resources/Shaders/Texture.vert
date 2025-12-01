#version 330 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;

out vec3 vNormal;   // [추가]
out vec3 vFragPos;  // [추가]
out vec2 vTexCoord;

void main()
{
    vTexCoord = aTexCoord;

    // [추가] 월드 공간 상의 정점 위치 계산
    vFragPos = vec3(uModel * vec4(aPosition, 1.0));

    // [추가] 법선 벡터 변환 (회전/크기 적용)
    // 역행렬 계산은 비용이 크므로, 추후엔 CPU에서 NormalMatrix를 넘겨주는 것이 좋습니다.
    vNormal = mat3(transpose(inverse(uModel))) * aNormal;

    gl_Position = uProjection * uView * uModel * vec4(aPosition, 1.0);
}