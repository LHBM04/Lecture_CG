#include "Light.h"

#include "Shader.h"

Light::Light(const glm::vec3& position_,
			 const glm::vec3& color_,
			 const float      ambient_,
			 const float      specular_) noexcept
	: position(position_)
	, color(color_)
	, ambient(ambient_)
	, specular(specular_)
{
	Shader::SetUniformVector3("uLight.position", position);
	Shader::SetUniformVector3("uLight.color", color);
	Shader::SetUniformFloat("uLight.ambient", ambient);
	Shader::SetUniformFloat("uLight.specular", specular);
}