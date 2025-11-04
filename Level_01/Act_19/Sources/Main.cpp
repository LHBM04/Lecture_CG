#include <spdlog/spdlog.h>

#include "Rendering/Camera.h"
#include "Rendering/Mesh.h"
#include "Rendering/Shader.h"

#include "Core/Application.h"
#include "Core/File.h"
#include "Core/Input.h"

#include "Objects/Planet.h"

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
 * @brief 해당 애플리케이션에서 사용할 셰이더.
 */
static Shader* shader = nullptr;

/**
 * @brief 해당 애플리케이션에서 사용할 카메라.
 */
static std::unique_ptr<Camera> camera = nullptr;

/**
 * @brief 해당 애플리케이션에서 시뮬레이션 할 행성 오브젝트들.
 */
static std::vector<std::unique_ptr<Planet>> planets;

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
    shader = Shader::LoadFrom("");
    shader->Use();

    constexpr glm::vec3 position    = {-5.0f, 3.0f, -5.0f};
    constexpr glm::vec3 front       = {5.0f, -3.0f, 5.0f};
    constexpr glm::vec3 up          = {0.0f, 1.0f, 0.0f};
    const     float     aspectRatio = static_cast<float>(Application::GetSpecification().width) /
                                      static_cast<float>(Application::GetSpecification().height);

    camera = std::make_unique<Camera>(position, front, up);
    camera->SetProjection(Camera::ProjectionType::Perspective);
    camera->SetAspectRatio(aspectRatio);

    Object* sun = planets.emplace_back(std::make_unique<Planet>(Planet::Properties{nullptr, 0, 0, 0})).get();
    sun->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
    sun->SetScale(glm::vec3(2.5f, 2.5f, 2.5f));

    Object* earth1 = planets.emplace_back(std::make_unique<Planet>(Planet::Properties{sun, 2.5f, 45.0f, 5.0f})).get();
    earth1->SetPosition(glm::vec3(-3.0f, -3.0f, 0.0f));
    earth1->SetScale(glm::vec3(1.0f, 1.0f, 1.0f));

    Object* moon1 = planets.emplace_back(std::make_unique<Planet>(Planet::Properties{earth1, 1.5f, 45.0f, 15.0f})).get();
    moon1->SetPosition(glm::vec3(3.0f, -3.0f, 0.0f));
    moon1->SetScale(glm::vec3(0.5f, 0.5f, 0.5f));

    Object* earth2 = planets.emplace_back(std::make_unique<Planet>(Planet::Properties{sun, 2.5f, 45.0f, 5.0f})).get();
    earth2->SetPosition(glm::vec3(-4.0f, 0.0f, 0.0f));
    earth2->SetScale(glm::vec3(1.0f, 1.0f, 1.0f));

    Object* moon2 = planets.emplace_back(std::make_unique<Planet>(Planet::Properties{moon1, 2.5f, 45.0f, 5.0f})).get();
    moon2->SetPosition(glm::vec3(-1.5f, 0.0f, 0.0f));
    moon2->SetScale(glm::vec3(0.5f, 0.5f, 0.5f));
}

static void OnUpdate() noexcept
{
    for (const std::unique_ptr<Planet>& planet : planets)
    {
        planet->Update();
    }
}

static void OnDisplay() noexcept
{
    if (camera != nullptr)
    {
        camera->PreRender(*shader);
    }

    for (const std::unique_ptr<Planet>& planet : planets)
    {
        planet->Render(*shader);
    }
}