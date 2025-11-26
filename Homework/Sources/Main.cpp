#include "PCH.h"

#include "Application.h"
#include "Input.h"

#include "Camera.h"
#include "Light.h"
#include "Mesh.h"
#include "Shader.h"

#include "Object.h"
#include "Player.h"
#include "Mountain.h"

/**
 * @brief 카메라 모드를 정의합니다.
 */
enum class CameraMode : unsigned char
{
	/**
	 * @brief 1인칭.
	 */
	FirstPerson,

	/**
	 * @brief 3인칭 쿼터뷰.
	 */
	ThirdPerson,
	
	/**
	 * @brief 일반.
	 */
	TopDown
};

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
 * @brief 미로를 만듭니다.
 */
static void BuildMaze() noexcept;

/**
 * @brief 플레이어를 생성합니다.
 */
static void SpawnPlayer() noexcept;

/**
 * @brief 애플리케이션 제목.
 */
static constexpr const char* APP_TITLE = "CG Homework";

/**
 * @brief 애플리케이션 너비.
 */
static constexpr int APP_WIDTH = 1920;

/**
 * @brief 애플리케이션 높이.
 */
static constexpr int APP_HEIGHT = 1080;

/**
 * @brief 무대의 최대 가로 사이즈.
 */
static constexpr std::size_t MIN_LENGTH = 5;

/**
 * @brief 무대의 최대 세로 사이즈.
 */
static constexpr std::size_t MAX_LENGTH = 25;

/**
 * @brief 무대의 가로 사이즈.
 */
static int width;

/**
 * @brief 무대의 세로 사이즈.
 */
static int height;

/**
 * @brief 메인 카메라.
 */
static std::unique_ptr<Camera> mainCamera;

/**
 * @brief 현재 카메라 모드.
 */
static CameraMode cameraMode = CameraMode::TopDown;

/**
 * @brief 서브 카메라.
 */
static std::unique_ptr<Camera> subCamera;

/**
 * @brief 조명.
 */
static std::unique_ptr<Light> light;

/**
 * @brief 플레이어 오브젝트.
 */
static std::unique_ptr<Player> player = nullptr;

/**
 * @brief 산 오브젝트들.
 */
static std::vector<std::unique_ptr<Mountain>> mountains;

int main()
{
	::SetConsoleOutputCP(CP_UTF8);
	::SetConsoleCP(CP_UTF8);

	::GetUserInput();

	Application::Configuration configuration = { };
	configuration.width				= APP_WIDTH;
	configuration.height			= APP_HEIGHT;
	configuration.title				= APP_TITLE;
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
	if (!Shader::Initialize())
	{
		spdlog::critical("Shader initialize failed");
		Application::Quit(-1);
	}

	// 메인 카메라
	{
		constexpr glm::vec2 camSize = glm::vec2(APP_WIDTH, APP_WIDTH);
		constexpr glm::vec2 camPos  = glm::vec2(APP_WIDTH, APP_HEIGHT) - camSize;

		const     glm::vec3		   eye		= glm::vec3(0.0f, 20.0f, 40.0f);
		constexpr glm::vec3		   at		= glm::vec3(0.0f,  0.0f,   0.0f);
		constexpr glm::vec3		   up		= glm::vec3(0.0f,  1.0f,   0.0f);
		const	  Camera::Viewport viewport = Camera::Viewport(camPos.x, camPos.y, camSize.x, camSize.y);

		mainCamera = std::make_unique<Camera>(Camera::Projection::Perspective, eye, at, up, viewport);
	}
	// 서브 카메라
	{
		constexpr glm::vec2 camSize = glm::vec2(300.0f, 300.0f);
		constexpr glm::vec2 camPos  = glm::vec2(APP_WIDTH, APP_HEIGHT) - camSize;

		const     glm::vec3		   eye		= glm::vec3(0.0f, (height / 10) * 20.0f,  0.0f);
		constexpr glm::vec3		   at		= glm::vec3(0.0f,  0.0f,  0.0f);
		constexpr glm::vec3		   up       = glm::vec3(0.0f,  0.0f, -1.0f);
		const	  Camera::Viewport viewport = Camera::Viewport(camPos.x, camPos.y, camSize.x, camSize.y);
		
		subCamera = std::make_unique<Camera>(Camera::Projection::Perspective, eye, at, up, viewport);
	}
	// 조명
	{
		constexpr glm::vec3 position = glm::vec3(0.0f, 20.0f, 0.0f);
		constexpr glm::vec3 color	 = glm::vec3(1.0f, 1.0f, 1.0f);
		light = std::make_unique<Light>(position, color);
	}

	std::vector<bool> map(width * height, false);

	// 2. 십자(Cross) 만들기
	// 맵의 중앙 인덱스 계산
	int centerZ = height / 2;
	int centerX = width / 2;

	// 가로줄 뚫기 (좌 -> 우)
	for (int x = 0; x < width; ++x)
	{
		map[centerZ * width + x] = true;
	}

	// 세로줄 뚫기 (상 -> 하)
	for (int z = 0; z < height; ++z)
	{
		map[z * width + centerX] = true;
	}
	
	mountains.clear();
	mountains.reserve(width * height);

	for (int z = 0; z < height; ++z)
	{
		for (int x = 0; x < width; ++x)
		{
			Mountain* mountain = mountains.emplace_back(std::make_unique<Mountain>(map[z * width + x])).get();

			const float posX = static_cast<float>(x) - static_cast<float>(width) * 0.5f + 0.5f;
			const float posZ = static_cast<float>(z) - static_cast<float>(height) * 0.5f + 0.5f;

			mountain->SetPosition(glm::vec3(posX, 0.0f, posZ));
		}
	}
}

