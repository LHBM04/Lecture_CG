#pragma once

#include "PCH.h"

#include "Shader.h"

class Light final
{
public:
	/**
	 * @brief 생성자.
	 * 
	 * @param position_ 생성할 조명의 위치
	 * @param color_	생성할 조명의 색상
	 * @param ambient_  생성할 조명의 환경광 강도
	 * @param specular_ 생성할 조명의 난반사광 강도
	 */
	explicit Light(const glm::vec3& position_,
				   const glm::vec3& color_,
				   const float      ambient_ = 0.1f,
				   const float      specular_ = 0.5f) noexcept;

	[[nodiscard]]
	inline const glm::vec3& GetPosition() const noexcept;
	inline void SetPosition(const glm::vec3& position_) noexcept;

	[[nodiscard]]
	inline const glm::vec3& GetColor() const noexcept;
	inline void SetColor(const glm::vec3& color_) noexcept;

	[[nodiscard]]
	inline const float GetAmbient() const noexcept;
	inline void SetAmbient(const float ambient_) noexcept;

	[[nodiscard]]
	inline const float GetSpecular() const noexcept;
	inline void SetSpecular(const float specular_) noexcept;

private:
	/**
	 * @brief 해당 조명의 위치.
	 */
	glm::vec3 position;
	
	/**
	 * @brief 해당 조명의 색상.
	 */
	glm::vec3 color;
	
	/**
	 * @brief 환경광 강도.
	 */
	float ambient;

	/**
	 * @brief 난반사광 강도.
	 */
	float specular;
};

inline const glm::vec3& Light::GetPosition() const noexcept
{
	return position;
}

inline void Light::SetPosition(const glm::vec3& position_) noexcept
{
	position = position_;
	Shader::SetUniformVector3("uLight.position", position);
}

inline const glm::vec3& Light::GetColor() const noexcept
{
	return color;
}

inline void Light::SetColor(const glm::vec3& color_) noexcept
{
	color = color_;
	Shader::SetUniformVector3("uLight.color", color);
}

inline const float Light::GetAmbient() const noexcept
{
	return ambient;
}

void Light::SetAmbient(const float ambient_) noexcept
{
	ambient = ambient_;
	Shader::SetUniformFloat("uLight.ambientStrength", ambient);
}

inline const float Light::GetSpecular() const noexcept
{
	return specular;
}

