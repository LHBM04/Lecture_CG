#include <spdlog/spdlog.h>

#include "Rendering/Camera.h"
#include "Rendering/Mesh.h"
#include "Rendering/Shader.h"

#include "Core/Application.h"
#include "Core/File.h"
#include "Core/Input.h"
#include "Core/Time.h"

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

/**
 * @brief 모든 행성의 공전 궤도 Z축 기울기 속도.
 */
static constexpr float ORBIT_TILT_SPEED = 30.0f; // 초당 30도

static GLenum renderMode = GL_TRIANGLES;

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
    specification.onRender                  = OnDisplay;

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
    sun->SetColor(glm::vec3(0.0f, 0.0f, 1.0f));

    Object* earth1 = planets.emplace_back(std::make_unique<Planet>(Planet::Properties{sun, 2.5f, -45.0f, 25.0f})).get();
    earth1->SetPosition(glm::vec3(-6.0f, 0.0f, 6.0f));
    earth1->SetScale(glm::vec3(1.0f, 1.0f, 1.0f));
    earth1->SetColor(glm::vec3(0.0f, 1.0f, 0.0f));

    Object* moon1 = planets.emplace_back(std::make_unique<Planet>(Planet::Properties{earth1, 1.0f, -45.0f, 50.0f})).get();
    moon1->SetPosition(glm::vec3(3.0f, -3.0f, 0.0f));
    moon1->SetScale(glm::vec3(0.5f, 0.5f, 0.5f));
    moon1->SetColor(glm::vec3(1.0f, 0.0f, 0.0f));
}

static void OnUpdate() noexcept
{
    if (Input::IsKeyPressed('p'))
    {
        camera->SetProjection(Camera::ProjectionType::Orthographic);
    }
    else if (Input::IsKeyPressed('P'))
    {
        camera->SetProjection(Camera::ProjectionType::Perspective);
    }

    if (Input::IsKeyPressed('m'))
    {
        renderMode = GL_TRIANGLES;
    }
    else if (Input::IsKeyPressed('M'))
    {
        renderMode = GL_LINES;
    }

    if (Input::IsKeyHeld('w'))
    {
        const glm::vec3 forward = glm::normalize(camera->GetUp());
        camera->SetPosition(camera->GetPosition() - forward * 4.0f * Time::GetDeltaTime());
    }
    if (Input::IsKeyHeld('s'))
    {
        const glm::vec3 forward = glm::normalize(camera->GetUp());
        camera->SetPosition(camera->GetPosition() + forward * 4.0f * Time::GetDeltaTime());
    }
    if (Input::IsKeyHeld('a'))
    {
        const glm::vec3 right = glm::normalize(glm::cross(camera->GetFront(), camera->GetUp()));
        camera->SetPosition(camera->GetPosition() + right * 4.0f * Time::GetDeltaTime());
    }
    if (Input::IsKeyHeld('d'))
    {
        const glm::vec3 right = glm::normalize(glm::cross(camera->GetFront(), camera->GetUp()));
        camera->SetPosition(camera->GetPosition() - right * 4.0f * Time::GetDeltaTime());
    }

    if (Input::IsKeyHeld('+'))
    {
        const glm::vec3 forward = glm::normalize(camera->GetFront());
        camera->SetPosition(camera->GetPosition() + forward * 4.0f * Time::GetDeltaTime());
    }
    if (Input::IsKeyHeld('-'))
    {
        const glm::vec3 forward = glm::normalize(camera->GetFront());
        camera->SetPosition(camera->GetPosition() - forward * 4.0f * Time::GetDeltaTime());
    }

    if (Input::IsKeyHeld('y'))
    {
        for (const std::unique_ptr<Planet>& planet : planets)
        {
            planet->SetInfo(Planet::Properties{
                planet->GetInfo().parent,
                planet->GetInfo().distance + 1.0f * Time::GetDeltaTime(),
                planet->GetInfo().angle,
                planet->GetInfo().speed
            });
        }
    }
    else if (Input::IsKeyHeld('Y'))
    {
        for (const std::unique_ptr<Planet>& planet : planets)
        {
            planet->SetInfo(Planet::Properties{
                planet->GetInfo().parent,
                planet->GetInfo().distance - 1.0f * Time::GetDeltaTime(),
                planet->GetInfo().angle,
                planet->GetInfo().speed
            });
        }
    }

    if (Input::IsKeyHeld('z'))
    {
        Planet::s_OrbitTiltZ += ORBIT_TILT_SPEED * Time::GetDeltaTime();
    }
    else if (Input::IsKeyHeld('Z'))
    {
        Planet::s_OrbitTiltZ -= ORBIT_TILT_SPEED * Time::GetDeltaTime();
    }

    if (Input::IsKeyPressed('q') ||
        Input::IsKeyPressed('Q'))
    {
        Application::Quit();
    }

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
        planet->Render(*shader, renderMode);
    }
}