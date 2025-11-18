#include "Stage.h"

#include "Input.h"

#include "Mesh.h"

Stage::Stage(Mesh* const mesh_) noexcept
	: mesh(mesh_)
	, isFloorOpen(false)
{
}

Stage::~Stage() noexcept
{

}

void Stage::Update(const float deltaTime_) noexcept
{
	RotateSelf(deltaTime_);

	if (Input::IsMouseButtonPressed(GLFW_MOUSE_BUTTON_RIGHT))
	{
		OpenFloor();
	}
}

void Stage::Render(const Shader& shader_) const noexcept
{
	if (mesh)
	{
		shader_.SetUniformMatrix4x4("uModel", GetModelMatrix());
		mesh->Render();
	}
}

void Stage::RotateSelf(const float deltaTime_) noexcept
{
	const glm::vec2 mouseDelta = Input::GetMouseDelta();
	glm::vec3 rotation = GetRotation();
	rotation.z += mouseDelta.x * 0.2f;
	SetRotation(rotation);
}

void Stage::OpenFloor() noexcept
{
	if (!mesh)
	{
		return;
	}

	isFloorOpen = !isFloorOpen;

	constexpr float floorOpenYOffset = -2.0f;

	auto vertices = mesh->GetVertices();
	vertices[floorVertexIndices[0]].position.y += floorOpenYOffset;
	vertices[floorVertexIndices[1]].position.y += floorOpenYOffset;
	vertices[floorVertexIndices[2]].position.y += floorOpenYOffset;
	vertices[floorVertexIndices[3]].position.y += floorOpenYOffset;
	mesh->SetVertices(vertices);

	mesh->UpdateVertices();
}