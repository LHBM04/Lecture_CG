#include "Object.h"

Object::Object() noexcept
    : model(gluNewQuadric())
{
    gluQuadricDrawStyle(model, GLU_FILL);
    gluQuadricNormals(model, GLU_SMOOTH);
    gluQuadricOrientation(model, GLU_OUTSIDE);
}

Object::~Object() noexcept
{
    if (model != nullptr)
    {
        gluDeleteQuadric(model);
    }
}

void Object::Update(const float deltaTime_) noexcept
{
    if (model == nullptr)
    {
        return;
    }
}

void Object::Render(const Shader& shader_) const noexcept
{
    if (model == nullptr)
    {
        return;
    }

    glm::mat4 modelMatrix = {1.0f};
    modelMatrix = glm::translate(modelMatrix, position);
    modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
    modelMatrix = glm::scale(modelMatrix, scale);

    shader_.SetUniformMatrix4x4("uModel", modelMatrix);

    gluSphere(model, 1.0f, 30, 30);
}