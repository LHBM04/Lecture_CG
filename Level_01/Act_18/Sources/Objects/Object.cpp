#include "Object.h"

Object::Object(const glm::vec3& position_,
               const glm::vec3& rotation_,
               const glm::vec3& scale_) noexcept
    : mesh(nullptr)
    , position(position_)
    , rotation(rotation_)
    , scale(scale_)
{

}

Object::~Object() noexcept
{
    if (mesh)
    {
        mesh.reset();
    }
}

void Object::Update() noexcept
{

}

void Object::Render(const Shader& shader_) const noexcept
{
    if (mesh == nullptr)
    {
        return;
    }

    glm::mat4 modelMatrix = {1.0f};
    modelMatrix = glm::translate(modelMatrix, position);
    modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.x), glm::vec3{1.0f, 0.0f, 0.0f});
    modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.y), glm::vec3{0.0f, 1.0f, 0.0f});
    modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.z), glm::vec3{0.0f, 0.0f, 1.0f});
    modelMatrix = glm::scale(modelMatrix, scale);
    modelMatrix = glm::translate(modelMatrix, {-0.5f, -0.5f, -0.5f});
    shader_.SetUniformMatrix4x4("uModel", modelMatrix);

    mesh->Render();
}