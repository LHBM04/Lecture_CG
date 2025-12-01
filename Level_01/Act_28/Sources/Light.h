#pragma once 

#include "PCH.h"

class Light final
{
public:
	/**
	 * @brief 생성자.
	 * 
	 * @param position_ 생성할 조명의 위치
	 * @param color_	생성할 조명의 색상
	 */
	explicit Light(const glm::vec3& position_, 
				   const glm::vec3& color_) noexcept;

	/**
	 * @brief 해당 조명의 위치를 반환합니다.
	 * 
	 * @return glm::vec3& 해당 조명의 위치
	 */
	[[nodiscard]]
	inline constexpr glm::vec3& GetPosition() const noexcept;

	/**
	 * @brief 해당 조명의 위치를 설정합니다.
	 * 
	 * @param position_ 설정할 조명의 위치
	 */
	inline void SetPosition(const glm::vec3& position_) noexcept;

	/**
	 * @brief 해당 조명의 색상을 반환합니다.
	 * 
	 * @return glm::vec3& 해당 조명의 색상
	 */
	[[nodiscard]]
	inline constexpr glm::vec3& GetColor() const noexcept;

	/**
	 * @brief 해당 조명의 색상을 설정합니다.
	 * 
	 * @param color_ 설정할 조명의 색상
	 */
	inline void SetColor(const glm::vec3& color_) noexcept;

private:
	/**
	 * @brief 해당 조명의 위치.
	 */
	glm::vec3 position;

	/**
	 * @brief 해당 조명의 색상.
	 */
	glm::vec3 color;
};

inline constexpr glm::vec3& Light::GetPosition() const noexcept
{
	return const_cast<glm::vec3&>(position);
}

inline void Light::SetPosition(const glm::vec3& position_) noexcept
{
	position = position_;
}

inline constexpr glm::vec3& Light::GetColor() const noexcept
{
	return const_cast<glm::vec3&>(color);
}

inline void Light::SetColor(const glm::vec3& color_) noexcept
{
	color = color_;
}