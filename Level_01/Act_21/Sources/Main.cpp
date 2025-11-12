#include <memory>

#include "Application.h"
#include "Camera.h"
#include "File.h"
#include "Input.h"
#include "Shader.h"
#include "Tank.h"

#include <GLFW/glfw3.h>

#include <spdlog/spdlog.h>

/**
 * @brief 애플리케이션이 시작될 때 호출됩니다.
 */
static void OnStart() noexcept;

/**
 * @brief 매 프레임마다 호출됩니다.
 * 
 * @param deltaTime_ 현재 프레임과 이전 프레임 사이의 간격
 */
static void OnUpdate(float deltaTime_) noexcept;

/**
 * @brief 렌더링이 실행될 때 호출됩니다.
 */
static void OnRender() noexcept;

/**
 * @brief 애플리케이션이 종료될 때 호출됩니다.
 */
static void OnClose() noexcept;

/**
 * @brief 컨텍스트 메이저 버전.
 */
static constexpr int CONTEXT_MAJOR_VERSION = 4;

/**
 * @brief 컨텍스트 마이너 버전.
 */
static constexpr int CONTEXT_MINOR_VERSION = 6;

/**
 * @brief 애플리케이션 이름.
 */
static constexpr const char* APPLICATION_NAME = "Level 1 - Act 20";

/**
 * @brief 애플리케이션 너비.
 */
static constexpr int APPLICATION_WIDTH = 1280;

/**
 * @brief 애플리케이션 높이.
 */
static constexpr int APPLICATION_HEIGHT = 720;

/**
 * @brief 애플리케이션 내에서 사용할 카메라.
 */
static std::unique_ptr<Camera> camera;

/**
 * @brief 애플리케이션 내에서 사용할 셰이더.
 */
static std::unique_ptr<Shader> shader;

/**
 * @brief 애플리케이션 내에서 사용할 탱크.
 */
static std::unique_ptr<Tank> tank;

int main(int, char**)
{
	Application::Specification specification = {};
	specification.majorVersion				 = CONTEXT_MAJOR_VERSION;
	specification.minorVersion				 = CONTEXT_MINOR_VERSION;
	specification.name						 = APPLICATION_NAME;
	specification.width						 = APPLICATION_WIDTH;
	specification.height					 = APPLICATION_HEIGHT;
	specification.onStart					 = OnStart;
	specification.onUpdate					 = OnUpdate;
	specification.onRender					 = OnRender;
	specification.onClose					 = OnClose;

	return Application::Run(specification);
}

void OnStart() noexcept
{
	const std::string vertexShaderFile   = File::ReadFile("Resources/Shaders/Vertex.vert");
	const char* const vertexShaderSource = vertexShaderFile.c_str();

	const std::string fragmentShaderFile   = File::ReadFile("Resources/Shaders/Fragment.frag");
	const char* const fragmentShaderSource = fragmentShaderFile.c_str();

	shader = std::make_unique<Shader>(vertexShaderSource, fragmentShaderSource);
	shader->Use();
	
	constexpr Camera::ProjectionType projectionType = Camera::ProjectionType::Perspective;
	const glm::vec3 target   = glm::vec3(0.0f, 0.0f, 0.0f);
	const glm::vec3 position = glm::vec3(0.0f, 10.0f, 20.0f);
	const glm::vec3 front    = glm::normalize(target - position);
	const glm::vec3 up       = glm::vec3(0.0f, 1.0f, 0.0f);

	camera = std::make_unique<Camera>(projectionType, position, front, up);

	tank = std::make_unique<Tank>();
}

static bool isAnimA = false;

static float orbitAngle = 0.0f;

void OnUpdate(float deltaTime_) noexcept
{
	static bool trigger = false;

	if (Input::IsKey(GLFW_KEY_Z))
	{
		const auto position = camera->GetPosition();
		const auto forward  = glm::normalize(camera->GetForward());
	}

	if (Input::IsKeyDown(GLFW_KEY_Q))
	{
		Application::Quit();
	}

	if (Input::IsKeyDown(GLFW_KEY_O))
	{
		trigger = !trigger;
	}

	if (Input::IsKeyDown(GLFW_KEY_C))
	{
		tank.reset();
		tank = std::make_unique<Tank>();
	}

	if (Input::IsKeyDown(GLFW_KEY_Z))
	{
		if (Input::IsModPressed(GLFW_MOD_SHIFT))
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

	if (Input::IsKeyDown(GLFW_KEY_X))
	{
		if (Input::IsModPressed(GLFW_MOD_SHIFT))
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

	if (Input::IsKeyDown(GLFW_KEY_Y))
	{
		auto rotationInput = Input::IsModPressed(GLFW_MOD_SHIFT) ? -1 : 1;

		const float rotationAngle = rotationInput * 10.0f * deltaTime_;

		const glm::vec3 currentForward = camera->GetForward();
		const glm::vec3 upVector	   = camera->GetUp();

		glm::mat4 rotMatrix  = glm::rotate(glm::mat4(1.0f), glm::radians(rotationAngle), upVector);
		glm::vec3 newForward = glm::vec3(rotMatrix * glm::vec4(currentForward, 0.0f));

		camera->SetForward(glm::normalize(newForward));
	}

	if (Input::IsKeyDown(GLFW_KEY_A))
	{
		isAnimA = !isAnimA;
	}

	if (Input::IsKeyDown(GLFW_KEY_R) || isAnimA)
	{
		const float orbitSpeed = 45.0f; 
		const float orbitRadius = 20.0f;
		const float cameraHeight = 10.0f;
		const glm::vec3 target = glm::vec3(0.0f, 0.0f, 0.0f); 

		orbitAngle += orbitSpeed * deltaTime_;
		if (orbitAngle > 360.0f)
		{
			orbitAngle -= 360.0f;
		}

		const float angleInRadians = glm::radians(orbitAngle);

		glm::vec3 newPosition;
		newPosition.x = orbitRadius * glm::cos(angleInRadians);
		newPosition.y = cameraHeight;
		newPosition.z = orbitRadius * glm::sin(angleInRadians);

		glm::vec3 newForward = glm::normalize(target - newPosition);

		camera->SetPosition(newPosition);
		camera->SetForward(newForward);
	}

	if (!trigger)
		tank->Update(deltaTime_);
}

void OnRender() noexcept
{
	camera->PreRender(*(shader));
	tank->Render(*(shader));
}

void OnClose() noexcept
{
	camera.reset();
	shader.reset();
}