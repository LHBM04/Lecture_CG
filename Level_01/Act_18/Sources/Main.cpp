#include <spdlog/spdlog.h>
#include <algorithm> // std::swap을 위해 추가

#include "Rendering/Camera.h"
#include "Rendering/Mesh.h"
#include "Rendering/Shader.h"

#include "Core/Application.h"
#include "Core/File.h"
#include "Core/Input.h"
#include "Core/Time.h"

#include "Objects/Axes.h"
#include "Objects/Planet.h"


/**
 * @brief 현재 재생 중인 애니메이션 상태.
 */
enum class AnimationState
{
    None,
    T_Swap,
    U_Swap,
    V_Swap
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
static Shader* shader = nullptr;

/**
 * @brief 해당 애플리케이션에서 사용할 카메라.
 */
static std::unique_ptr<Camera> camera = nullptr;

/**
 * @brief 해당 애플리케이션의 좌표축.
 */
static std::unique_ptr<Axes> axes = nullptr;

/**
 * @brief 해당 애플리케이션에서 시뮬레이션할 첫 번째 오브젝트.
 */
static std::unique_ptr<Planet> lhs = nullptr;

/**
 * @brief 해당 애플리케이션에서 시뮬레이션할 두 번째 오브젝트.
 */
static std::unique_ptr<Planet> rhs = nullptr;

/**
 * @brief 현재 재생 중인 애니메이션 상태.
 */
static AnimationState currentAnimation = AnimationState::None;

/**
 * @brief 애니메이션 경과 시간.
 */
static float animationTimer = 0.0f;

/**
 * @brief 애니메이션 총 재생 시간 (초).
 */
static constexpr float ANIMATION_DURATION = 1.5f;

static glm::vec3 lhsStartPosition;
static glm::vec3 rhsStartPosition;
static glm::vec3 lhsStartScale;
static glm::vec3 rhsStartScale;

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
    shader = Shader::Create("");
    shader->Use();

    constexpr glm::vec3 position    = {-5.0f, 3.0f, -5.0f};
    constexpr glm::vec3 front       = {5.0f, -3.0f, 5.0f};
    constexpr glm::vec3 up          = {0.0f, 1.0f, 0.0f};
    const     float     aspectRatio = static_cast<float>(Application::GetSpecification().width) /
                                      static_cast<float>(Application::GetSpecification().height);

    camera = std::make_unique<Camera>(position, front, up);
    camera->SetProjection(Camera::ProjectionType::Perspective);
    camera->SetAspectRatio(aspectRatio);

    axes = std::make_unique<Axes>();
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

    lhsStartPosition = lhs->GetPosition();
    rhsStartPosition = rhs->GetPosition();
}

static void OnUpdate() noexcept
{
    static glm::vec3 lhsTargetPosition = rhs->GetPosition();
    static glm::vec3 rhsTargetPosition = lhs->GetPosition();

    if (Input::IsKeyHeld('t'))
    {
        {
            glm::vec3 currentPosition = lhs->GetPosition();

            if (lhsTargetPosition == currentPosition)
            {
                lhsTargetPosition = {};
                return;
            }

            const glm::vec3 direction = glm::normalize(lhsTargetPosition - currentPosition);
            lhs->SetPosition(currentPosition + direction * 5.0f * Time::GetDeltaTime());
        }
        {
            glm::vec3 currentPosition = rhs->GetPosition();

            if (rhsTargetPosition == currentPosition)
            {
                rhsTargetPosition = {};
                return;
            }

            const glm::vec3 direction = glm::normalize(rhsTargetPosition - currentPosition);
            rhs->SetPosition(currentPosition + direction * 5.0f * Time::GetDeltaTime());
        }
    }
    if (Input::IsKeyHeld('u'))
    {
        lhsTargetPosition = rhs->GetPosition();
        rhsTargetPosition = lhs->GetPosition();

        {
            const glm::vec3 currentPos   = lhs->GetPosition();
            const float     radius       = glm::length(glm::vec2(currentPos.x, currentPos.y));
            const float     currentAngle = atan2(currentPos.y, currentPos.x);

            const float angleDelta = glm::radians(50.0f) * Time::GetDeltaTime();
            const float newAngle   = currentAngle + angleDelta;

            glm::vec3 nextPosition = currentPos;
            nextPosition.x = radius * cos(newAngle);
            nextPosition.y = radius * sin(newAngle);

            lhs->SetPosition(nextPosition);
        }
        {
            const glm::vec3 currentPos   = rhs->GetPosition();
            const float     radius       = glm::length(glm::vec2(currentPos.x, currentPos.y));
            const float     currentAngle = atan2(currentPos.y, currentPos.x);

            const float angleDelta = glm::radians(50.0f) * Time::GetDeltaTime();
            const float newAngle   = currentAngle + angleDelta;

            glm::vec3 nextPosition = currentPos;
            nextPosition.x = radius * cos(newAngle);
            nextPosition.y = radius * sin(newAngle);

            rhs->SetPosition(nextPosition);
        }

        return;
    }
    if (Input::IsKeyHeld('v'))
    {
        lhsTargetPosition = rhs->GetPosition();
        rhsTargetPosition = lhs->GetPosition();

        // lhs
        {
            glm::vec3 scale = lhs->GetScale();
            scale += glm::vec3{1.0f, 1.0f, 1.0f} * 0.05f * Time::GetDeltaTime();
            lhs->SetScale(scale);

            const glm::vec3 currentPos   = lhs->GetPosition();
            const float     radius       = glm::length(glm::vec2(currentPos.x, currentPos.y));
            const float     currentAngle = atan2(currentPos.y, currentPos.x);

            const float angleDelta = glm::radians(50.0f) * Time::GetDeltaTime();
            const float newAngle   = currentAngle + angleDelta;

            glm::vec3 nextPosition = currentPos;
            nextPosition.x = radius * cos(newAngle);
            nextPosition.y = radius * sin(newAngle);

            lhs->SetPosition(nextPosition);

            const glm::vec3 original = lhs->GetRotation();
            const glm::vec3 rotate   = original + ((glm::vec3{0.0f, 1.0f, 0.0f} * 5.0f) * Time::GetDeltaTime());

            lhs->SetRotation(rotate);
        }
        // rhs
        {
            glm::vec3 scale = rhs->GetScale();
            scale += glm::vec3{-1.0f, -1.0f, -1.0f} * 0.05f * Time::GetDeltaTime();
            rhs->SetScale(scale);

            const glm::vec3 currentPos   = rhs->GetPosition();
            const float     radius       = glm::length(glm::vec2(currentPos.x, currentPos.y));
            const float     currentAngle = atan2(currentPos.y, currentPos.x);

            const float angleDelta = glm::radians(50.0f) * Time::GetDeltaTime();
            const float newAngle   = currentAngle + angleDelta;

            glm::vec3 nextPosition = currentPos;
            nextPosition.x = radius * cos(newAngle);
            nextPosition.y = radius * sin(newAngle);

            rhs->SetPosition(nextPosition);

            const glm::vec3 original = rhs->GetRotation();
            const glm::vec3 rotate   = original + ((glm::vec3{0.0f, 1.0f, 0.0f} * 5.0f) * Time::GetDeltaTime());

            rhs->SetRotation(rotate);
        }
    }

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
        lhs.reset();
        lhs = std::make_unique<Planet>();
        lhs->SetPosition({2.0f, 0.0f, 0.0f});
        lhs->SetRotation({30.0f, 30.0f, 0.0f});
        lhs->SetMesh(Mesh::LoadFrom("Resources/Models/Cube.obj"));

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

static void OnRender() noexcept
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