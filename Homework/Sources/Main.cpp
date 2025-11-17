#include "PCH.h"

#include "Application.h"
#include "Camera.h"
#include "Input.h"
#include "Object.h"
#include "Shader.h"
#include "Stage.h"
#include "Wall.h"

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
static std::unique_ptr<Camera> mainCamera;

/**
 * @brief 서브 카메라.
 */
static std::unique_ptr<Camera> subCamera;

/**
 * @brief 셰이더.
 */
static std::unique_ptr<Shader> shader;

/**
 * @brief
 */
static std::unique_ptr<Mesh> stageMesh;

/**
 * @brief 
 */
static std::unique_ptr<Mesh> wallMesh;

/**
 * @brief 오브젝트들.
 */
static std::vector<std::unique_ptr<Object>> objects;

/**
 * @brief 난수 발생을 위한 랜덤 디바이스.
 */
static std::random_device rd;

/**
 * @brief 난수 생성기.
 */
static std::mt19937 gen(rd());

int main()
{
	::GetUserInput();

	Application::Configuration configuration = { };
	configuration.width				= 800;
	configuration.height			= 600;
	configuration.title				= "CG Homework";
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
	mainCamera = std::make_unique<Camera>();
	mainCamera->SetProjection(Camera::Projection::Perspective);
	mainCamera->SetPosition(glm::vec3(0.0f, 10.0f, 20.0f));
	mainCamera->SetForward(glm::vec3(0.0f, 0.0f, 0.0f) - glm::vec3(0.0f, 10.0f, 20.0f));
	mainCamera->SetUp(glm::vec3(0.0f, 1.0f, 0.0f));
	mainCamera->SetViewport(Camera::Viewport{ 0, 0, 800, 600 });

	 subCamera = std::make_unique<Camera>();
	 subCamera->SetProjection(Camera::Projection::Orthographic);
	 subCamera->SetPosition(glm::vec3(0.0f, 20.0f, 0.0f));
	 subCamera->SetForward(glm::vec3(0.0f, -1.0f, 0.0f));
	 subCamera->SetUp(glm::vec3(0.0f, 0.0f, 1.0f));
	 subCamera->SetViewport(Camera::Viewport{ 700, 500, 100, 100 });

	shader = std::make_unique<Shader>();
	shader->Use();

	stageMesh = std::unique_ptr<Mesh>(Mesh::LoadFrom("Resources/Meshes/Stage.obj"));
	if (!stageMesh)
	{
		std::println("Stage mesh load failed.");
		Application::Quit(-1);
	}
	objects.push_back(std::make_unique<Stage>(width, height, stageMesh.get()));
	
	wallMesh = std::unique_ptr<Mesh>(Mesh::LoadFrom("Resources/Meshes/Wall.obj"));
	if (!wallMesh)
	{
		std::println("Wall mesh load failed.");
		Application::Quit(-1);
	}

	std::vector<std::vector<int>> gridHeights(height, std::vector<int>(width, 5));

	const int centerX = static_cast<int>(width / 2);
	const int centerZ = static_cast<int>(height / 2);

	for (std::size_t x = 0; x < width; ++x)
	{
		gridHeights[centerZ][x] = 0;
	}

	for (std::size_t z = 0; z < height; ++z)
	{
		gridHeights[z][centerX] = 0;
	}

	std::uniform_int_distribution<int> heightDist(1, 5);
	for (std::size_t z = 0; z < height; ++z)
	{
		for (std::size_t x = 0; x < width; ++x)
		{
			const int wallHeight = gridHeights[z][x];

			if (wallHeight > 0)
			{
				Object* wall = objects.emplace_back(std::make_unique<Wall>(heightDist(gen), wallMesh.get())).get();

				const float posX = static_cast<float>(x) - static_cast<float>(width) / 2.0f + 0.5f;
				const float posZ = static_cast<float>(z) - static_cast<float>(height) / 2.0f + 0.5f;
				const float posY = 0.501f + (wall->GetScale().y / 2.0f);

				wall->SetPosition(glm::vec3(posX, posY, posZ));
			}
		}
	}
}

void OnTick(const float deltaTime_) noexcept
{
	if (Input::IsKeyPressed(GLFW_KEY_O))
	{
		mainCamera->SetProjection(Camera::Projection::Orthographic);
	}
	else if (Input::IsKeyPressed(GLFW_KEY_P))
	{
		mainCamera->SetProjection(Camera::Projection::Perspective);
	}

	if (Input::IsKeyPressed(GLFW_KEY_C))
	{
		::Reset();
	}
	if (Input::IsKeyPressed(GLFW_KEY_Q))
	{
		Application::Quit(0);
	}
	
	for (const auto& object : objects)
	{
		if (!object)
		{
			continue;
		}

		object->Update(deltaTime_);
	}
}

void OnDisplay() noexcept
{
	if (!shader)
	{
		return;
	}

	if (mainCamera)
	{
		mainCamera->PreRender(*shader);

		for (const auto& object : objects)
		{
			if (object)
			{
				object->Render(*shader);
			}
		}
	}

	if (subCamera)
	{
		subCamera->PreRender(*shader);
		
		for (const auto& object : objects)
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
	mainCamera.reset();
	subCamera.reset();

	shader.reset();

	for (auto& object : objects)
	{
		object.reset();
	}
}

void Reset() noexcept
{
	::OnClose();
	::GetUserInput();
	::OnLoad();
}

void GetUserInput() noexcept
{
	std::system("cls");

	while (true)
	{
		std::print("가로/세로 길이를 입력해주세요.\n");
		std::print("※ 길이는 {}에서 {} 사이의 정수로 입력해주세요.\n", MIN_LENGTH, MAX_LENGTH);
		std::print("※ 종료를 원하신다면 'q'/'Q'를 입력해주세요.\n");
		std::print("==>> ");

		std::string input;
		std::getline(std::cin, input);

		if (input == "Q" || input == "q")
		{
			Application::Quit(0);
		}
		else
		{
			std::istringstream iss(input);
			std::string		   token;

			iss >> token;
			width = std::stoi(token);

			iss >> token;
			height = std::stoi(token);

			break;
		}
	}

	std::system("cls");

	std::print("입력된 길이: 가로: {}, 세로: {}\n", width, height);
	std::print("※ 프로그램 리셋: ['c'/'C']\n");
	std::print("※ 프로그램 종료: ['q'/'Q']\n");
}