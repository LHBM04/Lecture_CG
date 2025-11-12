#ifndef GUARD_BOX_H
#define GUARD_BOX_H

#include "Object.h"

class Box final
	: public Object
{
public:
	/**
	 * @brief 생성자.
	 * 
	 * @param position_ 상자의 위치
	 * @param rotation_ 상자의 회전
	 * @param scale_    상자의 크기
	 */
	explicit Box(
		const glm::vec3& position_ = glm::vec3(0.0f),
		const glm::vec3& rotation_ = glm::vec3(0.0f),
		const glm::vec3& scale_    = glm::vec3(1.0f)
	) noexcept;

	/**
	 * @brief 소멸자.
	 */
	virtual ~Box() noexcept override;

	/* 박스를 업데이트. */
	virtual void Update(const float deltaTime_) noexcept override;

	/* 박스를 렌더링. */
	virtual void Render(
		const Shader& shader_,
		const GLenum  renderMode_ = GL_TRIANGLES
	) const noexcept override;

private:
	/**
	 * @brief 상자 내부의 구슬들.
	 */
	std::array<std::unique_ptr<Object>, 6> marbles;
};

#endif // !GUARD_BOX_H