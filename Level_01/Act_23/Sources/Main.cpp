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
static std::unique_ptr<Camera> mainCamera;
static std::unique_ptr<Camera> topViewCamera;
static std::unique_ptr<Camera> sideViewCamera;

/**
 * @brief 애플리케이션 내에서 사용할 셰이더.
 */
static std::unique_ptr<Shader> shader;

/**
 * @brief 바닥.
 */
static std::unique_ptr<Object> plane;

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
	
	// 1. 메인 카메라 (기존 카메라 설정)
	constexpr Camera::Projection mainProjection = Camera::Projection::Perspective;
	const glm::vec3 target = glm::vec3(0.0f, 0.0f, 0.0f);
	const glm::vec3 position = glm::vec3(0.0f, 10.0f, 20.0f);
	const glm::vec3 front = glm::normalize(target - position);
	const glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	mainCamera = std::make_unique<Camera>(mainProjection, position, front, up);

	// 2. Top 뷰 카메라 (직교 투영)
	constexpr Camera::Projection orthoProjection = Camera::Projection::Orthographic;
	const glm::vec3 topPos = glm::vec3(0.0f, 50.0f, 0.0f); // 위에서 아래로
	const glm::vec3 topFront = glm::vec3(0.0f, -1.0f, 0.0f);
	const glm::vec3 topUp = glm::vec3(0.0f, 0.0f, -1.0f); // 위쪽이 -Z 방향
	topViewCamera = std::make_unique<Camera>(orthoProjection, topPos, topFront, topUp);

	// 3. Side 뷰 카메라 (직교 투영)
	const glm::vec3 sidePos = glm::vec3(50.0f, 0.0f, 0.0f); // +X에서 -X 방향으로
	const glm::vec3 sideFront = glm::vec3(-1.0f, 0.0f, 0.0f);
	const glm::vec3 sideUp = glm::vec3(0.0f, 1.0f, 0.0f); // 위쪽이 +Y 방향
	sideViewCamera = std::make_unique<Camera>(orthoProjection, sidePos, sideFront, sideUp);

	tank = std::make_unique<Tank>();

	plane = std::make_unique<Object>();
	plane->SetMesh(Mesh::LoadFrom("Resources/Models/Plane.obj"));
	plane->SetScale(glm::vec3(100.0f, 1.0f, 100.0f));
}

static bool isAnimA = false;

static float orbitAngle = 0.0f;

