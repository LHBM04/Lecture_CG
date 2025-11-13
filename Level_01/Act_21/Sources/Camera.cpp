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

void Camera::PreRender(const Shader& shader_) const noexcept
{
    const glm::mat4 view = GetViewMatrix();
    shader_.SetUniformMatrix4x4("uView", view);

    const glm::mat4 projection = GetProjectionMatrix();
    shader_.SetUniformMatrix4x4("uProjection", projection);
}