#include "Camera.h"

Camera::Camera(
	const Camera::Projection projection_,
    const glm::vec3&         position_,
    const glm::vec3&         forward_,
    const glm::vec3&         up_
) noexcept
    : projection(projection_)
    , position(position_)
    , forward(forward_)
    , up(up_)
{

}

void Camera::PreRender(const Shader& shader_, float viewportAspectRatio_) const noexcept
{
    const glm::mat4 view = GetViewMatrix();
    shader_.SetUniformMatrix4x4("uView", view);

    // 수정된 GetProjectionMatrix 호출
    const glm::mat4 projection = GetProjectionMatrix(viewportAspectRatio_);
    shader_.SetUniformMatrix4x4("uProjection", projection);
}