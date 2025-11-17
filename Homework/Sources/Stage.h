#pragma once

#include "PCH.h"

#include "Mesh.h"
#include "Object.h"
#include "Shader.h"

class Stage final
	: public Object
{
public:
	/**
	 * @brief 
	 * 
	 * @param width_ 
	 * @param height_
	 */
	explicit Stage(const std::size_t& width_,
				   const std::size_t& height_,
				   Mesh* const		  mesh_) noexcept;

	/**
	 * @brief 
	 */
	virtual ~Stage() noexcept;

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
	 * @brief 해당 스테이지의 메쉬.
	 */
	Mesh* mesh;
};