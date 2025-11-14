#ifndef GUARD_CAMERA_H
#define GUARD_CAMERA_H

#include <glm/vec3.hpp>
#include <glm/ext.hpp>

#include "Application.h"
#include "Shader.h"

/**
 * @brief Scene 내 카메라를 정의합니다.
 */
class Camera final
{
public:
    /**
     * @enum Projection
     *
     * @brief 카메라의 투영 방식을 정의합니다.
     */
    enum class Projection
    {
        /**
         * @brief 직교 투영 방식.
         */
        Orthographic,

        /**
         * @brief 원근 투영 방식.
         */
        Perspective
    };

    /**
     * @brief 생성자.
     *
	 * @param projection_ 생성할 카메라의 투영 방식
     * @param position_       생성할 카메라의 위치
     * @param front_          생성할 카메라가 바라보는 방향
     * @param up_             생성할 카메라의 윗방향 벡터
     */
    explicit Camera(
        const Camera::Projection projection_,
        const glm::vec3&         position_,
        const glm::vec3&         front_,
        const glm::vec3&         up_
    ) noexcept;

    /**
     * @brief 해당 카메라의 위치를 가져옵니다.
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
     * @brief View 행렬 반환
     */
    [[nodiscard]]
    inline glm::mat4 GetViewMatrix() const noexcept;

    /**
         * @brief Projection 행렬 반환
         * @param aspectRatio_ 사용할 뷰포트의 종횡비.
         */
    [[nodiscard]]
    inline glm::mat4 GetProjectionMatrix(float aspectRatio_) const noexcept;

    /**
     * @brief SetAspectRatio 주석은 있으나 구현이 없으므로 삭제하거나 구현합니다.
     * 여기서는 GetProjectionMatrix가 aspectRatio를 직접 받으므로 불필요합니다.
     */
     // inline void SetAspectRatio(const float aspectRatio_) noexcept;

     /**
      * @brief 렌더링 사전 작업을 수행합니다. (셰이더에 View/Projection 행렬 전달)
      * * @param shader_ 사용할 셰이더
      * @param viewportAspectRatio_ 이 카메라가 렌더링될 뷰포트의 종횡비
      */
    void PreRender(const Shader& shader_, float viewportAspectRatio_) const noexcept;

    /**
     * @brief 종횡비 업데이트 (전략 객체에 위임)
     */
    inline void SetAspectRatio(const float aspectRatio_) noexcept;

    /**
     * @brief 투영 방식을 런타임에 교체합니다.
     */
    inline void SetProjection(Camera::Projection projection_) noexcept;

private:
    /**
     * @brief 해당 카메라의 투영 방식.
     */
    Camera::Projection projection;

	/**
	 * @brief 해당 카메라의 위치.
	 */
	glm::vec3 position;

	/**
	 * @brief 해당 카메라의 앞방향 벡터.
	 */
	glm::vec3 forward;

	/**
	 * @brief 해당 카메라의 윗방향 벡터.
	 */
	glm::vec3 up;

    /**
     * @brief 시야각.
     */
    float fov = 45.0f;

    /**
     * @brief 
     */
    float nearPlane = 0.1f;

    /**
     * @brief
     */
    float farPlane = 100.0f;

    /**
     * @brief
     */
    float orthoSize = 10.0f;
};

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

inline glm::mat4 Camera::GetViewMatrix() const noexcept
{
    return glm::lookAt(position, position + forward, up);
}

inline glm::mat4 Camera::GetProjectionMatrix(const float aspectRatio_) const noexcept
{
    // const float height      = static_cast<float>(Application::GetSpecification().height);
    // const float width       = static_cast<float>(Application::GetSpecification().width);
    // const float aspectRatio = width / height; // <-- 기존 코드를 사용하지 않음

    switch (projection)
    {
        case Projection::Orthographic:
        {
            const float halfHeight = orthoSize * 0.5f;
            const float halfWidth  = halfHeight * aspectRatio_; // <-- 인자로 받은 종횡비 사용
            return glm::ortho(-halfWidth, halfWidth, -halfHeight, halfHeight, nearPlane, farPlane);
        }
        case Projection::Perspective:
        {
            return glm::perspective(glm::radians(fov), aspectRatio_, nearPlane, farPlane); // <-- 인자로 받은 종횡비 사용
        }
        default:
        {
            return { 1.0f };
        }
    }
}

inline void Camera::SetProjection(Camera::Projection projection_) noexcept
{
    projection = projection_;
}

#endif // !GUARD_CAMERA_H