#ifndef GUARD_BLOCK_H
#define GUARD_BLOCK_H

#include "Object.h"

class Mesh;

class Block final
	: public Object
{
public:
	/**
	 * @brief 생성자.
	 * 
	 * @param mesh_ 해당 블록의 메쉬
	 */
	explicit Block(Mesh* const mesh_) noexcept;

	/**
	 * @brief 소멸자.
	 */
	virtual ~Block() noexcept override;

	/**
	 * @brief 해당 객체를 업데이트합니다.
	 * 
	 * @param deltaTime_ 이전 프레임과 현재 프레임 사이의 간격
	 */
	virtual void Update(const float deltaTime_) noexcept override;

	/**
	 * @brief 해당 객체를 렌더링합니다.
	 * 
	 * @param shader_ 사용할 셰이더
	 */
	virtual void Render(const Shader& shader_) const noexcept override;

private:
	/**
	 * @brief 해당 블록의 메쉬.
	 */
	Mesh* mesh;
};

#endif // !GUARD_BLOCK_H