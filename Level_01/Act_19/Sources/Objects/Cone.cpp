#include "Cone.h"

#include "../Core/Input.h"
#include "../Core/Time.h"

#include "../Rendering/Cylinder.h"

Cone::Cone() noexcept
    : Object{}
{
    Cylinder* model = new Cylinder(1.0f, 0.0f, 1.5f, 32, 4);
    SetModel(model);
}

void Cone::Update() noexcept
{
    if (Input::IsKeyHeld('w'))
    {
        const glm::vec3 rotation = GetRotation();
        SetRotation({rotation.x + 90.0f * Time::GetDeltaTime(), rotation.y, rotation.z});
    }
    if (Input::IsKeyHeld('a'))
    {
        const glm::vec3 rotation = GetRotation();
        SetRotation({rotation.x, rotation.y + 90.0f * Time::GetDeltaTime(), rotation.z});
    }
    if (Input::IsKeyHeld('s'))
    {
        const glm::vec3 rotation = GetRotation();
        SetRotation({rotation.x - 90.0f * Time::GetDeltaTime(), rotation.y, rotation.z});
    }
    if (Input::IsKeyHeld('d'))
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

void Cone::Render(const Shader& shader_) const noexcept
{
    shader_.SetUniformVector3("uColor", {1.0f, 0.5f, 0.0f});
    Object::Render(shader_);
}