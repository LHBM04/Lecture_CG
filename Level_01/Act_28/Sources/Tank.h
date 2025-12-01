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
	 * @brief ������.
	 */
	explicit Tank() noexcept;

	/**
	 * @brief �Ҹ���.
	 */
	virtual ~Tank() noexcept override;

	/**
	 * @brief 탱크를 매 프레임마다 업데이트합니다.
	 *
	 * @param deltaTime_ 이전 프레임과 현재 프레임 사이의 간격
	 */
	virtual void Update(const float deltaTime_) noexcept override;

	/**
	 * @brief 탱크를 그립니다.
	 *
	 * @param shader_ 사용할 셰이더
	 */
	virtual void Render(const Shader& shader_) const noexcept override;

private:
	/**
	 * @brief 몸통 메쉬.
	 */
	std::shared_ptr<Mesh> baseMesh;

	/**
	 * @brief 포탑 메쉬.
	 */
	std::shared_ptr<Mesh> topMesh;

	/**
	 * @brief 포신 메쉬.
	 */
	std::shared_ptr<Mesh> barrelMesh;

	/**
	 * @brief 포대 메쉬.
	 */
	std::shared_ptr<Mesh> poleMesh;

	/**
	 * @brief 중간 부분 메쉬.
	 */
	std::shared_ptr<Mesh> middleMesh;

	/**
	 * @brief 몸통.
	 */
	std::unique_ptr<Object> base;

	/**
	 * @brief 중간 부분.
	 */
	std::unique_ptr<Object> middle;

	/**
	 * @brief 왼쪽 포탑.
	 */
	std::unique_ptr<Object> leftTop;

	/**
	 * @brief 왼쪽 포신.
	 */
	std::unique_ptr<Object> leftBarrel;

	/**
	 * @brief 왼쪽 포신 피벗.
	 */
	std::unique_ptr<Object> leftBarrelPivot;

	/**
	 * @brief 왼쪽 포대.
	 */
	std::unique_ptr<Object> leftPole;

	/**
	 * @brief 왼쪽 포대 피벗.
	 */
	std::unique_ptr<Object> leftPolePivot;

	/**
	 * @brief 오른쪽 포탑.
	 */
	std::unique_ptr<Object> rightTop;

	/**
	 * @brief 오른쪽 포신.
	 */
	std::unique_ptr<Object> rightBarrel;

	/**
	 * @brief 오른쪽 포신 피벗.
	 */
	std::unique_ptr<Object> rightBarrelPivot;

	/**
	 * @brief 오른쪽 포대.
	 */
	std::unique_ptr<Object> rightPole;

	/**
	 * @brief 오른쪽 포대 피벗.
	 */
	std::unique_ptr<Object> rightPolePivot;

	/**
	 * @brief ��ũ �̵� �ӵ�.
	 */
	static constexpr float MOVE_SPEED = 5.0f;

	/**
	 * @brief ��ũ ȸ�� �ӵ�.
	 */
	static constexpr float ROTATE_SPEED = 45.0f;

	/**
	 * @brief �߾� ��ü �ִϸ��̼� Ʈ����.
	 */
	bool isAnimT = false;

	/**
	 * @brief ��� ��ü �ִϸ��̼� Ʈ����.
	 */
	bool isAnimL = false;

	/**
	 * @brief �� ���� y�� ȸ�� �ִϸ��̼� Ʈ����.
	 */
	bool isAnimG = false;

	/**
	 * @brief G �ִϸ��̼� ��� �ð�.
	 */
	float animGTime = 0.0f;

	/**
	 * @brief �� ���� x�� ȸ�� �ִϸ��̼� Ʈ����.
	 */
	bool isAnimP = false;

	float animPTime = 0.0f;

	/**
	 * @brief ��� ��ü�� �̵� Ÿ��.
	 */
	glm::vec3 leftTargetPos;

	/**
	 * @brief �� �ٸ� ��� ��ü�� �̵� Ÿ��
	 */
	glm::vec3 rightTargetPos;

	/**
	 * @brief ȸ���� ���� ���� ��ü.
	 */
	std::unique_ptr<Object> temp;

	std::unique_ptr<Object> left;
	std::unique_ptr<Object> right;
};

#endif // !GUARD_TANK_H