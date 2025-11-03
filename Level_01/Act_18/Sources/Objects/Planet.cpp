#include "Planet.h"

#include "../Core/Time.h"
#include "../Core/Input.h"

Planet::Planet() noexcept
    : isSelected{false}
    , translateFactor{50.0f}
    , rotateFactor{15.0f}
    , scaleFactor{10.0f}
{

}

void Planet::Update() noexcept
{
    if (!isSelected)
    {
        return;
    }

    Translate();
    Rotate();
    Scale();
}

void Planet::Translate() noexcept
{
    if (Input::IsKeyPressed('d'))
    {
        const glm::vec3 original  = GetPosition();
        const glm::vec3 translate = original + ((glm::vec3{1.0f, 0.0f, 0.0f} * translateFactor) * Time::GetDeltaTime());
        Object::SetPosition(translate);
    }
    else if (Input::IsKeyPressed('D'))
    {
        const glm::vec3 original  = GetPosition();
        const glm::vec3 translate = original + ((glm::vec3{-1.0f, 0.0f, 0.0f} * translateFactor) * Time::GetDeltaTime());
        Object::SetPosition(translate);
    }

    if (Input::IsKeyPressed('e'))
    {
        const glm::vec3 original  = GetPosition();
        const glm::vec3 translate = original + ((glm::vec3{0.0f, 1.0f, 0.0f} * translateFactor) * Time::GetDeltaTime());
        Object::SetPosition(translate);
    }
    else if (Input::IsKeyPressed('E'))
    {
        const glm::vec3 original  = GetPosition();
        const glm::vec3 translate = original + ((glm::vec3{0.0f, -1.0f, 0.0f} * translateFactor) * Time::GetDeltaTime());
        Object::SetPosition(translate);
    }
}

void Planet::Rotate() noexcept
{
    if (Input::IsKeyHeld('x'))
    {
        const glm::vec3 original = GetRotation();
        const glm::vec3 rotate   = original + ((glm::vec3{1.0f, 0.0f, 0.0f} * rotateFactor) * Time::GetDeltaTime());

        Object::SetRotation(rotate);
    }
    else if (Input::IsKeyHeld('X'))
    {
        const glm::vec3 original = GetRotation();
        const glm::vec3 rotate   = original + ((glm::vec3{-1.0f, 0.0f, 0.0f} * rotateFactor) * Time::GetDeltaTime());

        Object::SetRotation(rotate);
    }

    if (Input::IsKeyHeld('y'))
    {
        const glm::vec3 original = GetRotation();
        const glm::vec3 rotate   = original + ((glm::vec3{0.0f, 1.0f, 0.0f} * rotateFactor) * Time::GetDeltaTime());

        Object::SetRotation(rotate);
    }
    else if (Input::IsKeyHeld('Y'))
    {
        const glm::vec3 original = GetRotation();
        const glm::vec3 rotate   = original + ((glm::vec3{0.0f, -1.0f, 0.0f} * rotateFactor) * Time::GetDeltaTime());

        Object::SetRotation(rotate);
    }

    if (Input::IsKeyHeld('z'))
    {
        const glm::vec3 original = GetRotation();
        const glm::vec3 rotate   = original + ((glm::vec3{0.0f, 0.0f, 1.0f} * rotateFactor) * Time::GetDeltaTime());

        Object::SetRotation(rotate);
    }
    else if (Input::IsKeyHeld('Z'))
    {
        const glm::vec3 original = GetRotation();
        const glm::vec3 rotate   = original + ((glm::vec3{0.0f, 0.0f, -1.0f} * rotateFactor) * Time::GetDeltaTime());

        Object::SetRotation(rotate);
    }

    if (Input::IsKeyHeld('r'))
    {
        const glm::vec3 currentPos   = GetPosition();
        const float     radius       = glm::length(glm::vec2(currentPos.x, currentPos.z));
        const float     currentAngle = atan2(currentPos.z, currentPos.x);

        const float angleDelta = glm::radians(translateFactor) * Time::GetDeltaTime();
        const float newAngle   = currentAngle - angleDelta;

        glm::vec3 nextPosition = currentPos;
        nextPosition.x = radius * cos(newAngle);
        nextPosition.z = radius * sin(newAngle);

        Object::SetPosition(nextPosition);
    }
    else if (Input::IsKeyHeld('R'))
    {
        const glm::vec3 currentPos   = GetPosition();
        const float     radius       = glm::length(glm::vec2(currentPos.x, currentPos.z));
        const float     currentAngle = atan2(currentPos.z, currentPos.x);

        const float angleDelta = glm::radians(translateFactor) * Time::GetDeltaTime();
        const float newAngle   = currentAngle + angleDelta;

        glm::vec3 nextPosition = currentPos;
        nextPosition.x = radius * cos(newAngle);
        nextPosition.z = radius * sin(newAngle);

        Object::SetPosition(nextPosition);
    }
}

void Planet::Scale() noexcept
{
    if (Input::IsKeyHeld('a'))
    {
        const glm::vec3 currentScale = GetScale();
        const glm::vec3 scaleAmount  = glm::vec3{1.0f} * scaleFactor * Time::GetDeltaTime();
        Object::SetScale(currentScale + scaleAmount);
    }
    else if (Input::IsKeyHeld('A'))
    {
        const glm::vec3 currentScale = GetScale();
        const glm::vec3 scaleAmount  = glm::vec3{1.0f} * scaleFactor * Time::GetDeltaTime();
        const glm::vec3 nextScale    = currentScale - scaleAmount;
        Object::SetScale(glm::max(nextScale, glm::vec3{0.1f}));
    }

    if (Input::IsKeyHeld('b'))
    {
        const glm::vec3 currentPos   = GetPosition();
        const glm::vec3 currentScale = GetScale();
        const float     scaleAmount  = scaleFactor * Time::GetDeltaTime();

        glm::vec3 nextPosition = currentPos + (currentPos * scaleAmount);
        Object::SetPosition(nextPosition);

        glm::vec3 nextScale = currentScale + (currentScale * scaleAmount);
        Object::SetScale(nextScale);
    }
    else if (Input::IsKeyHeld('B'))
    {
        const glm::vec3 currentPos   = GetPosition();
        const glm::vec3 currentScale = GetScale();
        const float     scaleAmount  = scaleFactor * Time::GetDeltaTime();

        glm::vec3 nextPosition = currentPos - (currentPos * scaleAmount);
        Object::SetPosition(nextPosition);

        glm::vec3 nextScale = currentScale - (currentScale * scaleAmount);
        Object::SetScale(glm::max(nextScale,  glm::vec3{0.1f}));
    }
}