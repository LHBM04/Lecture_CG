#include "Marble.h"

#include "Mesh.h"
#include "CollisionBox.h"

Marble::Marble(Mesh* const mesh_) noexcept
	: mesh(mesh_)
	, collisionBox(std::make_unique<CollisionBox>(glm::vec3(-0.5f), glm::vec3(0.5f))
{
	direction = glm::vec3(1.0f, 0.0f, 0.0f);
}

Marble::~Marble() noexcept
{
}

void Marble::Update(const float deltaTime_) noexcept
{
	const glm::vec3 current = GetPosition();
	const glm::vec3 next	= current + direction * (moveSpeed * deltaTime_);

	// 충돌 검사
	// 박스의 면과 닿으면 반대 방향으로 튕겨나오도록

	SetPosition(next);
}

void Marble::Render(const Shader& shader_) const noexcept
{
	if (mesh)
	{
		shader_.SetUniformMatrix4x4("uModel", GetModelMatrix());
		mesh->Render();
	}
}