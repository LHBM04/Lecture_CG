#ifndef GUARD_CAMERA_H
#define GUARD_CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Shader.h"

/**
 * @interface Camera
 *
 * @brief 카메라를 정의합니다.
 */
class Camera final
{
public:
    /**
     * @enum ProjectionType
     *
     * @brief 카메라의 투영 방식을 정의합니다.
     */
    enum class ProjectionType
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
     * @param position_   생성할 카메라의 위치
     * @param front_      생성할 카메라가 바라보는 방향
     * @param up_         생성할 카메라의 윗방향 벡터
     */
    explicit Camera(const glm::vec3& position_,
                    const glm::vec3& front_,
                    const glm::vec3& up_) noexcept;

    /**
     * @brief 렌더링 전에 호출됩니다.
     */
    void PreRender(const Shader& shader_) const noexcept;

    /**
     * @brief 카메라 위치 반환
     */
    [[nodiscard]]
    inline constexpr glm::vec3 GetPosition() const noexcept;

    /**
     * @brief 카메라 위치 설정
     */
    inline void SetPosition(const glm::vec3& position_) noexcept;

    /**
     * @brief 카메라 전방 벡터 반환
     */
    [[nodiscard]]
    inline constexpr glm::vec3 GetFront() const noexcept;

    /**
     * @brief 카메라 전방 벡터 설정
     */
    inline void SetFront(const glm::vec3& front_) noexcept;

    /**
     * @brief 카메라 업 벡터 반환
     */
    [[nodiscard]]
    inline constexpr glm::vec3 GetUp() const noexcept;

    /**
     * @brief 카메라 업 벡터 설정
     */
    inline void SetUp(const glm::vec3& up_) noexcept;

    /**
     * @brief View 행렬 반환
     */
    [[nodiscard]]
    inline glm::mat4 GetViewMatrix() const noexcept;

    /**
     * @brief Projection 행렬 반환 (전략 객체에 위임)
     */
    [[nodiscard]]
    inline glm::mat4 GetProjectionMatrix() const noexcept;

    /**
     * @brief 종횡비 업데이트 (전략 객체에 위임)
     */
    inline void SetAspectRatio(const float aspectRatio_) noexcept;

    /**
     * @brief 투영 방식을 런타임에 교체합니다.
     */
    inline void SetProjection(Camera::ProjectionType projection_) noexcept;

private:
    /**
     * @brief 해당 카메라의 투영 방식.
     */
    Camera::ProjectionType projectionType;

    /**
     * @brief 해당 카메라의 위치.
     */
    glm::vec3 position;

    /**
     * @brief 해당 카메라가 바라보는 방향.
     */
    glm::vec3 front;

    /**
     * @brief 해당 카메라의 윗방향 벡터.
     */
    glm::vec3 up;

    /**
     * @brief 시야각.
     */
    float fov = 45.0f;

    /**
     * @brief 종횡비.
     */
    float aspectRatio = 0.0f;

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

constexpr glm::vec3 Camera::GetPosition() const noexcept
{
    return position;
}

inline void Camera::SetPosition(const glm::vec3& position_) noexcept
{
    position = position_;
}

inline constexpr glm::vec3 Camera::GetFront() const noexcept
{
    return front;
}

inline void Camera::SetFront(const glm::vec3& front_) noexcept
{
    front = front_;
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
    return glm::lookAt(position, position + front, up);
}

inline glm::mat4 Camera::GetProjectionMatrix() const noexcept
{
    switch (projectionType)
    {
        case ProjectionType::Orthographic:
        {
            const float halfHeight = orthoSize * 0.5f;
            const float halfWidth  = halfHeight * aspectRatio;

            return glm::ortho(-halfWidth, halfWidth, -halfHeight, halfHeight, nearPlane, farPlane);
        }
        case ProjectionType::Perspective:
        {
            return glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);
        }
        default:
        {
            return {1.0f};
        }
    }
}

inline void Camera::SetAspectRatio(const float aspectRatio_) noexcept
{
    aspectRatio = aspectRatio_;
}

inline void Camera::SetProjection(Camera::ProjectionType projection_) noexcept
{
    projectionType = projection_;
}

#endif // !GUARD_CAMERA_H