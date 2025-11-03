#include "World.h"

#include "../Core/Application.h"

World::World() noexcept
{
    camera = std::make_unique<Camera>(glm::vec3{0.0f, 0.0f, 5.0f},
                                      glm::vec3{0.0f, 0.0f, -1.0f},
                                      glm::vec3{0.0f, 1.0f, 0.0f});
    camera->SetProjection(Camera::ProjectionType::Perspective);
    camera->SetAspectRatio(static_cast<float>(Application::GetSpecification().width) /
                           static_cast<float>(Application::GetSpecification().height));
}

World::~World() noexcept
{
    for (auto& object : objects)
    {
        object.reset();
    }

    camera.reset();
}

void World::AddObject(std::unique_ptr<Object>&& object_) noexcept
{
    objects.push_back(std::move(object_));
}

void World::FixedUpdate() const noexcept
{
    for (const auto& object : objects)
    {
        object->Update();
    }
}

void World::Update() const noexcept
{
    for (const auto& object : objects)
    {
        object->Update();
    }
}

void World::LateUpdate() const noexcept
{
    for (const auto& object : objects)
    {
        object->Update();
    }
}

void World::Render(const Shader& shader_) const noexcept
{
    camera->PreRender(shader_);

    for (const auto& object : objects)
    {
        object->Render(shader_);
    }
}
