// Act_19/Sources/Objects/Planet.cpp

#include "Planet.h"

#include "../Core/Time.h" // Input.h는 여기서 필요 없습니다.

float Planet::s_OrbitTiltZ = 0.0f;

Planet::Planet(const Planet::Properties& properties_) noexcept
    : properties{properties_}
{
    SetMesh(Mesh::LoadFrom("Resources/Models/Sphere.obj"));
}

void Planet::Update() noexcept
{
    const Object* const parent = properties.parent;

    // 2. 부모가 있는 경우(태양이 아닌 경우)에만 공전 로직을 수행합니다.
    if (parent != nullptr)
    {
        // 공전 각도 누적
        properties.angle += properties.speed * Time::GetDeltaTime();
        const float orbitAngleRadians = glm::radians(properties.angle);

        // 3. X-Z 평면상의 2D 궤도 위치 계산
        const float x_orbit = properties.distance * cos(orbitAngleRadians);
        const float z_orbit = properties.distance * sin(orbitAngleRadians);

        // 4. static 변수 s_OrbitTiltZ를 이용해 Z축 궤도 기울기(회전) 적용
        const float tiltAngleRadians = glm::radians(s_OrbitTiltZ);

        glm::vec3 offset = {0.0f, 0.0f, 0.0f};

        // Z축 회전 행렬 적용:
        // x' = x * cos(T) - y * sin(T)
        // y' = x * sin(T) + y * cos(T)
        // z' = z
        // (여기서 2D 궤도는 y=0 이라고 가정합니다)
        offset.x = x_orbit * cos(tiltAngleRadians);
        offset.y = x_orbit * sin(tiltAngleRadians);
        offset.z = z_orbit;

        SetPosition(parent->GetPosition() + offset);
    }
}