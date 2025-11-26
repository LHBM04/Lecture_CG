#include "Player.h"

#include "Input.h"

#include "Resources.h"

Player::Player() noexcept
	: Object(Transform(glm::vec3(0.0f, 0.0f, 0.0f),
					   glm::vec3(0.0f, 0.0f, 0.0f),
					   glm::vec3(0.25f, 0.25f, 0.25f),
					   nullptr),
			 Resources::GetMesh("Resources/Meshes/Player.obj"))
	, moveSpeed(DEFAULT_MOVE_SPEED)
    , jumpForce(DEFAULT_JUMP_FORCE)
	, direction(glm::vec3(0.0f))
	, verticalVelocity(0.0f)
{

}

void Player::OnUpdate(const float deltaTime_) noexcept
{
    // 타일 위에 왜 안 올라갈까
    // 그냥 하드 코딩함
    const float tileHeight = 0.1f;
    const float halfHeight = GetScale().y * 0.5f;
    const float groundY = tileHeight + halfHeight;

#pragma region Control Direction
    if (Input::IsKeyHeld(GLFW_KEY_UP))
    {
        direction.z -= 1.0f;
    }
    if (Input::IsKeyHeld(GLFW_KEY_DOWN))
    {
        direction.z += 1.0f;
    }
    if (Input::IsKeyHeld(GLFW_KEY_LEFT))
    {
        direction.x -= 1.0f;
    }
    if (Input::IsKeyHeld(GLFW_KEY_RIGHT))
    {
        direction.x += 1.0f;
    }
    if (Input::IsKeyPressed(GLFW_KEY_SPACE))
    {
        if (GetPosition().y <= groundY + 0.01f)
        {
            verticalVelocity = jumpForce;
        }
    }
#pragma endregion

#pragma region Control Speed/Force
    if (Input::IsKeyHeld(GLFW_KEY_KP_ADD))
    {
        moveSpeed = glm::min(MAX_PLAYER_SPEED, moveSpeed + 5.0f * deltaTime_);
        jumpForce = glm::min(MAX_PLAYER_SPEED, jumpForce + 1.0f * deltaTime_);
    }
    if (Input::IsKeyHeld(GLFW_KEY_KP_SUBTRACT))
    {
		moveSpeed = glm::max(0.0f, moveSpeed - 5.0f * deltaTime_);
		jumpForce = glm::max(0.0f, jumpForce - 1.0f * deltaTime_);
    }
    if (Input::IsKeyHeld(GLFW_KEY_EQUAL))
    {
        moveSpeed = DEFAULT_MOVE_SPEED;
		jumpForce = DEFAULT_JUMP_FORCE;
    }
#pragma endregion

    glm::vec3 position = GetPosition();
    glm::vec3 rotation = GetRotation();

    // 이동 처리
    {
        const float yaw = glm::degrees(std::atan2(direction.x, direction.z));

        position += direction * (moveSpeed * deltaTime_);
        rotation = glm::vec3(0.0f, yaw, 0.0f);

        direction = glm::vec3(0.0f);
    }
	// 중력 처리
    {
        verticalVelocity -= GRAVITY * deltaTime_;

        position.y += verticalVelocity * deltaTime_;

        if (position.y < groundY)
        {
            position.y = groundY;  
            verticalVelocity = 0.0f;
        }
    }

    SetPosition(position);
    SetRotation(rotation);
}