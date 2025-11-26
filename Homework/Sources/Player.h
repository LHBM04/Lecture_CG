#pragma once

#include "Object.h"

class Player final
	: public Object
{
public:
	/**
	 * @brief 생성자.
	 */
	explicit Player() noexcept;

private:
	/**
	 * @brief
	 *
	 * @param deltaTime_
	 */
	virtual void OnUpdate(const float deltaTime_) noexcept override;

private:
	/**
	 * @brief 플레이어의 최대 이동 속도.
	 */
	static constexpr float MAX_PLAYER_SPEED = 20.0f;

	/**
	 * @brief 플레이어의 최대 점프 힘.
	 */
	static constexpr float MAX_JUMP_FORCE = 10.0f;

	/**
	 * @brief 플레이어의 기본 이동 속도.
	 */
	static constexpr float DEFAULT_MOVE_SPEED = 3.0f;

	/**
	 * @brief 플레이어의 기본 점프 힘.
	 */
	static constexpr float DEFAULT_JUMP_FORCE = 5.0f;

	/**
	 * @brief 중력 가속도.
	 */
	static constexpr float GRAVITY = 9.81f;

	/**
	 * @brief 플레이어의 이동 방향.
	 */
	glm::vec3 direction;

	/**
	 * @brief 플레이어의 수직 속도.
	 */
	float verticalVelocity;

	/**
	 * @brief 플레이어의 이동 속도.
	 */
	float moveSpeed;

	/**
	 * @brief 플레이어의 점프 힘.
	 */
	float jumpForce;
};