void OnTick(const float deltaTime_) noexcept
{
#pragma region Control Cameras
	if (Input::IsKeyPressed(GLFW_KEY_O))
	{
		mainCamera->SetProjection(Camera::Projection::Orthographic);
	}
	else if (Input::IsKeyPressed(GLFW_KEY_P))
	{
		mainCamera->SetProjection(Camera::Projection::Perspective);
	}
	
	if (Input::IsKeyHeld(GLFW_KEY_Z))
	{
		if (mainCamera->GetProjection() != Camera::Projection::Perspective)
		{
			return;
		}

		constexpr float CAM_MOVE_SPEED = 5.0f;
		const float dir = Input::IsModified(GLFW_MOD_SHIFT) ? -1.0f : 1.0f;

		const glm::vec3 forward = glm::normalize(mainCamera->GetForward());
		const glm::vec3 newPos = mainCamera->GetPosition() + forward * CAM_MOVE_SPEED * dir * deltaTime_;

		if (cameraMode == CameraMode::TopDown)
		{
			mainCamera->SetPosition(newPos);
		}
	}
	if (Input::IsKeyHeld(GLFW_KEY_Y))
	{
		if (mainCamera->GetProjection() != Camera::Projection::Perspective)
		{
			return;
		}

		constexpr float ORBIT_SPEED = 2.0f;

		const float direction = Input::IsModified(GLFW_MOD_SHIFT) ? -1.0f : 1.0f;
		const float angle	  = ORBIT_SPEED * deltaTime_ * direction;

		const glm::vec3 currentPos     = mainCamera->GetPosition();
		const glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 1.0f, 0.0f));
		const glm::vec3 newPos         = glm::vec3(rotationMatrix * glm::vec4(currentPos, 1.0f));

		mainCamera->SetPosition(newPos);
		mainCamera->SetForward(glm::normalize(-newPos));
	}

	if (Input::IsKeyPressed(GLFW_KEY_3))
	{
		cameraMode = CameraMode::ThirdPerson;
	}
	else if (Input::IsKeyPressed(GLFW_KEY_1))
	{
		cameraMode = CameraMode::FirstPerson;
	}
	else if (Input::IsKeyPressed(GLFW_KEY_2))
	{
		cameraMode = CameraMode::TopDown;
	}

	switch (cameraMode)
	{
		case CameraMode::FirstPerson:
		{
			if (player && mainCamera)
			{
				const glm::vec3 playerPos = player->GetPosition();
				const glm::vec3 eyeOffset = glm::vec3(0.0f, 0.1f, 0.0f);

				mainCamera->SetPosition(playerPos + eyeOffset);

				const glm::vec3 rotation = player->GetRotation();
				const float     yawRad = glm::radians(rotation.y);

				glm::vec3 dir = glm::normalize(glm::vec3(std::sin(yawRad), 0.0f, std::cos(yawRad)));

				mainCamera->SetForward(dir);
				mainCamera->SetUp(glm::vec3(0.0f, 1.0f, 0.0f));
			}
			break;
		}
		case CameraMode::ThirdPerson:
		{
			if (player && mainCamera)
			{
				const glm::vec3 playerPos = player->GetPosition();

				constexpr float heightOffset = 2.0f;
				constexpr float backOffset   = 5.0f;

				const glm::vec3 desiredPos = playerPos + glm::vec3(0.0f, heightOffset, backOffset);
				mainCamera->SetPosition(desiredPos);

				const glm::vec3 lookTarget    = playerPos;
				const glm::vec3 targetForward = glm::normalize(lookTarget - desiredPos);

				mainCamera->SetForward(targetForward);
				mainCamera->SetUp(glm::vec3(0.0f, 1.0f, 0.0f));
			}
			break;
		}
		default:
		{
			break;
		}
	}
