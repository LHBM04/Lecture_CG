#ifndef GUARD_TANK_H
#define GUARD_TANK_H

#include <memory>

#include <glm/vec3.hpp>

#include "Object.h"

/**
 * @brief ��ũ�� �����մϴ�.
 */
class Tank final
	: public Object
{
public:
	/**
	 * @brief ������.
	 *
	 * @param position_ ������ ��ũ�� ��ġ
	 * @param rotation_ ������ ��ũ�� ȸ��
	 * @param scale_    ������ ��ũ�� ũ��
	 */
	explicit Tank(
		const glm::vec3& position_ = glm::vec3{ 0.0f, 0.0f, 0.0f },
		const glm::vec3& rotation_ = glm::vec3{ 0.0f, 0.0f, 0.0f },
		const glm::vec3& scale_    = glm::vec3{ 1.0f, 1.0f, 1.0f }
	) noexcept;

	/**
	 * @brief �Ҹ���.
	 */
	virtual ~Tank() noexcept override;

	/**
	 * @brief �ش� ������ ������Ʈ�մϴ�.
	 * 
	 * @param deltaTime_ ���� �����Ӱ� ���� ������ ������ �ð� ����
	 */
	virtual void Update(const float deltaTime_) noexcept override;

	/**
	 * @brief �ش� ������ �������մϴ�.
	 * 
	 * @param shader_     ����� ���̴�
	 * @param renderMode_ ������ ���
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