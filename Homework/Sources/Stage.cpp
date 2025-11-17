#include "Stage.h"

Stage::Stage(const std::size_t& width_, const std::size_t& height_, Mesh* const		  mesh_) noexcept
	: mesh(mesh_)
{
	SetScale(glm::vec3(static_cast<float>(width_), 1.0f, static_cast<float>(height_)));
}

Stage::~Stage() noexcept
{
}

void Stage::Update(const float deltaTime_) noexcept
{
	// 스테이지는 특별한 업데이트가 필요하지 않음
}

void Stage::Render(const Shader& shader_) const noexcept
{
	if (!mesh)
	{
		return;
	}

	shader_.SetUniformMatrix4x4("uModel", GetModelMatrix());
	mesh->Render(GL_TRIANGLES);
}