#include "Camera.h"

#include "Shader.h"

Camera::Camera(const Camera::Projection projection_,
               const glm::vec3&         eye_,
               const glm::vec3&         at_,
               const glm::vec3&         up_,
               const Camera::Viewport&  viewport_) noexcept
    : projection(projection_)
    , position(eye_)
    , forward(at_ - eye_)
    , up(up_)
	, viewport(viewport_)
{

}

void Camera::PreRender() const noexcept
{
	glViewport(viewport.x, viewport.y, viewport.width, viewport.height);

    const glm::mat4 view = GetViewMatrix();
    Shader::SetUniformMatrix4x4("uView", view);

    const glm::mat4 projection = GetProjectionMatrix();
    Shader::SetUniformMatrix4x4("uProjection", projection);

    Shader::SetUniformVector3("uViewPos", position);
}