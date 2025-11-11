#include "Planet.h"

#include "../Core/Time.h"
#include "Object.h"

Planet::Planet(
    Object* const parent_,
    const float   distance_,
    const float   angle_,
    const float   speed_,
    const float   tilt_    
) noexcept
	: mParent(parent_)
    , mDistance(distance_)
    , mAngle(angle_)
    , mSpeed(speed_)
	, mTilt(tilt_)
{
    SetMesh(Mesh::LoadFrom("Resources/Models/Sphere.obj"));
}

void Planet::Update() noexcept
{
    if (mParent == nullptr)
    {
        return;
    }

    mAngle += mSpeed * Time::GetDeltaTime();
    const float angle = glm::radians(mAngle - 90.0f);

    const float orbitX = mDistance * cos(angle);
    const float orbitZ = mDistance * sin(angle);

    const float tiltAngle = glm::radians(mTilt - 90.0f);

    glm::vec3 offset = { 0.0f, 0.0f, 0.0f };

    offset.x = orbitX * cos(tiltAngle);
    offset.y = orbitX * sin(tiltAngle);
    offset.z = orbitZ;

    SetPosition(mParent->GetPosition() + offset);
}