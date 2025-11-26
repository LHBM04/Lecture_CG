#pragma once

#include "Object.h"

/**
 * @brief 산 오브젝트를 정의합니다.
 */
class Mountain final
	: public Object
{
public:
	/**
	 * @brief 생성자.
	 */
	explicit Mountain(const bool include_) noexcept;

	/**
	 * @brief
	 */
	virtual ~Mountain() noexcept;

	/**
	 * @brief 미로 경로 포함 여부를 설정합니다.
	 *
	 * @param include_ 미로 경로 포함 여부
	 */
	inline void IncludePath(const bool include_) noexcept;

protected:
	/**
	 * @brief
	 *
	 * @param deltaTime_
	 */
	virtual void OnUpdate(const float deltaTime_) noexcept override;

private:
	/**
	 * @brief 애니메이션 상태를 정의합니다.
	 */
	enum class AnimationState
	{
		/**
		 * @brief 기본: 설정된 높이(height)까지 자라남
		 */
		Default,

		/**
		 * @brief M: 숨쉬기 (오르락내리락)
		 */
		AnimationM,

		/**
		 * @brief R: 미로 경로 (납작해짐)
		 */
		AnimationR,

		/**
		 * @brief  V: 균등화 (높이 3.0 고정)
		 */
		AnimationV
	};

	/**
	 * @brief 해당 산의 높이.
	 */
	float height;

	/**
	 * @brief 애니메이션 속도.
	 */
	float animationSpeed;

	/**
	 * @brief 미로 경로 포함 여부.
	 */
	bool isIncluded;

	/**
	 * @brief 애니메이션 시작 여부.
	 */
	bool animation;

	/**
	 * @brief 현재 애니메이션 상태.
	 */
	AnimationState currentState;

	/**
	 * @brief 성장 여부.
	 */
	bool shouldGrow;

	/**
	 * @brief 멈춤 여부.
	 */
	bool shouldPause;
};

inline void Mountain::IncludePath(const bool include_) noexcept
{
	isIncluded = include_;
}