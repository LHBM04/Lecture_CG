#include "Object.h"

Object::Object(
    const glm::vec3& position_,
    const glm::vec3& rotation_,
    const glm::vec3& scale_
) noexcept
    : mesh(nullptr)
	, transform{ position_, rotation_, scale_, nullptr }
{

}

Object::~Object() noexcept
{
    if (mesh)
    {
        mesh.reset();
    }
}

void Object::Update(const float deltaTime_) noexcept
{
   
}

void Object::Render(const Shader& shader_) const noexcept
{
    if (mesh != nullptr)
    {
        glm::mat4 modelMatrix = transform.GetModelMatrix();
        shader_.SetUniformMatrix4x4("uModel", modelMatrix);

        mesh->Render();
    }
}