#ifndef GUARD_CUBE_H
#define GUARD_CUBE_H

#include "Object.h"

class Cube final
	: public Object
{
public:
	/**
	 * @brief 생성자.
	 * 
	 * @param position_ 생성할 큐브의 위치 값
	 * @param rotation_ 생성할 큐브의 회전 값
	 * @param scale_    생성할 큐브의 크기 값
	 */
	explicit Cube(
		const glm::vec3& position_ = glm::vec3{ 0.0f, 0.0f, 0.0f },
		const glm::vec3& rotation_ = glm::vec3{ 0.0f, 0.0f, 0.0f },
		const glm::vec3& scale_    = glm::vec3{ 1.0f, 1.0f, 1.0f }
	) noexcept;

	/**
	 * @brief 소멸자.
	 */
	virtual ~Cube() noexcept override;

	/**
	 * @brief 큐브를 업데이트합니다.
	 */
	virtual void Update(float deltaTime_) noexcept override;

};

#endif // !GUARD_CUBE_H