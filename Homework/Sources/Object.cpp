#include "Object.h"

#include "Mesh.h"
#include "Shader.h"

Object::Object(const Transform& transform_, Mesh* const mesh_) noexcept
	: transform(transform_)
	, mesh(mesh_)
{
}

void Object::OnUpdate(const float deltaTime_) noexcept
{
	// 기본 구현은 아무 것도 하지 않음
}

void Object::OnRender() const noexcept
{
	// 기본 구현은 아무 것도 하지 않음
}

void Object::Update(const float deltaTime_) noexcept
{
	// 일관성을 위함.
	// 뭔가 추가되면 더 적을 수 있겠지만...
	OnUpdate(deltaTime_);
}

void Object::Render() const noexcept
{
	if (!mesh)
	{
		return;
	}

	Shader::SetUniformMatrix4x4("uModel", transform.GetModel());
	mesh->Render(GL_TRIANGLES);

	OnRender();
}