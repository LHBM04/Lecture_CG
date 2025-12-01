#include "PCH.h"

#include "Application.h"
#include "Camera.h"
#include "Input.h"
#include "Light.h"
#include "Mesh.h"
#include "Object.h"
#include "Shader.h"
#include "Texture.h"

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
 * @brief 사용할 셰이더.
 */
static std::vector<std::unique_ptr<Shader>> shader;

/**
 * @brief 표준 셰이더.
 */
static Shader* standardShader = nullptr;

/**
 * @brief 텍스처 셰이더.
 */
static Shader* textureShader = nullptr;

/**
 * @brief 사용할 메쉬들.
 */
static std::vector<std::unique_ptr<Mesh>> meshes;

/**
 * @brief 큐브 메쉬.
 */
static Mesh* cubeMesh = nullptr;

/**
 * @brief 피라미드 메쉬.
 */
static Mesh* pyramidMesh = nullptr;

/**
 * @brief 사용할 텍스처들.
 */
static std::vector<std::unique_ptr<Texture>> textures;

/**
 * @brief 큐브 텍스처.
 */
static Texture* cubeTexture = nullptr;

/**
 * @brief 피라미드 텍스처.
 */
static Texture* pyramidTexture = nullptr;

/**
 * @brief 시뮬레이션 할 오브젝트들.
 */
static std::vector<std::unique_ptr<Object>> objects;

/**
 * @brief 단일 큐브 오브젝트 (테스트용).
 */
static Object* cube = nullptr;

/**
 * @brief 단일 피라미드 오브젝트 (테스트용).
 */
static Object* pyramid = nullptr;

int main()
{
	Application::Configuration configuration = { };
	configuration.width				= 800;
	configuration.height			= 600;
	configuration.title				= "Level 01 - Act 29";
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
	camera->SetPosition(glm::vec3(0.0f, 2.0f, 5.0f));
	camera->SetForward(glm::normalize(glm::vec3(0.0f, 0.0f, 0.0f) - camera->GetPosition()));
	camera->SetUp(glm::vec3(0.0f, 1.0f, 0.0f));
	camera->SetViewport(Camera::Viewport{ 0, 0, 800, 600 });

	constexpr glm::vec3 lightPosition = glm::vec3(0.0f, 2.0f, 5.0f);
	constexpr glm::vec3 lightColor    = glm::vec3(1.0f, 1.0f, 1.0f);
	light = std::make_unique<Light>(lightPosition, lightColor);

	standardShader = shader.emplace_back(std::make_unique<Shader>("Resources/Shaders/Standard.vert",
																  "Resources/Shaders/Standard.frag")).get();
	if (!standardShader)
	{
		spdlog::critical("Standard shader creation failed.");
		Application::Quit(-1);
	}

	textureShader = shader.emplace_back(std::make_unique<Shader>("Resources/Shaders/Texture.vert",
																 "Resources/Shaders/Texture.frag")).get();
	if (!textureShader)
	{
		spdlog::critical("Texture shader creation failed.");
		Application::Quit(-1);
	}

	cubeMesh = meshes.emplace_back(Mesh::LoadFrom("Resources/Meshes/Cube.obj")).get();
	if (!cubeMesh)
	{
		spdlog::critical("Cube mesh load failed.");
		Application::Quit(-1);
	}

	pyramidMesh = meshes.emplace_back(Mesh::LoadFrom("Resources/Meshes/Pyramid.obj")).get();
	if (!pyramidMesh)
	{
		spdlog::critical("Pyramid mesh load failed.");
		Application::Quit(-1);
	}

	cubeTexture = textures.emplace_back(Texture::LoadFrom("Resources/Textures/Texture.png")).get();
	if (!cubeTexture)
	{
		spdlog::critical("Cube texture load failed.");
		Application::Quit(-1);
	}

	pyramidTexture = textures.emplace_back(Texture::LoadFrom("Resources/Textures/Texture.png")).get();
	if (!pyramidTexture)
	{
		spdlog::critical("Pyramid texture load failed.");
		Application::Quit(-1);
	}

	cube = objects.emplace_back(std::make_unique<Object>(cubeMesh, cubeTexture)).get();
	cube->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));

	pyramid = objects.emplace_back(std::make_unique<Object>(pyramidMesh, pyramidTexture)).get();
	pyramid->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
}

static bool trigger = false;

static bool turnOff = false;

void OnTick(const float deltaTime_) noexcept
{
	if (Input::IsKeyPressed(GLFW_KEY_Q))
	{
		Application::Quit(0);
	}

	if (Input::IsKeyPressed(GLFW_KEY_C))
		trigger = false;

	if (Input::IsKeyPressed(GLFW_KEY_P))
		trigger = true;

	if (!trigger)
	{
		cube->Update(deltaTime_);
	}
	if (trigger)
	{
		pyramid->Update(deltaTime_);
	}
}

void OnDisplay() noexcept
{
	textureShader->Use();
	textureShader->SetUniformVector3("uLightPos", light->GetPosition());
	textureShader->SetUniformVector3("uLightColor", light->GetColor());
	textureShader->SetUniformVector3("uViewPos", camera->GetPosition());

	camera->PreRender(*textureShader);

	if (!trigger)
		cube->Render(*textureShader);
	else
		pyramid->Render(*textureShader);
}

void OnClose() noexcept
{
	camera.reset();

	light.reset();

	objects.clear();
}