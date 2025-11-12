#include "Box.h"

Box::Box(
	const glm::vec3& position_,
	const glm::vec3& rotation_,
	const glm::vec3& scale_
) noexcept
	: Object(position_, rotation_, scale_)
{
	SetMesh(Mesh::LoadFrom("Resources/Models/Box.obj"));
	for (auto& marble : marbles)
	{
		marble = std::make_unique<Object>();
		marble->SetMesh(Mesh::LoadFrom("Resources/Models/Marble.obj"));
		marble->SetParent(this);
	}
}

Box::~Box() noexcept
{
	for (auto& marble : marbles)
	{
		marble.reset();
	}
}

void Box::Update(const float deltaTime_) noexcept
{
	constexpr float rotationSpeed = 90.0f;

	for (size_t i = 0; i < marbles.size(); ++i)
	{
		float angle = static_cast<float>(glfwGetTime()) * rotationSpeed + (i * 60.0f);
		float radius = 0.5f;
		float x = radius * glm::cos(glm::radians(angle));
		float z = radius * glm::sin(glm::radians(angle));
		marbles[i]->SetPosition(glm::vec3(x, 0.0f, z));
	}
}

void Box::Render(
	const Shader& shader_,
	const GLenum  renderMode_
) const noexcept
{
	Object::Render(shader_, renderMode_);

	for (const auto& marble : marbles)
	{
		marble->Render(shader_, renderMode_);
	}
}