#ifndef GUARD_MARBLE_H
#define GUARD_MARBLE_H

#include "Object.h"

class Mesh;

class Marble final
	: public Object
{
public:
	/**
	 * @brief 
	 * 
	 * @param mesh_ 
	 */
	explicit Marble(Mesh* const mesh_) noexcept;

	/**
	 * @brief 
	 */
	virtual ~Marble() noexcept override;

	/**
	 * @brief 
	 * 
	 * @param deltaTime_ 
	 */
	virtual void Update(const float deltaTime_) noexcept override;

	/**
	 * @brief 
	 * 
	 * @param shader_ 
	 */
	virtual void Render(const Shader& shader_) const noexcept override;

private:
	/**
	 * @brief 해당 구슬의 매쉬.
	 */
	Mesh* mesh;

	glm::vec3  velocity = glm::vec3(3.0f, 2.0f, 0.0f); // 초기 속도
	float      radius = 0.5f;       // 메쉬 크기에 맞게 조정
	float      restitution = 0.85f;      // 반발계수
	float      damping = 0.995f;     // 미세 감쇠
};

#endif // !GUARD_MARBLE_H