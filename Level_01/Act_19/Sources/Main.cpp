#include <spdlog/spdlog.h>

#include "Core/Application.h"
#include "Core/File.h"
#include "Core/Input.h"
#include "Core/Time.h"

#include "Objects/Axes.h"
#include "Objects/Cone.h"
#include "Objects/Cube.h"

#include "World/World.h"

/**
 * @brief 애플리케이션이 시작될 때 호출됩니다.
 */
static void OnStart() noexcept;

/**
 * @brief 고정된 시간 간격으로 호출됩니다.
 */
static void OnFixedUpdate() noexcept;

/**
 * @brief 매 프레임마다 호출됩니다.
 */
static void OnUpdate() noexcept;

/**
 * @brief 프레임 렌더링이 끝난 후 호출됩니다.
 */
static void OnLateUpdate() noexcept;

/**
 * @brief 윈도우가 그려질 때 호출됩니다.
 */
static void OnDisplay() noexcept;

/**
 * @brief 애플리케이션 주버전.
 */
static constexpr int MAJOR_VERSION = 4;

/**
 * @brief 애플리케이션 부버전.
 */
static constexpr int MINOR_VERSION = 6;

/**
 * @brief 애플리케이션 이름.
 */
static constexpr const char* const APPLICATION_NAME = "Level 01 - Act 19";

/**
 * @brief 애플리케이션 너비.
 */
static constexpr int APPLICATION_WIDTH = 1280;

/**
 * @brief 애플리케이션 높이.
 */
static constexpr int APPLICATION_HEIGHT = 720;

/**
 * @brief 초당 프레임 수.
 */
static constexpr int FPS = 60;

/**
 * @brief 해당 애플리케이션에서 시뮬레이션할 월드.
 */
static std::unique_ptr<World> world = nullptr;

/**
 * @brief 해당 애플리케이션에서 사용할 셰이더.
 */
static std::unique_ptr<Shader> shader = nullptr;

int main()
{
    Application::Specification specification = {};
    specification.majorVersion               = MAJOR_VERSION;
    specification.minorVersion               = MINOR_VERSION;
    specification.name                       = APPLICATION_NAME;
    specification.width                      = APPLICATION_WIDTH;
    specification.height                     = APPLICATION_HEIGHT;
    specification.fps                        = FPS;
    specification.onStart                    = OnStart;
    specification.onFixedUpdate              = OnFixedUpdate;
    specification.onUpdate                   = OnUpdate;
    specification.onLateUpdate               = OnLateUpdate;
    specification.onDisplay                  = OnDisplay;

    return Application::Run(specification);
}

static void OnStart() noexcept
{
    const std::string vertexShaderFile   = File::ReadFile("Resources/Shaders/Vertex.glsl");
    const char* const vertexShaderSource = vertexShaderFile.c_str();

    const std::string fragmentShaderFile   = File::ReadFile("Resources/Shaders/Fragment.glsl");
    const char* const fragmentShaderSource = fragmentShaderFile.c_str();

    shader = std::make_unique<Shader>(vertexShaderSource, fragmentShaderSource);
    shader->Use();

    world = std::make_unique<World>();
    world->AddObject(std::make_unique<Axes>());
    world->AddObject(std::make_unique<Cube>());
    // world->AddObject(std::make_unique<Cone>());
}

static void OnFixedUpdate() noexcept
{
    world->FixedUpdate();
}

static void OnUpdate() noexcept
{
    world->Update();
}

static void OnLateUpdate() noexcept
{
    world->LateUpdate();
}

static void OnDisplay() noexcept
{
    world->Render(*shader);
}