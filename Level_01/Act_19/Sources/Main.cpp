#include <memory>

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
static void OnRender() noexcept;

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
static std::unique_ptr<Shader> shader = nullptr;

/**
 * @brief 해당 애플리케이션에서 사용할 카메라.
 */
static std::unique_ptr<Camera> camera = nullptr;

/**
 * @brief 해당 애플리케이션에서 시뮬레이션 할 행성 오브젝트들.
 */
static std::vector<std::unique_ptr<Planet>> planets;

/**
 * @brief 행성들의 공전 궤도 경로.
 */
static std::vector<std::vector<std::unique_ptr<Object>>> pathes;

/**
 * @brief 모든 행성의 공전 궤도 Z축 기울기 속도.
 */
static constexpr float ORBIT_TILT_SPEED = 30.0f; // 초당 30도

/**
 * @brief 렌더 모드.
 */
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
    specification.onRender                  = OnRender;

    return Application::Run(specification);
}

static void OnStart() noexcept
{
    const std::string vertexShaderFile = File::ReadFile("Resources/Shaders/Vertex.glsl");
    const char* const vertexShaderSource = vertexShaderFile.c_str();

    const std::string fragmentShaderFile = File::ReadFile("Resources/Shaders/Fragment.glsl");
    const char* const fragmentShaderSource = fragmentShaderFile.c_str();

    shader = std::make_unique<Shader>(vertexShaderSource, fragmentShaderSource);
    shader->Use();

    constexpr glm::vec3 position    = { 0.0f, 0.0f,  10.0f };
    constexpr glm::vec3 front       = { 0.0f, 0.0f, -1.0f };
    constexpr glm::vec3 up          = { 0.0f, 1.0f,  0.0f };
    const     float     aspectRatio = static_cast<float>(Application::GetSpecification().width) / static_cast<float>(Application::GetSpecification().height);

    camera = std::make_unique<Camera>(position, front, up);
    camera->SetProjection(Camera::ProjectionType::Perspective);
    camera->SetAspectRatio(aspectRatio);

    Planet* sun = planets.emplace_back(std::make_unique<Planet>()).get();
    sun->SetScale(glm::vec3(2.5f, 2.5f, 2.5f));
    sun->SetColor(glm::vec3(0.0f, 0.0f, 1.0f));

    Planet* earth1 = planets.emplace_back(std::make_unique<Planet>(sun, 2.5f, 0.0f, 50.0f, 90.0f)).get();
    earth1->SetScale(glm::vec3(1.0f, 1.0f, 1.0f));
    earth1->SetColor(glm::vec3(0.0f, 1.0f, 0.0f));
    
	Planet* moon1 = planets.emplace_back(std::make_unique<Planet>(earth1, 1.0f, -45.0f, 100.0f, 90.0f)).get();
	moon1->SetScale(glm::vec3(0.5f, 0.5f, 0.5f));
	moon1->SetColor(glm::vec3(1.0f, 0.0f, 0.0f));

    Planet* earth2 = planets.emplace_back(std::make_unique<Planet>(sun, 2.5f, -90.0f, 60.0f, 90 - 45.0f)).get();
    earth2->SetScale(glm::vec3(1.0f, 1.0f, 1.0f));
    earth2->SetColor(glm::vec3(0.0f, 1.0f, 0.0f));

    Planet* moon2 = planets.emplace_back(std::make_unique<Planet>(earth2, 1.0f, -45.0f, 100.0f, 180.0f)).get();
    moon2->SetScale(glm::vec3(0.5f, 0.5f, 0.5f));
    moon2->SetColor(glm::vec3(1.0f, 0.0f, 0.0f));

    Planet* earth3 = planets.emplace_back(std::make_unique<Planet>(sun, 2.5f, -180.0f, 70.0f, 90 + 45.0f)).get();
    earth3->SetScale(glm::vec3(1.0f, 1.0f, 1.0f));
    earth3->SetColor(glm::vec3(0.0f, 1.0f, 0.0f));

    Planet* moon3 = planets.emplace_back(std::make_unique<Planet>(earth3, 1.0f, -45.0f, 100.0f, 270.0f)).get();
    moon3->SetScale(glm::vec3(0.5f, 0.5f, 0.5f));
    moon3->SetColor(glm::vec3(1.0f, 0.0f, 0.0f));

    pathes.resize(planets.size());
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
        camera->SetPosition(camera->GetPosition() + forward * 4.0f * Time::GetDeltaTime());
    }
    if (Input::IsKeyHeld('s'))
    {
        const glm::vec3 forward = glm::normalize(camera->GetUp());
        camera->SetPosition(camera->GetPosition() - forward * 4.0f * Time::GetDeltaTime());
    }
    if (Input::IsKeyHeld('a'))
    {
        const glm::vec3 right = glm::normalize(glm::cross(camera->GetForward(), camera->GetUp()));
        camera->SetPosition(camera->GetPosition() + right * 4.0f * Time::GetDeltaTime());
    }
    if (Input::IsKeyHeld('d'))
    {
        const glm::vec3 right = glm::normalize(glm::cross(camera->GetForward(), camera->GetUp()));
        camera->SetPosition(camera->GetPosition() - right * 4.0f * Time::GetDeltaTime());
    }

    if (Input::IsKeyHeld('+'))
    {
        const glm::vec3 forward = glm::normalize(camera->GetForward());
        camera->SetPosition(camera->GetPosition() + forward * 4.0f * Time::GetDeltaTime());
    }
    if (Input::IsKeyHeld('-'))
    {
        const glm::vec3 forward = glm::normalize(camera->GetForward());
        camera->SetPosition(camera->GetPosition() - forward * 4.0f * Time::GetDeltaTime());
    }

    if (Input::IsKeyHeld('y'))
    {
        for (const std::unique_ptr<Planet>& planet : planets)
        {
			const float distance = planet->GetDistance();
            planet->SetDistance(distance + 2.5f * Time::GetDeltaTime());
        }
    }
    else if (Input::IsKeyHeld('Y'))
    {
        for (const std::unique_ptr<Planet>& planet : planets)
        {
            const float distance = planet->GetDistance();
            planet->SetDistance(distance - 2.5f * Time::GetDeltaTime());
        }
    }

    if (Input::IsKeyHeld('z'))
    {
        for (const std::unique_ptr<Planet>& planet : planets)
        {
            const float tilt = planet->GetTilt();
            planet->SetTilt(tilt + 12.5f * Time::GetDeltaTime());
        }
    }
    else if (Input::IsKeyHeld('Z'))
    {
        for (const std::unique_ptr<Planet>& planet : planets)
        {
            const float tilt = planet->GetTilt();
            planet->SetTilt(tilt - 12.5f * Time::GetDeltaTime());
        }
    }

    if (Input::IsKeyPressed('`'))
    {
        Application::Quit();
    }

	for (std::size_t index = 0; index < planets.size(); ++index)
    {
        planets[index]->Update();

        //Object* pathObj = pathes[index].emplace_back(std::make_unique<Object>()).get();
		//pathObj->SetPosition(planets[index]->GetPosition());
        //pathObj->SetScale(glm::vec3(0.1f, 0.1f, 0.1f));
        //pathObj->SetColor(glm::vec3(0.0f, 0.0f, 0.0f));
		//pathObj->SetMesh(Mesh::LoadFrom("Resources/Models/Sphere.obj"));
    }

    for (auto& path : pathes)
    {
        if (path.size() > 10)
        {
			path.erase(path.begin());
        }
    }
}

static void OnRender() noexcept
{
    if (camera != nullptr)
    {
        camera->PreRender(*shader);
    }

    for (const std::unique_ptr<Planet>& planet : planets)
    {
        planet->Render(*shader, renderMode);
    }

    for (const auto& path : pathes)
    {
        for (const auto& point : path)
        {
            point->Render(*shader, renderMode);
        }
	}
}