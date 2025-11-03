#include <spdlog/spdlog.h>

#include "Rendering/Camera.h"
#include "Rendering/Mesh.h"
#include "Rendering/Shader.h"

#include "Core/Application.h"
#include "Core/File.h"
#include "Core/Input.h"
#include "Core/Time.h"

#include "Objects/Axes.h"
#include "Objects/Planet.h"

enum class SelectedObject
{
    /**
     * @brief 아무 것도 선택되지 않음.
     */
    None,

    /**
     * @brief 첫 번째 오브젝트 선택.
     */
    Lhs,

    /**
     * @brief 두 번째 오브젝트 선택.
     */
    Rhs,

    /**
     * @brief 모든 오브젝트 선택.
     */
    All
};

/**
 * @brief 애플리케이션이 시작될 때 호출됩니다.
 */
static void OnStart() noexcept;

/**
 * @brief 매 프레임마다 호출됩니다.
 */
static void OnUpdate() noexcept;

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
static constexpr const char* const APPLICATION_NAME = "Level 01 - Act 18";

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
 * @brief 해당 애플리케이션에서 사용할 셰이더.
 */
static std::unique_ptr<Shader> shader = nullptr;

/**
 * @brief 해당 애플리케이션에서 사용할 카메라.
 */
static std::unique_ptr<Camera> camera = nullptr;

/**
 * @brief 해당 애플리케이션의 좌표축.
 */
static std::unique_ptr<Object> axes = nullptr;

/**
 * @brief 해당 애플리케이션에서 시뮬레이션할 첫 번째 오브젝트.
 */
static std::unique_ptr<Planet> lhs = nullptr;

/**
 * @brief 해당 애플리케이션에서 시뮬레이션할 두 번째 오브젝트.
 */
static std::unique_ptr<Planet> rhs = nullptr;

/**
 * @brief 현재 선택된 오브젝트.
 */
static SelectedObject selectedObject = SelectedObject::None;

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
    specification.onUpdate                   = OnUpdate;
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

    constexpr glm::vec3 position    = {-5.0f, 3.0f, -5.0f};
    constexpr glm::vec3 front       = {5.0f, -3.0f, 5.0f};
    constexpr glm::vec3 up          = {0.0f, 1.0f, 0.0f};
    const     float     aspectRatio = static_cast<float>(Application::GetSpecification().width) /
                                      static_cast<float>(Application::GetSpecification().height);

    camera = std::make_unique<Camera>(position, front, up);
    camera->SetProjection(Camera::ProjectionType::Perspective);
    camera->SetAspectRatio(aspectRatio);

    axes = std::make_unique<Object>();
    axes->SetScale({10.0f, 10.0f, 10.0f});
    axes->SetMesh(Mesh::LoadFrom("Resources/Models/Axes.obj"));

    lhs = std::make_unique<Planet>();
    lhs->SetPosition({2.0f, 0.0f, 0.0f});
    lhs->SetRotation({30.0f, 30.0f, 0.0f});
    lhs->SetMesh(Mesh::LoadFrom("Resources/Models/Cube.obj"));

    rhs = std::make_unique<Planet>();
    rhs->SetPosition({-2.0f, 0.0f, 0.0f});
    rhs->SetRotation({30.0f, 30.0f, 0.0f});
    rhs->SetMesh(Mesh::LoadFrom("Resources/Models/Cone.obj"));
}

static void OnUpdate() noexcept
{
    if (Input::IsKeyPressed('1'))
    {
        lhs->SetSelected(true);
        rhs->SetSelected(false);
    }
    else if (Input::IsKeyPressed('2'))
    {
        lhs->SetSelected(false);
        rhs->SetSelected(true);
    }
    else if (Input::IsKeyPressed('3'))
    {
        lhs->SetSelected(true);
        rhs->SetSelected(true);
    }
    else if (Input::IsKeyPressed('0'))
    {
        lhs->SetSelected(false);
        rhs->SetSelected(false);
    }

    if (Input::IsKeyPressed('c'))
    {
        lhs->SetMesh(Mesh::LoadFrom("Resources/Models/Cylinder.obj"));
        rhs->SetMesh(Mesh::LoadFrom("Resources/Models/Sphere.obj"));
    }

    if (Input::IsKeyPressed('s'))
    {
        axes.reset();
        axes = std::make_unique<Object>();
        axes->SetScale({10.0f, 10.0f, 10.0f});
        axes->SetMesh(Mesh::LoadFrom("Resources/Models/Axes.obj"));

        lhs.reset();
        lhs = std::make_unique<Planet>();
        lhs->SetPosition({2.0f, 0.0f, 0.0f});
        lhs->SetRotation({30.0f, 30.0f, 0.0f});
        lhs->SetMesh(Mesh::LoadFrom("Resources/Models/Sphere.obj"));

        rhs.reset();
        rhs = std::make_unique<Planet>();
        rhs->SetPosition({-2.0f, 0.0f, 0.0f});
        rhs->SetRotation({30.0f, 30.0f, 0.0f});
        rhs->SetMesh(Mesh::LoadFrom("Resources/Models/Cone.obj"));
    }

    if (Input::IsKeyPressed('q'))
    {
        Application::Quit();
    }

    rhs->Update();
    lhs->Update();
}

static void OnDisplay() noexcept
{
    if (camera != nullptr)
    {
        camera->PreRender(*shader);
    }

    if (axes != nullptr)
    {
        axes->Render(*shader);
    }

    if (lhs != nullptr)
    {
        lhs->Render(*shader);
    }

    if (rhs != nullptr)
    {
        rhs->Render(*shader);
    }
}