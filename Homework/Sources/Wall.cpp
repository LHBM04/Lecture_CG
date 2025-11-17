#include "Wall.h"

Wall::Wall(const float reach_, Mesh* const mesh_) noexcept
	: mesh(mesh_)
	, height(static_cast<float>(reach_))
{

}

Wall::~Wall() noexcept
{
}

void Wall::Update(const float deltaTime_) noexcept
{
	const float currentYScale = GetScale().y;

	if (currentYScale < height)
	{
		float newYScale = currentYScale + deltaTime_ * 5.0f;
		if (newYScale > height)
		{
			newYScale = height;
		}

		const glm::vec3 currentPos = GetPosition();
		const float newYPos = 0.501f + (newYScale * 0.5f);

		SetScale(glm::vec3(1.0f, newYScale, 1.0f));
		SetPosition(glm::vec3(currentPos.x, newYPos, currentPos.z));
	}
	else if (currentYScale != height)
	{
		const glm::vec3 currentPos = GetPosition();
		const float finalYPos = 0.501f + (height * 0.5f);

		SetScale(glm::vec3(1.0f, height, 1.0f));
		SetPosition(glm::vec3(currentPos.x, finalYPos, currentPos.z));
	}
}

void Wall::Render(const Shader& shader_) const noexcept
{
	if (!mesh)
	{
		return;
	}

	shader_.SetUniformMatrix4x4("uModel", GetModelMatrix());
	mesh->Render(GL_TRIANGLES);
}