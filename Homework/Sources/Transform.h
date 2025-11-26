#pragma once

#include "PCH.h"

/**
 * @brief 트랜스폼(위치, 회전, 크기)를 정의합니다.
 */
struct Transform final
{
	/**
	 * @brief 위치 값.
	 */
	glm::vec3 position;

	/**
	 * @brief 회전 값
	 */
	glm::vec3 rotation;

	/**
	 * @brief 크기 값.
	 */
	glm::vec3 scale;

	/**
	 * @brief 부모 트랜스폼.
	 */
	Transform* parent;

	/**
	 * @brief 생성자.
	 * 
	 * @param position_ 생성할 트랜스폼의 위치 값.
	 * @param rotation_ 생성할 트랜스폼의 회전 값.
	 * @param scale_	생성할 트랜스폼의 크기 값.
	 * @param parent_	생성할 트랜스폼의 부모 트랜스폼.
	 */
	explicit Transform(const glm::vec3&	position_, 
					   const glm::vec3&	rotation_, 
					   const glm::vec3&	scale_,
					   Transform* const parent_) noexcept
		: position(position_)
		, rotation(rotation_)
		, scale(scale_)
		, parent(parent_)
	{
	}

	/**
	 * @brief 해당 트랜스폼의 모델 행렬을 반환합니다.
	 * 
	 * @return glm::mat4 해당 트랜스폼의 모델 행렬
	 */
	[[nodiscard]]
	inline glm::mat4 GetModel() const noexcept;
};

inline glm::mat4 Transform::GetModel() const noexcept
{
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, position);
	model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, scale);

	if (parent)
	{
		model = parent->GetModel() * model;
	}

	return model;
}