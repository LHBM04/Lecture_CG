#include "Object.h"

#include "Input.h"

#include "Mesh.h"
#include "Texture.h"

Object::Object(Mesh* mesh_, Texture* texture_) noexcept
    : mesh(mesh_)
    , texture(texture_)
{

}


void Object::Update(const float deltaTime_) noexcept
{
    if (Input::IsKeyHeld(GLFW_KEY_X))
    {
        const float rotateSpeed = 30.0f * (Input::IsModified(GLFW_MOD_SHIFT) ? -1.0f : 1.0f);
        SetRotation(GetRotation() + glm::vec3(rotateSpeed * deltaTime_, 0.0f, 0.0f));
    }

    if (Input::IsKeyHeld(GLFW_KEY_Y))
    {
        const float rotateSpeed = 30.0f * (Input::IsModified(GLFW_MOD_SHIFT) ? -1.0f : 1.0f);
        SetRotation(GetRotation() + glm::vec3(0.0f, rotateSpeed * deltaTime_, 0.0f));
    }
}

void Object::Render(const Shader& shader_) const noexcept
{
    shader_.SetUniformMatrix4x4("uModel", GetModelMatrix());

    // 2. 텍스쳐가 있다면 바인딩
    if (texture)
    {
        texture->Bind(0); // 0번 슬롯에 바인딩
        // 셰이더의 샘플러(uTexture)에게 0번 슬롯을 사용하라고 알림 (Texture 셰이더일 경우)
        // Standard 셰이더는 이 유니폼이 없으므로 무시됨(또는 위치 -1 반환)
        shader_.SetUniformInt("uTexture", 0);
    }

    // 3. 메쉬 그리기
    mesh->Render();

    // 4. 언바인딩 (선택사항, 안전을 위해)
    if (texture)
    {
        texture->Unbind();
    }
}