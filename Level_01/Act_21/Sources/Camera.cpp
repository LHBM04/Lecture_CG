#include "Camera.h"

Camera::Camera() noexcept
    : projection(Projection::Perspective),
      position(0.0f, 0.0f, 5.0f),
      forward(0.0f, 0.0f, -1.0f),
      up(0.0f, 1.0f, 0.0f) 
{

}

void Camera::PreRender(const Shader& shader_) const noexcept
{
	glViewport(viewport.x, viewport.y, viewport.width, viewport.height);

    const glm::mat4 view = GetViewMatrix();
    shader_.SetUniformMatrix4x4("uView", view);

    const glm::mat4 projection = GetProjectionMatrix();
    shader_.SetUniformMatrix4x4("uProjection", projection);
}