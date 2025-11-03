#include "Axes.h"

Axes::Axes() noexcept
    : Object{}
{

}

void Axes::Update() noexcept
{

}

void Axes::Render(const Shader& shader_) const noexcept
{
    Object::Render(shader_);
}