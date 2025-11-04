#include "Planet.h"

#include "../Core/Input.h"
#include "../Core/Time.h"

Planet::Planet(const Planet::Properties& properties_) noexcept
    : properties{properties_}
{
    SetMesh(Mesh::LoadFrom("Resources/Models/Sphere.obj"));
}

void Planet::Update() noexcept
{
    const Object* const parent = properties.parent;

    if (parent != nullptr)
    {
        const Object* const parent = properties.parent;

        if (parent != nullptr)
        {
            // 원인: properties.angle이 아닌 임시 변수 angle에 계산
            // const float angle   = properties.angle + properties.speed * Time::GetDeltaTime();

            // 해결: 1. properties.angle 값 자체에 각속도를 누적합니다.
            properties.angle += properties.speed * Time::GetDeltaTime();

            // 해결: 2. 누적된 각도를 사용합니다.
            const float radians = glm::radians(properties.angle);

            glm::vec3 offset = {0.0f, 0.0f, 0.0f};
            offset.x = properties.distance * cos(radians);
            offset.z = properties.distance * sin(radians);

            // 원인: 아래 코드는 이미 계산된 offset에 불필요한 회전을 이중으로 적용합니다.
            // const float     tiltRadians   = glm::radians(properties.angle);
            // const glm::mat4 rotation      = glm::rotate(glm::mat4(1.0f), tiltRadians, glm::vec3(0.0f, 1.0f, 0.0f));
            // const glm::vec3 rotatedOffset = glm::vec3(rotation * glm::vec4(offset, 1.0f));
            // SetPosition(parent->GetPosition() + rotatedOffset);

            // 해결: 3. 부모 위치에 계산된 offset만 더합니다.
            SetPosition(parent->GetPosition() + offset);
        }
    }
}