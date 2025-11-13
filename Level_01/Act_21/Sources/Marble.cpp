#include "Marble.h"

Marble::Marble(
	const float      boundary_,
	const glm::vec3& position_,
	const glm::vec3& rotation_,
	const glm::vec3& scale_
) noexcept
	: Object(position_, rotation_, scale_)
	, boundary(boundary_)
	, direction(glm::vec3(1.0f, 0.f, 0.0f))
{
	SetMesh(Mesh::LoadFrom("Resources/Models/Sphere.obj"));
}

Marble::~Marble() noexcept
{
}

void Marble::Update(const float deltaTime_) noexcept
{
	glm::vec3 position = GetPosition();
	position += direction * (MOVE_SPEED * deltaTime_);
	
	if (position.x > boundary)
	{
		position.x  = boundary;             
		direction.x = -direction.x;
	}
	else if (position.x < -boundary)
	{
		position.x  = -boundary;
		direction.x = -direction.x;
	}

	if (position.y > boundary)
	{
		position.y  = boundary;
		direction.y = -direction.y;
	}
	else if (position.y < -boundary)
	{
		position.y  = -boundary;
		direction.y = -direction.y;
	}

	SetPosition(position);
}