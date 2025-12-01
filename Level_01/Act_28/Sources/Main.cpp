#include "PCH.h"

#include "Application.h"
#include "Camera.h"
#include "Input.h"
#include "Light.h"
#include "Mesh.h"
#include "Object.h"
#include "Shader.h"
#include "Tank.h"

/**
 * @brief 창이 켜질 때 호출됩니다.
 */
static void OnLoad() noexcept;

/**
 * @brief 매 프레임마다 호출됩니다.
 * 
 * @param deltaTime_ 이전 프레임과의 시간 차이(초)
 */
static void OnTick(const float deltaTime_) noexcept;

/**
 * @brief 창이 그려질 때 호출됩니다.
 */
static void OnDisplay() noexcept;

/**
 * @brief 창이 닫힐 때 호출됩니다.
 */
static void OnClose() noexcept;

/**
 * @brief 메인 카메라.
 */
static std::unique_ptr<Camera> camera;

/**
 * @brief 조명.
 */
static std::unique_ptr<Light> light;

/**
 * @brief 셰이더.
 */
static std::unique_ptr<Shader> shader;

/**
 * @brief 평면 메쉬.
 */
static std::unique_ptr<Mesh> planeMesh;

/**
 * @brief 시뮬레이션 할 오브젝트들.
 */
static std::vector<std::unique_ptr<Object>> objects;

int main()
{
	Application::Configuration configuration = { };
	configuration.width				= 800;
	configuration.height			= 600;
	configuration.title				= "Level 01 - Act 27";
	configuration.shouldFullscreen	= false;
	configuration.shouldDecorate	= true;
	configuration.shouldResizable	= false;
	configuration.shouldVSync		= false;
	configuration.onLoad			= OnLoad;
	configuration.onTick			= OnTick;
	configuration.onDisplay			= OnDisplay;
	configuration.onClose			= OnClose;
	
	return Application::Run(configuration);
}

void OnLoad() noexcept
{
	camera = std::make_unique<Camera>();
	camera->SetProjection(Camera::Projection::Perspective);
	camera->SetPosition(glm::vec3(0.0f, 50.0f, 50.0f));
	camera->SetForward(glm::vec3(0.0f, 0.0f, 0.0f) - glm::vec3(0.0f, 50.0f, 50.0f));
	camera->SetUp(glm::vec3(0.0f, 1.0f, 0.0f));
	camera->SetViewport(Camera::Viewport{ 0, 0, 800, 600 });
	 
	constexpr glm::vec3 lightPosition = glm::vec3(10.0f, 0.0f, 10.0f);
	constexpr glm::vec3 lightColor	  = glm::vec3(1.0f, 1.0f, 1.0f);
	light = std::make_unique<Light>(lightPosition, lightColor);

	shader = std::make_unique<Shader>();
	shader->Use();

	planeMesh = std::unique_ptr<Mesh>(Mesh::LoadFrom("Resources/Meshes/Plane.obj"));
	if (!planeMesh)
	{
		spdlog::critical("Plane mesh load failed.");
		Application::Quit(-1);
	}

	const auto plane = objects.emplace_back(std::make_unique<Object>(planeMesh.get())).get();
	plane->SetScale(glm::vec3(100.0f, 1.0f, 100.0f));

	const auto tank = objects.emplace_back(std::make_unique<Tank>()).get();
	tank->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
}

static bool turnOff = false;

void OnTick(const float deltaTime_) noexcept
{
	if (Input::IsKeyPressed(GLFW_KEY_Q))
	{
		Application::Quit(0);
	}

	if (Input::IsKeyReleased(GLFW_KEY_C))
	{
		const float r = static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX);
		const float g = static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX);
		const float b = static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX);

		light->SetColor(glm::vec3(r, g, b));
	}

	if (Input::IsKeyPressed(GLFW_KEY_M))
	{
		turnOff = !turnOff;
	}

	// 이거 Y랑 S 명령어 합친 거임.
	// 실행하기 전에 명심하셈.
	if (Input::IsKeyHeld(GLFW_KEY_Y))
	{
		const glm::vec3 center  = glm::vec3(0.0f);
		const glm::vec3 current = light->GetPosition();

		glm::vec3 offset = current - center;

		float radius = glm::length(glm::vec2(offset.x, offset.z));
		float height = offset.y;

		if (radius < 0.001f)
		{
			radius = 3.0f;
			offset = glm::vec3(radius, 0.0f, 0.0f);
			height = 0.0f;
		}

		float angle = glm::atan(offset.z, offset.x);
		const float angularSpeed = glm::radians(90.0f); // 90 deg/sec
		const float dir = Input::IsModified(GLFW_MOD_SHIFT) ? -1.0f : 1.0f;

		angle += dir * angularSpeed * deltaTime_;

		const float x = glm::cos(angle) * radius;
		const float z = glm::sin(angle) * radius;

		light->SetPosition(center + glm::vec3(x, height, z));
	}

	if (Input::IsKeyHeld(GLFW_KEY_Z))
	{
		if (Input::IsModified(GLFW_MOD_SHIFT))
		{
			const glm::vec3 forward = glm::normalize(camera->GetForward());
			camera->SetPosition(camera->GetPosition() + forward * 4.0f * deltaTime_);
		}
		else
		{
			const glm::vec3 forward = glm::normalize(camera->GetForward());
			camera->SetPosition(camera->GetPosition() - forward * 4.0f * deltaTime_);
		}
	}

	if (Input::IsKeyHeld(GLFW_KEY_X))
	{
		if (Input::IsModified(GLFW_MOD_SHIFT))
		{
			const glm::vec3 right = glm::normalize(glm::cross(camera->GetForward(), camera->GetUp()));
			camera->SetPosition(camera->GetPosition() + right * 4.0f * deltaTime_);
		}
		else
		{
			const glm::vec3 right = glm::normalize(glm::cross(camera->GetForward(), camera->GetUp()));
			camera->SetPosition(camera->GetPosition() - right * 4.0f * deltaTime_);
		}
	}

	if (Input::IsKeyHeld(GLFW_KEY_Y))
	{
		auto rotationInput = Input::IsModified(GLFW_MOD_SHIFT) ? -1 : 1;

		const float rotationAngle = rotationInput * 10.0f * deltaTime_;

		const glm::vec3 currentForward = camera->GetForward();
		const glm::vec3 upVector	   = camera->GetUp();

		glm::mat4 rotMatrix  = glm::rotate(glm::mat4(1.0f), glm::radians(rotationAngle), upVector);
		glm::vec3 newForward = glm::vec3(rotMatrix * glm::vec4(currentForward, 0.0f));

		camera->SetForward(glm::normalize(newForward));
	}

	for (const auto& object : objects)
	{
		if (object)
		{
			object->Update(deltaTime_);
		}
	}
}

void OnDisplay() noexcept
{
	if (!shader) return;

	if (light)
	{
		shader->SetUniformVector3("lightPos", light->GetPosition());
		shader->SetUniformVector3("lightColor", !turnOff ? light->GetColor() : glm::vec3(0.0f));
		shader->SetUniformVector3("objectColor", glm::vec3(1.0f, 0.5f, 0.31f));
	}

	if (camera)
	{
		shader->SetUniformVector3("viewPos", camera->GetPosition());

		camera->PreRender(*(shader));

		for (const auto& object : objects)
		{
			if (object)
			{
				object->Render(*(shader));
			}
		}
	}
}

void OnClose() noexcept
{
	camera.reset();

	light.reset();

	shader.reset();

	objects.clear();
}