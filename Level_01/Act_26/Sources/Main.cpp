#include "PCH.h"

#include "Application.h"
#include "Camera.h"
#include "Input.h"
#include "Light.h"
#include "Mesh.h"
#include "Object.h"
#include "Shader.h"

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
 * @brief 모든 값을 초기화합니다.
 */
static void Reset() noexcept;

/**
 * @brief 맵을 초기화합니다.
 */
static void GetUserInput() noexcept;

/**
 * @brief 
 */
static constexpr std::size_t MIN_LENGTH = 5;

/**
 * @brief 
 */
static constexpr std::size_t MAX_LENGTH = 25;

/**
 * @brief 맵 가로 사이즈.
 */
static std::size_t width;

/**
 * @brief 맵 세로 사이즈.
 */
static std::size_t height;

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
 * @brief 구 메쉬.
 */
static std::unique_ptr<Mesh> sphereMesh;

/**
 * @brief 구 오브젝트들.
 */
static std::array<std::unique_ptr<Object>, 3> objects;

int main()
{
	Application::Configuration configuration = { };
	configuration.width				= 800;
	configuration.height			= 600;
	configuration.title				= "Level 01 - Act 26";
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
	camera->SetPosition(glm::vec3(5.0f, 5.0f, 5.0f));
	camera->SetForward(glm::vec3(0.0f, 0.0f, 0.0f) - glm::vec3(5.0f, 5.0f, 5.0f));
	camera->SetUp(glm::vec3(0.0f, 1.0f, 0.0f));
	camera->SetViewport(Camera::Viewport{ 0, 0, 800, 600 });
	 
	constexpr glm::vec3 lightPosition = glm::vec3(0.0f, 0.0f, 6.0f);
	constexpr glm::vec3 lightColor	  = glm::vec3(1.0f, 1.0f, 1.0f);
	light = std::make_unique<Light>(lightPosition, lightColor);

	shader = std::make_unique<Shader>();
	shader->Use();

	sphereMesh = std::unique_ptr<Mesh>(Mesh::LoadFrom("Resources/Meshes/Sphere.obj"));
	if (!sphereMesh)
	{
		spdlog::critical("Cube mesh load failed.");
		Application::Quit(-1);
	}

	for (int count = 0; count < 3; ++count)
	{
		objects.at(count) = std::make_unique<Object>(sphereMesh.get());
		objects.at(count)->SetPosition(glm::vec3(static_cast<float>(count * 2 - 2), 0.0f, 0.0f));
	}
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

	if (Input::IsKeyHeld(GLFW_KEY_R))
	{
		const glm::vec3 center  = glm::vec3(0.0f);
		const glm::vec3 current = light->GetPosition();

		glm::vec3 offset = current - center;

		// Horizontal orbit radius (XZ plane) and height preserved
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
}

void OnDisplay() noexcept
{
	if (!shader) return; // shader 체크 추가 권장

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

	for (auto& object : objects)
	{
		object.reset();
	}
}