#pragma endregion

	if (Input::IsKeyPressed(GLFW_KEY_C))
	{
		::OnClose();
		::GetUserInput();
		::OnLoad();
	}
	if (Input::IsKeyPressed(GLFW_KEY_Q))
	{
		Application::Quit(0);
	}
	if (Input::IsKeyPressed(GLFW_KEY_S))
	{
		player = std::make_unique<Player>();
		player->SetPosition(glm::vec3(0.0f, 10.0f, 0.0f));
	}
	
	if (player)
	{
		player->Update(deltaTime_);

		// 충돌 처리를 여기서 하는 게 맘에 들지 않는다.
		// 근데 시간이 없다.
		AABB playerBox = player->GetAABB();
		glm::vec3 pPos = player->GetPosition();
		bool collided = false;

		for (const auto& mountain : mountains)
		{
			if (mountain->GetScale().y < 0.5f)
			{
				continue;
			}

			AABB wallBox = mountain->GetAABB();

			if (AABB::CheckCollision(playerBox, wallBox))
			{
				glm::vec3 mPos = mountain->GetPosition();

				const float xOverlap = glm::min(playerBox.max.x, wallBox.max.x) - glm::max(playerBox.min.x, wallBox.min.x);
				const float zOverlap = glm::min(playerBox.max.z, wallBox.max.z) - glm::max(playerBox.min.z, wallBox.min.z);
				const float yOverlap = glm::min(playerBox.max.y, wallBox.max.y) - glm::max(playerBox.min.y, wallBox.min.y);

				if (xOverlap < zOverlap && xOverlap < yOverlap)
				{
					if (pPos.x < mPos.x)
					{
						pPos.x -= xOverlap;
					}
					else
					{
						pPos.x += xOverlap;
					}
				}
				else if (zOverlap < xOverlap && zOverlap < yOverlap)
				{
					if (pPos.z < mPos.z)
					{
						pPos.z -= zOverlap;
					}
					else
					{
						pPos.z += zOverlap;
					}
				}
				else 
				{
					if (pPos.y > mPos.y)
					{
						pPos.y += yOverlap;
					}
					else
					{
						pPos.y -= yOverlap;
					}
				}

				collided = true;
			}
		}

		if (collided)
		{
			player->SetPosition(pPos);

			if (light)
			{
				light->SetPosition(pPos + glm::vec3(0.0f, 1.0f, 0.5f));
			}
		}
	}

	for (const std::unique_ptr<Mountain>& mountain : mountains)
	{
		mountain->Update(deltaTime_);
	}
}

void OnDisplay() noexcept
{
	if (mainCamera)
	{
		mainCamera->PreRender();

		// 1인칭 일 때는 스킵.
		if (cameraMode != CameraMode::FirstPerson)
			if (player)
				player->Render();

		for (const std::unique_ptr<Mountain>& mountain : mountains)
		{
			mountain->Render();
		}
	}

	if (subCamera)
	{
		subCamera->PreRender();

		Shader::SetUniformVector3("uViewPos", mainCamera->GetPosition());
		
		if (player)
		player->Render();

		for (const std::unique_ptr<Mountain>& mountain : mountains)
		{
			mountain->Render();
		}
	}
}

void OnClose() noexcept
{
	mainCamera.reset();
	subCamera.reset();

	player.reset();
	mountains.clear();
}

void Reset() noexcept
{
	
}

void GetUserInput() noexcept
{
	std::system("cls");

	while (true)
	{
		std::cout << std::format("가로/세로 길이를 입력해주세요.\n");
		std::cout << std::format("※ 길이는 {}에서 {} 사이의 정수로 입력해주세요.\n", MIN_LENGTH, MAX_LENGTH);
		std::cout << std::format("※ 종료를 원하신다면 'q'/'Q'를 입력해주세요.\n");
		std::cout << std::format("==>> ");

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
	std::cout << "===== 조작 방법 =====\n";
	std::cout << "o / p : 투영 선택 (직교 / 원근)\n";
	std::cout << "z / Z : 원근 투영 시 z축 이동\n";
	std::cout << "m / M : 육면체 위아래 움직임 / 멈춤\n";
	std::cout << "+ / - : 육면체 이동 속도 증가 / 감소\n";
	std::cout << "y / Y : 카메라가 바닥 y축 기준으로 회전\n";
	std::cout << "r     : 미로 생성\n";
	std::cout << "v     : 육면체 움직임 정지, 낮은 높이로 변경 (토글)\n";
	std::cout << "s     : 미로에서 객체 등장\n";
	std::cout << "↑/↓/←/→ : 미로에서 객체 이동\n";
	std::cout << "1 / 3 : 1인칭 / 3인칭 시점 전환\n";
	std::cout << "c     : 모든 값 초기화\n";
	std::cout << "q     : 프로그램 종료\n";
	std::cout << "=====================\n";
}