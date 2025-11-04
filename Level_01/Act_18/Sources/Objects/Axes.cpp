#include "Axes.h"

#include <gl/glew.h>

Axes::Axes() noexcept
    : Object{}
{

}

void Axes::Render(const Shader& shader_) const noexcept
{
    const Mesh* const mesh = GetMesh();
    if (mesh == nullptr)
    {
        return;
    }

    glm::mat4 modelMatrix = {1.0f};
    modelMatrix = glm::translate(modelMatrix, GetPosition());
    modelMatrix = glm::rotate(modelMatrix, glm::radians(GetRotation().x), glm::vec3{1.0f, 0.0f, 0.0f});
    modelMatrix = glm::rotate(modelMatrix, glm::radians(GetRotation().y), glm::vec3{0.0f, 1.0f, 0.0f});
    modelMatrix = glm::rotate(modelMatrix, glm::radians(GetRotation().z), glm::vec3{0.0f, 0.0f, 1.0f});
    modelMatrix = glm::scale(modelMatrix, GetScale());
    shader_.SetUniformMatrix4x4("uModel", modelMatrix);

    mesh->Render(GL_LINES);
}