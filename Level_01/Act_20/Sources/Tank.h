#ifndef GUARD_TANK_H
#define GUARD_TANK_H

#include <memory>

#include <glm/vec3.hpp>

#include "Object.h"

/**
 * @brief 탱크를 정의합니다.
 */
class Tank final
	: public Object
{
public:
	/**
	 * @brief 생성자.
	 *
	 * @param position_ 생성할 탱크의 위치
	 * @param rotation_ 생성할 탱크의 회전
	 * @param scale_    생성할 탱크의 크기
	 */
	explicit Tank(
		const glm::vec3& position_ = glm::vec3{ 0.0f, 0.0f, 0.0f },
		const glm::vec3& rotation_ = glm::vec3{ 0.0f, 0.0f, 0.0f },
		const glm::vec3& scale_    = glm::vec3{ 1.0f, 1.0f, 1.0f }
	) noexcept;

	/**
	 * @brief 소멸자.
	 */
	virtual ~Tank() noexcept override;

	/**
	 * @brief 해당 대포를 업데이트합니다.
	 * 
	 * @param deltaTime_ 이전 프레임과 현재 프레임 사이의 시간 간격
	 */
	virtual void Update(const float deltaTime_) noexcept override;

	/**
	 * @brief 해당 대포를 렌더링합니다.
	 * 
	 * @param shader_     사용할 셰이더
	 * @param renderMode_ 렌더링 모드
	 */
	virtual void Render(
		const Shader& shader_, 
		const GLenum  renderMode_ = GL_TRIANGLES
	) const noexcept override;

private:
	std::unique_ptr<Object> base;

	std::unique_ptr<Object> middle;

	std::unique_ptr<Object> leftTop;
	std::unique_ptr<Object> leftBarrel;
	std::unique_ptr<Object> leftBarrelPivot;
	std::unique_ptr<Object> leftPole;
	std::unique_ptr<Object> leftPolePivot;

	std::unique_ptr<Object> rightTop;
	std::unique_ptr<Object> rightBarrel;
	std::unique_ptr<Object> rightBarrelPivot;
	std::unique_ptr<Object> rightPole;
	std::unique_ptr<Object> rightPolePivot;

	/**
	 * @brief 탱크 이동 속도.
	 */
	static constexpr float MOVE_SPEED = 5.0f;

	/**
	 * @brief 탱크 회전 속도.
	 */
	static constexpr float ROTATE_SPEED = 45.0f;

	/**
	 * @brief 중앙 몸체 애니메이션 트리거.
	 */
	bool isAnimT = false;

	/**
	 * @brief 상부 몸체 애니메이션 트리거.
	 */
	bool isAnimL = false;

	/**
	 * @brief 두 포신 y축 회전 애니메이션 트리거.
	 */
	bool isAnimG = false;
	
	/**
	 * @brief G 애니메이션 경과 시간.
	 */
	float animGTime = 0.0f;

	/**
	 * @brief 두 포신 x축 회전 애니메이션 트리거.
	 */
	bool isAnimP = false;

	float animPTime = 0.0f;
	
	/**
	 * @brief 상부 몸체의 이동 타겟.
	 */
	glm::vec3 leftTargetPos;

	/**
	 * @brief 또 다른 상부 몸체의 이동 타켓
	 */
	glm::vec3 rightTargetPos;

	/**
	 * @brief 회전을 위한 가상 객체.
	 */
	std::unique_ptr<Object> temp;

	std::unique_ptr<Object> left;
	std::unique_ptr<Object> right;
};

#endif // !GUARD_TANK_H