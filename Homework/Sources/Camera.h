#pragma once

#include "PCH.h"

/**
 * @brief 
 */
class Camera final
{
public:
	/**
	 * @brief 카메라의 투영 방식을 정의합니다.
	 */
	enum class Projection
	{
		/**
		 * @brief 원근 투영.
		 */
		Perspective,

		/**
		 * @brief 직교 투영.
		 */
		Orthographic
	};    

	/**
	 * @brief 뷰포트를 정의합니다.
	 */
	struct Viewport final
	{
		/**
		 * @brief x 좌표.
		 */
		int x;

		/**
		 * @brief y 좌표.
		 */
		int y;

		/**
		 * @brief 너비.
		 */
		int width;

		/**
		 * @brief 높이.
		 */
		int height;

		/**
		 * @brief 생성자.
		 * 
		 * @param x_	  생성할 뷰포트의 x 좌표
		 * @param y_	  생성할 뷰포트의 y 좌표
		 * @param width_  생성할 뷰포트의 너비
		 * @param height_ 생성할 뷰포트의 높이
		 */
		explicit Viewport(const int x_,
						  const int y_,
						  const int width_,  
						  const int height_) noexcept
			: x(x_)
			, y(y_)
			, width(width_)
			, height(height_)
		{
		}
	};

	/**
	 * @brief 생성자.
	 * 
	 * @param projection_ 생성할 카메라의 투영 방식
	 * @param eye_		  생성할 카메라의 위치
	 * @param at_		  생성할 카메라의 주시점
	 * @param up_		  생성할 카메라의 윗방향 벡터
	 * @param viewport_	  생성할 카메라의 뷰포트
	 */
	explicit Camera(const Camera::Projection projection_, 
					const glm::vec3&		 eye_,
					const glm::vec3&		 at_,
					const glm::vec3&		 up_,
					const Camera::Viewport&  viewport_) noexcept;

	/**
	 * @brief 해당 카메라의 투영 방식을 반환합니다.
	 * 
	 * @return Camera::Projection 해당 카메라의 투영 방식
	 */
	[[nodiscard]]
	inline constexpr Camera::Projection GetProjection() const noexcept;

	/**
	 * @brief 해당 카메라의 투영 방식을 지정한 방식으로 설정합니다.
	 * 
	 * @param projection_ 지정할 투영 방식
	 */
	inline void SetProjection(Camera::Projection projection_) noexcept;

	/**
	 * @brief 해당 카메라의 위치를 반환합니다.
	 *
	 * @return glm::vec3 해당 카메라의 위치
	 */
	[[nodiscard]]
	inline constexpr glm::vec3 GetPosition() const noexcept;

	/**
	 * @brief 해당 카메라의 위치를 지정한 위치로 설정합니다.
	 *
	 * @param position_ 지정할 위치
	 */
	inline void SetPosition(const glm::vec3& position_) noexcept;

	/**
	 * @brief 해당 카메라의 앞방향 벡터를 반환합니다.
	 *
	 * @return glm::vec3 해당 카메라의 앞방향 벡터
	 */
	[[nodiscard]]
	inline constexpr glm::vec3 GetForward() const noexcept;

	/**
	 * @brief 해당 카메라의 앞방향 벡터를 지정한 벡터로 설정합니다.
	 *
	 * @param position_ 지정할 앞방향 벡터
	 */
	inline void SetForward(const glm::vec3& position_) noexcept;

	/**
	 * @brief 해당 카메라의 윗방향 벡터를 반환합니다.
	 *
	 * @return glm::vec3 해당 카메라의 윗방향 벡터
	 */
	[[nodiscard]]
	inline constexpr glm::vec3 GetUp() const noexcept;

	/**
	 * @brief 해당 카메라의 윗방향 벡터를 지정한 벡터로 설정합니다.
	 *
	 * @param up_ 지정할 벡터.
	 */
	inline void SetUp(const glm::vec3& up_) noexcept;

	/**
	 * @brief 뷰포트 반환
	 */
	[[nodiscard]]
	inline const Camera::Viewport& GetViewport() noexcept;

	/**
	 * @brief 뷰포트 설정
	 */
	inline void SetViewport(const Viewport& viewport_) noexcept;

	/**
	 * @brief View 행렬 반환
	 */
	[[nodiscard]]
	inline glm::mat4 GetViewMatrix() const noexcept;

	/**
	 * @brief Projection 행렬 반환
	 */
	[[nodiscard]]
	inline glm::mat4 GetProjectionMatrix() const noexcept;

	/**
	 * @brief 렌더링 전에 호출되어 셰이더에 뷰 및 프로젝션 행렬을 설정합니다.
	 */
	void PreRender() const noexcept;

private:
	/**
	* @brief 해당 카메라의 투영 방식.
	*/
	Camera::Projection projection = Camera::Projection::Perspective;

	/**
	 * @brief 해당 카메라의 위치.
	 */
	glm::vec3 position = glm::vec3(0.0f);

	/**
	 * @brief 해당 카메라의 앞방향 벡터.
	 */
	glm::vec3 forward = glm::vec3(0.0f, 0.0f, 10.0f);

	/**
	 * @brief 해당 카메라의 윗방향 벡터.
	 */
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	
	/**
	 * @brief 시야각.
	 */
	float fov = 45.0f;

	/**
	 * @brief 근평면.
	 */
	float nearPlane = 0.1f;

	/**
	 * @brief 원평면.
	 */
	float farPlane = 100.0f;

	/**
	 * @brief 
	 */
	float orthoSize = 10.0f;

	/**
	 * @brief 뷰포트.
	 */
	Viewport viewport;
};

inline constexpr Camera::Projection Camera::GetProjection() const noexcept
{
	return projection;
}

inline void Camera::SetProjection(Camera::Projection projection_) noexcept
{
	projection = projection_;
}

inline constexpr glm::vec3 Camera::GetPosition() const noexcept
{
	return position;
}

inline void Camera::SetPosition(const glm::vec3& position_) noexcept
{
	position = position_;
}

inline constexpr glm::vec3 Camera::GetForward() const noexcept
{
	return forward;
}

inline void Camera::SetForward(const glm::vec3& forward_) noexcept
{
	forward = forward_;
}

inline constexpr glm::vec3 Camera::GetUp() const noexcept
{
	return up;
}

inline void Camera::SetUp(const glm::vec3& up_) noexcept
{
	up = up_;
}

inline const Camera::Viewport& Camera::GetViewport() noexcept
{
	return viewport;
}

inline void Camera::SetViewport(const Viewport& viewport_) noexcept
{
	viewport = viewport_;
}

inline glm::mat4 Camera::GetViewMatrix() const noexcept
{
	return glm::lookAt(position, position + forward, up);
}

inline glm::mat4 Camera::GetProjectionMatrix() const noexcept
{
	const float height		= viewport.height;
	const float width		= viewport.width;
	const float aspectRatio = width / height;

	switch (projection)
	{
		case Projection::Orthographic:
		{
			const float halfHeight = orthoSize * 0.5f;
			const float halfWidth  = halfHeight * aspectRatio;

			return glm::ortho(-halfWidth, halfWidth, -halfHeight, halfHeight, nearPlane, farPlane);
		}
		case Projection::Perspective:
		{
			return glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);
		}
		default:
		{
			return { 1.0f };
		}
	}
}