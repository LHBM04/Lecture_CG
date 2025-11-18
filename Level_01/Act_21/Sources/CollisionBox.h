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

inline constexpr glm::vec3 CollisionBox::GetCenter() const noexcept
{
	return center;
}

inline void CollisionBox::SetCenter(const glm::vec3& center_) noexcept
{
	center = center_;
}

inline constexpr glm::vec3 CollisionBox::GetMin() const noexcept
{
	return min;
}

inline void CollisionBox::SetMin(const glm::vec3& min_) noexcept
{
	min = min_;
}

inline constexpr glm::vec3 CollisionBox::GetMax() const noexcept
{
	return max;
}

inline void CollisionBox::SetMax(const glm::vec3& max_) noexcept
{
	max = max_;
}

constexpr bool CollisionBox::IsCollide(const CollisionBox& other_) const noexcept
{
	if (max.x < other_.min.x || min.x > other_.max.x) return false;
	if (max.y < other_.min.y || min.y > other_.max.y) return false;
	if (max.z < other_.min.z || min.z > other_.max.z) return false;
	return true;
}

#endif // !GUARD_COLLISIONBOX_H