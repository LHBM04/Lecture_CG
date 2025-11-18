#ifndef GUARD_MARBLE_H
#define GUARD_MARBLE_H

#include "PCH.h"

#include "Object.h"

class Mesh;
class CollisionBox;

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

	/**
	 * @brief 충돌 박스
	 */
	std::unique_ptr<CollisionBox> collisionBox;

	/**
	 * @brief 이동 방향
	 */
	glm::vec3 direction = glm::vec3(0.0f, 0.0f, 0.0f);

	/**
	 * @brief 이동 속도
	 */
	float moveSpeed = 5.0f;
};

#endif // !GUARD_MARBLE_H