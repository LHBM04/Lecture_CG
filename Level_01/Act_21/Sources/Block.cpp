#include "Block.h"

#include "Mesh.h"

Block::Block(Mesh* const mesh_) noexcept
	: mesh(mesh_)
{
	
}

Block::~Block() noexcept
{
}

void Block::Update(const float deltaTime_) noexcept
{
	// 블록은 움직이지 않음
}

void Block::Render(const Shader& shader_) const noexcept
{
	if (mesh)
	{
		shader_.SetUniformMatrix4x4("uModel", GetModelMatrix());
		mesh->Render();
	}
}