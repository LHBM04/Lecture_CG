#include "PCH.h"

#include "Application.h"
#include "Input.h"

#include "Shader.h"
#include "Camera.h"

#include "Mesh.h"

#include "Object.h"
#include "Block.h"
#include "Marble.h"
#include "Stage.h"

/**
 * @brief 창이 켜질 때 호출됩니다.
 */
static void OnStart() noexcept;

/**
 * @brief 매 프레임마다 호출됩니다.
 * 
 * @param deltaTime_ 이전 프레임과의 시간 차이(초)
 */
static void OnUpdate(const float deltaTime_) noexcept;

/**
 * @brief 창이 그려질 때 호출됩니다.
 */
static void OnRender() noexcept;

/**
 * @brief 창이 닫힐 때 호출됩니다.
 */
static void OnClose() noexcept;

/**
 * @brief 메인 카메라.
 */
static std::unique_ptr<Camera> camera;

/**
 * @brief 셰이더.
 */
static std::unique_ptr<Shader> shader;

/**
 * @brief 무대 오브젝트의 메쉬.
 */
static std::unique_ptr<Mesh> stageMesh;

/**
 * @brief 구슬 오브젝트의 메쉬.
 */
static std::unique_ptr<Mesh> marbleMesh;

/**
 * @brief 블록 오브젝트의 메쉬.
 */
static std::unique_ptr<Mesh> blockMesh;

/**
 * @brief 장면 내 오브젝트들.
 */
static std::vector<std::unique_ptr<Object>> objects;

int main()
{
	std::print("※ [z/Z]: Z축으로 양/음의 방향으로 이동\n");
	std::print("※ [y/Y]: 화면의 Y축에 대하여 시계, 반시계 방향으로 회전\n");
	std::print("※ [B]:   박스 내 새로운 공을 추가\n");
	std::print("※ [Q]:   프로그램 종료\n");
	std::print("※ 마우스를 좌/우로 이동하면 박스가 Z축에 대하여 회전합니다\n");

	Application::Configuration configuration = { };
	configuration.width				= 800;
	configuration.height			= 600;
	configuration.title				= "Level 01 - Act 21";
	configuration.shouldFullscreen	= false;
	configuration.shouldDecorate	= true;
	configuration.shouldResizable	= false;
	configuration.shouldVSync		= false;
	configuration.onStart			= OnStart;
	configuration.onUpdate			= OnUpdate;
	configuration.onRender			= OnRender;
	configuration.onClose			= OnClose;
	
	return Application::Run(configuration);
}

void OnStart() noexcept
{
	camera = std::make_unique<Camera>();
	camera->SetProjection(Camera::Projection::Perspective);
	camera->SetPosition(glm::vec3(0.0f, 0.0f, 25.0f));
	camera->SetForward(glm::vec3(0.0f) - camera->GetPosition());
	camera->SetUp(glm::vec3(0.0f, 1.0f, 0.0f));
	camera->SetViewport(Camera::Viewport{ 0, 0, 800, 600 });

	shader = std::make_unique<Shader>();
	shader->Use();

	stageMesh = Mesh::LoadFrom("Resources/Meshes/Stage.obj");
	if (!stageMesh)
	{
		spdlog::critical("메쉬 로드에 실패했습니다: Resources/Meshes/Stage.obj");
		return;
	}

	Object* stage = objects.emplace_back(std::make_unique<Stage>(stageMesh.get())).get();
	stage->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	stage->SetScale(glm::vec3(10.0f));

	marbleMesh = Mesh::LoadFrom("Resources/Meshes/Marble.obj");
	if (!marbleMesh)
	{
		spdlog::critical("메쉬 로드에 실패했습니다: Resources/Meshes/Marble.obj");
		return;
	}

	blockMesh = Mesh::LoadFrom("Resources/Meshes/Block.obj");
	if (!blockMesh)
	{
		spdlog::critical("메쉬 로드에 실패했습니다: Resources/Meshes/Block.obj");
		return;
	}

	Object* block1 = objects.emplace_back(std::make_unique<Block>(blockMesh.get())).get();
	block1->SetPosition(glm::vec3(-5.0f, 0.0f, 0.0f));
}

void OnUpdate(const float deltaTime_) noexcept
{
	if (Input::IsKeyPressed(GLFW_KEY_Z))
	{
		if (Input::IsModified(GLFW_MOD_SHIFT))
		{
			const glm::vec3 position = camera->GetPosition();
			const glm::vec3 forward  = glm::normalize(camera->GetForward());
			camera->SetPosition(position + forward * 4.0f * deltaTime_);
		}
		else
		{
			const glm::vec3 position = camera->GetPosition();
			const glm::vec3 forward = glm::normalize(camera->GetForward());
			camera->SetPosition(position - forward * 4.0f * deltaTime_);
		}
	}

	if (Input::IsKeyPressed(GLFW_KEY_B))
	{
		objects.emplace_back(std::make_unique<Marble>(marbleMesh.get()));
	}

	if (Input::IsKeyPressed(GLFW_KEY_Q))
	{
		Application::Quit(0);
	}

	for (const std::unique_ptr<Object>& object : objects)
	{
		if (object)
		{
			object->Update(deltaTime_);
		}
	}
}

void OnRender() noexcept
{
	if (!shader)
	{
		return;
	}

	if (camera)
	{
		camera->PreRender(*shader);

		for (const std::unique_ptr<Object>& object : objects)
		{
			if (object)
			{
				object->Render(*shader);
			}
		}
	}
}

void OnClose() noexcept
{
	camera.reset();
	shader.reset();

	stageMesh.reset();
	marbleMesh.reset();
	blockMesh.reset();

	objects.clear();
}