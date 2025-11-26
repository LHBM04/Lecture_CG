#include "Mountain.h"
#include "Input.h"
#include "Transform.h"
#include "Random.h"
#include "Resources.h"

Mountain::Mountain(const bool include_) noexcept
    : Object(Transform(glm::vec3(0.0f),
                       glm::vec3(0.0f),
                       glm::vec3(1.0f, 1.0f, 1.0f),
                       nullptr),
             Resources::GetMesh("Resources/Meshes/Mountain.obj"))
    , height(Random::GetFloat(3.0f, 7.0f))
    , animationSpeed(Random::GetFloat(2.0f, 10.0f))
    , isIncluded(include_)
    , currentState(AnimationState::Default)
    , shouldPause(false)
{
    
}

Mountain::~Mountain() noexcept
{
}

void Mountain::OnUpdate(const float deltaTime_) noexcept
{
    if (Input::IsKeyPressed(GLFW_KEY_M))
    {
        if (Input::IsModified(GLFW_MOD_SHIFT))
        {
            shouldPause = true;
        }
        else
        {
            shouldPause  = false;
            currentState = AnimationState::AnimationM;
        }
    }
    else if (Input::IsKeyPressed(GLFW_KEY_R))
    {
        if (isIncluded)
        {
            currentState = AnimationState::AnimationR;
        }
    }
    else if (Input::IsKeyPressed(GLFW_KEY_V))
    {
        currentState = AnimationState::AnimationV;
    }

    glm::vec3 currentScale = GetScale();
    float     newScaleY    = currentScale.y;

    switch (currentState)
    {
        case AnimationState::Default:
        {
            if (newScaleY < height)
            {
                newScaleY += deltaTime_ * animationSpeed;
                
                if (newScaleY > height)
                {
                    newScaleY = height;
                }
            }
            break;
        }
        case AnimationState::AnimationM:
        {
            if (shouldPause)
            {
                return;
            }

            if (shouldGrow)
            {
                newScaleY += deltaTime_ * animationSpeed;
                if (newScaleY >= height)
                {
                    newScaleY = height;
                    shouldGrow = false;
                }
            }
            else
            {
                newScaleY -= deltaTime_ * animationSpeed;
                if (newScaleY <= 0.0f) 
                {
                    newScaleY = 0.0f;
                    shouldGrow = true;
                }
            }
            break;
        }

        case AnimationState::AnimationR:
        {
            constexpr float targetHeight = 0.05f;

            if (newScaleY > targetHeight)
            {
                newScaleY -= deltaTime_ * animationSpeed;
                if (newScaleY < targetHeight) newScaleY = targetHeight;
            }
            else if (newScaleY < targetHeight)
            {
                newScaleY += deltaTime_ * animationSpeed;
                if (newScaleY > targetHeight) newScaleY = targetHeight;
            }
            break;
        }
        case AnimationState::AnimationV:
        {
            constexpr float targetHeight = 3.0f;

            if (std::abs(newScaleY - targetHeight) > 0.01f)
            {
                float dir = (targetHeight - newScaleY > 0) ? 1.0f : -1.0f;
                newScaleY += dir * deltaTime_ * animationSpeed;

                if ((dir > 0 && newScaleY > targetHeight) || 
                    (dir < 0 && newScaleY < targetHeight))
                {
                    newScaleY = targetHeight;
                }
            }
            else
            {
                newScaleY = targetHeight;
            }
            break;
        }
    }

    const float newYPos = (newScaleY * 0.5f);

    SetScale(glm::vec3(1.0f, newScaleY, 1.0f));
    SetPosition(glm::vec3(GetPosition().x, newYPos, GetPosition().z));
}