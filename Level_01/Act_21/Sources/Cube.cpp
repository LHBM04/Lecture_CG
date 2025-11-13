#include "Cube.h"

Cube::Cube(
	const glm::vec3& position_,
	const glm::vec3& rotation_,
	const glm::vec3& scale_
) noexcept
	: Object(position_, rotation_, scale_)
{
	SetMesh(Mesh::LoadFrom("Resources/Models/Cube.obj"));
}

Cube::~Cube() noexcept
{
}

void Cube::Update(float deltaTime_) noexcept
{
	glm::vec3 position = GetPosition();
	SetPosition(position + glm::vec3(0.1f, -1.0f, 0.1f) * deltaTime_);

	
}