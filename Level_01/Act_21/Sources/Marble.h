#ifndef GUARD_MARBLE_H
#define GUARD_MARBLE_H

#include "Object.h"

/**
 * @brief 구슬.
 */
class Marble final
	: public Object
{
public:
	/**
	 * @brief 생성자.
	 * 
	 * @param boundary_ 생성할 구슬이 움직일 수 있는 경계값
	 * @param position_ 생성할 구슬의 위치
	 * @param rotation_ 생성할 구슬의 회전
	 * @param scale_	생성할 구슬의 크기
	 */
	explicit Marble(
		const float      boundary_,
		const glm::vec3& position_ = glm::vec3(0.0f),
		const glm::vec3& rotation_ = glm::vec3(0.0f),
		const glm::vec3& scale_    = glm::vec3(1.0f)
	) noexcept;

	/**
	 * @brief 소멸자.
	 */
	virtual ~Marble() noexcept override;

	/**
	 * @brief 
	 * 
	 * @param deltaTime_ 
	 */
	virtual void Update(const float deltaTime_) noexcept override;

private:
	/**
	 * @brief 해당 구슬의 이동 속도.
	 */
	static constexpr float MOVE_SPEED = 10;

	/**
	 * @brief 경계값
	 */
	float boundary;

	/**
	 * @brief 해당 구슬의 이동 방향.
	 */
	glm::vec3 direction;
};

#endif // !GUARD_MARBLE_H