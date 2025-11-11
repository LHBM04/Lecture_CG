#include <memory>

#include "Application.h"
#include "Camera.h"
#include "File.h"
#include "Shader.h"

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
	constexpr glm::vec3 position					= glm::vec3(0.0f, 0.0f, 10.0f);
	constexpr glm::vec3 front						= glm::vec3(0.0f, 0.0f, -1.0f);
	constexpr glm::vec3 up							= glm::vec3(0.0f, 1.0f,  0.0f);

	camera = std::make_unique<Camera>(projectionType, position, front, up);
}

void OnUpdate(float deltaTime_) noexcept
{
	
}

void OnRender() noexcept
{
	camera->PreRender(*(shader));
	
}

void OnClose() noexcept
{
	camera.reset();
	shader.reset();
}