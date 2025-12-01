#include "Light.h"

Light::Light(const glm::vec3& position_,
			 const glm::vec3& color_) noexcept
	: position(position_)
	, color(color_)
{
} 