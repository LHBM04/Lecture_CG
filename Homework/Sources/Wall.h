#pragma once

#include "Object.h"
#include "Mesh.h"
#include "Shader.h"

class Wall final
	: public Object
{
public:
	/**
	 * @brief
	 */
	explicit Wall(const float reach_, Mesh* const mesh_) noexcept;

	/**
	 * @brief
	 */
	virtual ~Wall() noexcept;

	/**
	 * @brief
	 *
	 * @param deltaTime_
	 */
	virtual void Update(const float deltaTime_) noexcept override;

	/**
	 * @brief
	 *
	 * @param shader_
	 */
	virtual void Render(const Shader& shader_) const noexcept override;

private:
	/**
	 * @brief 해당 월의 메쉬.
	 */
	Mesh* mesh;

	/**
	 * @brief 해당 월의 높이.
	 */
	float height;
};