#include "Marble.h"

#include "Mesh.h"

Marble::Marble(Mesh* const mesh_) noexcept
	: mesh(mesh_)
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