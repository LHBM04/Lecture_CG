#include "Camera.h"

Camera::Camera(const glm::vec3& position_,
               const glm::vec3& front_,
               const glm::vec3& up_) noexcept
    : projectionType{Camera::ProjectionType::Perspective}
    , position{position_}
    , front{front_}
    , up{up_}
{

}

void Camera::PreRender(const Shader& shader_) const noexcept
{
    const glm::mat4 view = GetViewMatrix();
    shader_.SetUniformMatrix4x4("uView", view);

    const glm::mat4 projection = GetProjectionMatrix();
    shader_.SetUniformMatrix4x4("uProjection", projection);
}