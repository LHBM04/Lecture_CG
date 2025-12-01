#include "Object.h"

#include "Input.h"
#include "Mesh.h"

Object::Object(Mesh* const mesh_) noexcept
	: mesh(mesh_)
{
}

void Object::Update(const float deltaTime_) noexcept
{

}

void Object::Render(const Shader& shader_) const noexcept
{
	if (!mesh)
	{
		return;
	}

	shader_.SetUniformMatrix4x4("model", GetModelMatrix());
	mesh->Render();
}