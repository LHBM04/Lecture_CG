#include "Box.h"

#include "Input.h"

Box::Box(
	const glm::vec3& position_,
	const glm::vec3& rotation_,
	const glm::vec3& scale_
) noexcept
	: Object(position_, rotation_, scale_)
{
	SetMesh(Mesh::LoadFrom("Resources/Models/Box.obj"));
}

Box::~Box() noexcept
{
}

void Box::Update(const float deltaTime_) noexcept
{
	glm::vec2 mouseDelta = Input::GetMouseDelta();

	static float angleZ = 0.0f;
	angleZ += mouseDelta.x * 0.2f;

	SetRotation(glm::vec3(0.0f, 0.0f, angleZ));
}