#include "Marble.h"

#include "Mesh.h"

Marble::Marble(Mesh* const mesh_) noexcept
	: mesh(mesh_)
{
    velocity = glm::vec3(1.0f, 0.0f, 0.0f);
}

Marble::~Marble() noexcept
{
}

void Marble::Update(const float deltaTime_) noexcept
{
    if (deltaTime_ <= 0.0f)
        return;

    // 1) 현재 위치 적분 (월드)
    glm::vec3 pos = GetPosition();
    pos += velocity * deltaTime_;

    Object* const parent = GetParent();

    if (parent)
    {
        // 2) 부모(무대) 기준 값
        const glm::vec3 parentPos = parent->GetPosition();
        const glm::vec3 parentScale = parent->GetScale();
        const float     rotZDeg = parent->GetRotation().z;
        const float     rotZRad = glm::radians(rotZDeg);

        // 회전 행렬 / 역행렬 (Z축만)
        const float c = std::cos(rotZRad);
        const float s = std::sin(rotZRad);
        const glm::mat2 rotZ = glm::mat2(c, -s,
            s, c);
        const glm::mat2 invRotZ = glm::mat2(c, s,
            -s, c); // 정규직교이므로 transpose와 동일

        // 3) 월드 -> 부모 로컬 (Z회전만 고려)
        glm::vec3 relPos = pos - parentPos;
        glm::vec2 relPos2(relPos.x, relPos.y);
        relPos2 = invRotZ * relPos2;
        relPos.x = relPos2.x;
        relPos.y = relPos2.y;

        glm::vec2 vel2(velocity.x, velocity.y);
        vel2 = invRotZ * vel2;
        glm::vec3 localVel(vel2.x, vel2.y, velocity.z);

        // 4) 로컬 경계 계산
        glm::vec3 halfExtents = parentScale * 0.5f;

        auto collideAxis = [&](float& p, float& v, float limit)
            {
                const float maxPos = limit - radius;
                const float minPos = -limit + radius;

                if (p > maxPos)
                {
                    p = maxPos;
                    v = -v * restitution;
                }
                else if (p < minPos)
                {
                    p = minPos;
                    v = -v * restitution;
                }
            };

        collideAxis(relPos.x, localVel.x, halfExtents.x);
        collideAxis(relPos.y, localVel.y, halfExtents.y);
        // 필요하면 Z도 사용 (무대가 Z축 두께를 가진다면)
        // collideAxis(relPos.z, localVel.z, halfExtents.z);

        // 5) 로컬 -> 월드 복귀
        glm::vec2 newRelPos2(relPos.x, relPos.y);
        newRelPos2 = rotZ * newRelPos2;
        relPos.x = newRelPos2.x;
        relPos.y = newRelPos2.y;

        glm::vec2 newVel2(localVel.x, localVel.y);
        newVel2 = rotZ * newVel2;

        pos = parentPos + relPos;
        velocity = glm::vec3(newVel2.x, newVel2.y, localVel.z);
    }
    else
    {
        // 부모 없으면 단순 AABB 경계 (예: ±5)
        glm::vec3 halfExtents(5.0f);
        auto collideAxis = [&](float& p, float& v, float limit)
            {
                const float maxPos = limit - radius;
                const float minPos = -limit + radius;
                if (p > maxPos) { p = maxPos; v = -v * restitution; }
                else if (p < minPos) { p = minPos; v = -v * restitution; }
            };
        collideAxis(pos.x, velocity.x, halfExtents.x);
        collideAxis(pos.y, velocity.y, halfExtents.y);
    }

    // 6) 감쇠 및 멈춤 처리
    velocity *= damping;
    if (glm::length(velocity) < 0.01f)
        velocity = glm::vec3(0.0f);

    // 7) 최종 위치 적용
    SetPosition(pos);
}

void Marble::Render(const Shader& shader_) const noexcept
{
	if (mesh)
	{
		shader_.SetUniformMatrix4x4("uModel", GetModelMatrix());
		mesh->Render();
	}
}