void OnUpdate(float deltaTime_) noexcept
{
	static bool trigger = false;

	if (Input::IsKeyPressed(GLFW_KEY_Q))
	{
		Application::Quit();
	}

	if (Input::IsKeyPressed(GLFW_KEY_O))
	{
		trigger = !trigger;
	}

	if (Input::IsKeyPressed(GLFW_KEY_C))
	{
		tank.reset();
		tank = std::make_unique<Tank>();
	}

	if (Input::IsKeyPressed(GLFW_KEY_Z))
	{
		if (Input::IsModPressed(GLFW_MOD_SHIFT))
		{
			{
				const glm::vec3 forward = glm::normalize(mainCamera->GetForward());
				mainCamera->SetPosition(mainCamera->GetPosition() + forward * 4.0f * deltaTime_);
			}
			{
				const glm::vec3 forward = glm::normalize(sideViewCamera->GetForward());
				sideViewCamera->SetPosition(sideViewCamera->GetPosition() + forward * 4.0f * deltaTime_);

			}
			{
				const glm::vec3 forward = glm::normalize(topViewCamera->GetForward());
				topViewCamera->SetPosition(topViewCamera->GetPosition() + forward * 4.0f * deltaTime_);
			}
		}
		else
		{
			{
				const glm::vec3 forward = glm::normalize(mainCamera->GetForward());
				mainCamera->SetPosition(mainCamera->GetPosition() - forward * 4.0f * deltaTime_);
			}
			{
				const glm::vec3 forward = glm::normalize(sideViewCamera->GetForward());
				sideViewCamera->SetPosition(sideViewCamera->GetPosition() - forward * 4.0f * deltaTime_);
			}
			{
				const glm::vec3 forward = glm::normalize(topViewCamera->GetForward());
				topViewCamera->SetPosition(topViewCamera->GetPosition() - forward * 4.0f * deltaTime_);
			}
		}
	}

	if (Input::IsKeyPressed(GLFW_KEY_X))
	{
		if (Input::IsModPressed(GLFW_MOD_SHIFT))
		{
			{
				const glm::vec3 right = glm::normalize(glm::cross(mainCamera->GetForward(), mainCamera->GetUp()));
				mainCamera->SetPosition(mainCamera->GetPosition() + right * 4.0f * deltaTime_);
			}
			{
				const glm::vec3 right = glm::normalize(glm::cross(sideViewCamera->GetForward(), sideViewCamera->GetUp()));
				sideViewCamera->SetPosition(sideViewCamera->GetPosition() + right * 4.0f * deltaTime_);
			}
			{
				const glm::vec3 right = glm::normalize(glm::cross(topViewCamera->GetForward(), topViewCamera->GetUp()));
				topViewCamera->SetPosition(topViewCamera->GetPosition() + right * 4.0f * deltaTime_);
			}
		}
		else
		{
			{
				const glm::vec3 right = glm::normalize(glm::cross(mainCamera->GetForward(), mainCamera->GetUp()));
				mainCamera->SetPosition(mainCamera->GetPosition() - right * 4.0f * deltaTime_);
			}
			{
				const glm::vec3 right = glm::normalize(glm::cross(sideViewCamera->GetForward(), sideViewCamera->GetUp()));
				sideViewCamera->SetPosition(sideViewCamera->GetPosition() - right * 4.0f * deltaTime_);
			}
			{
				const glm::vec3 right = glm::normalize(glm::cross(topViewCamera->GetForward(), topViewCamera->GetUp()));
				topViewCamera->SetPosition(topViewCamera->GetPosition() - right * 4.0f * deltaTime_);
			}
		}
	}

	if (Input::IsKeyPressed(GLFW_KEY_Y))
	{
		auto rotationInput = Input::IsModPressed(GLFW_MOD_SHIFT) ? -1 : 1;
		
		const float rotationAngle = rotationInput * 10.0f * deltaTime_;
		
		{
			const glm::vec3 currentForward = mainCamera->GetForward();
			const glm::vec3 upVector = mainCamera->GetUp();

			glm::mat4 rotMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(rotationAngle), upVector);
			glm::vec3 newForward = glm::vec3(rotMatrix * glm::vec4(currentForward, 0.0f));

			mainCamera->SetForward(glm::normalize(newForward));
		}
		{
			const glm::vec3 currentForward = sideViewCamera->GetForward();
			const glm::vec3 upVector = sideViewCamera->GetUp();
			glm::mat4 rotMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(rotationAngle), upVector);
			glm::vec3 newForward = glm::vec3(rotMatrix * glm::vec4(currentForward, 0.0f));
			sideViewCamera->SetForward(glm::normalize(newForward));
		}
		{
			const glm::vec3 currentForward = topViewCamera->GetForward();
			const glm::vec3 upVector = topViewCamera->GetUp();
			glm::mat4 rotMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(rotationAngle), upVector);
			glm::vec3 newForward = glm::vec3(rotMatrix * glm::vec4(currentForward, 0.0f));
			topViewCamera->SetForward(glm::normalize(newForward));
		}
	}

	if (Input::IsKeyPressed(GLFW_KEY_A))
	{
		isAnimA = !isAnimA;
	}

	if (Input::IsKeyPressed(GLFW_KEY_R) || isAnimA)
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

		// camera->SetPosition(newPosition);
		// camera->SetForward(newForward);
	}

	if (!trigger)
		tank->Update(deltaTime_);
}

void OnRender() noexcept
{
	const auto& spec = Application::GetSpecification();
	const int totalWidth = spec.width;
	const int totalHeight = spec.height;

	{
		const int v_x = 0;
		const int v_y = 0;
		const int v_w = totalWidth / 2;
		const int v_h = totalHeight;
		const float aspect = static_cast<float>(v_w) / static_cast<float>(v_h);

		glViewport(v_x, v_y, v_w, v_h);
		mainCamera->PreRender(*(shader), aspect);

		tank->Render(*(shader));
		plane->Render(*(shader));
	}

	{
		const int v_x = totalWidth / 2;
		const int v_y = totalHeight / 2;
		const int v_w = totalWidth / 2;
		const int v_h = totalHeight / 2;
		const float aspect = static_cast<float>(v_w) / static_cast<float>(v_h);

		glViewport(v_x, v_y, v_w, v_h);
		topViewCamera->PreRender(*(shader), aspect);

		tank->Render(*(shader));
		plane->Render(*(shader));
	}

	{
		const int v_x = totalWidth / 2;
		const int v_y = 0;
		const int v_w = totalWidth / 2;
		const int v_h = totalHeight / 2;
		const float aspect = static_cast<float>(v_w) / static_cast<float>(v_h);

		glViewport(v_x, v_y, v_w, v_h);
		sideViewCamera->PreRender(*(shader), aspect);

		tank->Render(*(shader));
		plane->Render(*(shader));
	}
}

void OnClose() noexcept
{
	shader.reset();
	plane.reset();
}