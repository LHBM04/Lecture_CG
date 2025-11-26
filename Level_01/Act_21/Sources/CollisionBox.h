#ifndef GUARD_COLLISIONBOX_H
#define GUARD_COLLISIONBOX_H

#include "PCH.h"

class CollisionBox final
{
public:
	/**
	 * @brief »ý¼ºÀÚ.
	 */
	explicit CollisionBox(const glm::vec3& center_, 
						  const glm::vec3& min_, 
						  const glm::vec3& max_) noexcept;

	/**
	 * @brief 
	 */
	[[nodiscard]]
	inline constexpr glm::vec3 GetCenter() const noexcept;

	/**
	 * @brief 
	 */
	inline void SetCenter(const glm::vec3& center_) noexcept;

	/**
	 * @brief
	 */
	[[nodiscard]]
	inline constexpr glm::vec3 GetMin() const noexcept;

	/**
	 * @brief 
	 */
	inline void SetMin(const glm::vec3& min_) noexcept;

	/**
	 * @brief 
	 */
	[[nodiscard]]
	inline constexpr glm::vec3 GetMax() const noexcept;

	/**
	 * @brief 
	 */
	inline void SetMax(const glm::vec3& max_) noexcept;

	/**
	 * @brief 
	 */
	[[nodiscard]]
	inline constexpr bool IsCollide(const CollisionBox& other_) const noexcept;

private:
	/**
	 * @brief 
	 */
	glm::vec3 center = glm::vec3(0.0f);

	/**
	 * @brief 
	 */
	glm::vec3 min = glm::vec3(0.0f);

	/**
	 * @brief 
	 */
	glm::vec3 max = glm::vec3(0.0f);
}

#endif // !GUARD_COLLISIONBOX_H