#include "Cube.h"

#include "../Core/Input.h"
#include "../Core/Time.h"

#include "../Rendering/Cylinder.h"
#include "../Rendering/Sphere.h"

Cube::Cube() noexcept
    : Object{}
{
    Sphere* model = new Sphere(1.0f, 4, 2);
    SetModel(model);
}

void Cube::Update() noexcept
{
    if (Input::IsKeyHeld('W'))
    {
        const glm::vec3 rotation = GetRotation();
        SetRotation({rotation.x + 90.0f * Time::GetDeltaTime(), rotation.y, rotation.z});
    }
    if (Input::IsKeyHeld('A'))
    {
        const glm::vec3 rotation = GetRotation();
        SetRotation({rotation.x, rotation.y + 90.0f * Time::GetDeltaTime(), rotation.z});
    }
    if (Input::IsKeyHeld('S'))
    {
        const glm::vec3 rotation = GetRotation();
        SetRotation({rotation.x - 90.0f * Time::GetDeltaTime(), rotation.y, rotation.z});
    }
    if (Input::IsKeyHeld('D'))
    {
        const glm::vec3 rotation = GetRotation();
        SetRotation({rotation.x, rotation.y - 90.0f * Time::GetDeltaTime(), rotation.z});
    }

    if (Input::IsKeyHeld('+'))
    {
        const glm::vec3 scale = GetScale();
        SetScale(scale + glm::vec3{1.0f, 1.0f, 1.0f} * Time::GetDeltaTime());
    }
    if (Input::IsKeyHeld('-'))
    {
        const glm::vec3 scale = GetScale();
        SetScale(scale - glm::vec3{1.0f, 1.0f, 1.0f} * Time::GetDeltaTime());
    }
}

void Cube::Render(const Shader& shader_) const noexcept
{
    shader_.SetUniformVector3("uColor", {0.5f, 1.0f, 0.0f});
    Object::Render(shader_);
}