#ifndef GUARD_CAMERA_H
#define GUARD_CAMERA_H

#include <memory>

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
     * @brief 소멸자.
     */
    ~Camera() noexcept;

    /**
     * @brief 렌더링 전에 호출됩니다.
     */
    void PreRender(const Shader& shader_) const noexcept;

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

    float fov = 45.0f;
    float aspectRatio = 0.0f;
    float nearPlane = 0.1f;
    float farPlane = 100.0f;
    float orthoSize = 10.0f;
};

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