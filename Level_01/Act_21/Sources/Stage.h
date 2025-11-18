#ifndef GUARD_BOX_H
#define GUARD_BOX_H

#include "PCH.h"

#include "Object.h"

class Mesh;

class Stage final
	: public Object
{
public:
	/**
	 * @brief 생성자.
	 * 
	 * @param mesh_ 해당 박스의 메쉬
	 */
	explicit Stage(Mesh* const mesh_) noexcept;

	/**
	 * @brief 소멸자.
	 */
	virtual ~Stage() noexcept override;

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
	 * @brief 자기 자신을 회전시킵니다.
	 */
	void RotateSelf(const float deltaTime_) noexcept;

	/**
	 * @brief 바닥을 엽니다.
	 */
	void OpenFloor() noexcept;

	/**
	 * @brief 해당 박스의 메쉬.
	 */
	Mesh* mesh;

	/**
	 * @brief 바닥이 열린 상태인지 여부.
	 */
	bool isFloorOpen;

	/**
	 * @brief 바닥 정점 인덱스들.
	 */
	const std::array<std::size_t, 4> floorVertexIndices = { 8, 9, 10, 11 };
};
	
#endif // !GUARD_BOX_H