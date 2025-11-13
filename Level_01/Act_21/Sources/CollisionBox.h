#ifndef GUARD_COLLISIONBOX_H
#define GUARD_COLLISIONBOX_H

#include <glm/vec2.hpp>
#include <glm/ext.hpp>
#include <cmath>

struct Rect final
{
	/**
	 * @brief 위치 (사각형 중심)
	 */
	glm::vec2 position;

	/**
	 * @brief 회전 각도 (라디안)
	 */
	float angle;

	/**
	 * @brief 크기 (사각형 한 변의 길이)
	 */
	float size;
};

// rect_.angle 은 라디안 단위, rect_.size 는 한 변의 길이로 가정합니다.
// position_ 가 rect_ 내부에 있으면 true 를 반환합니다.
static inline bool IsCollision(const Rect& rect_, const glm::vec2& position_) noexcept
{
	// 점을 사각형의 로컬 좌표계로 변환: 먼저 중심 기준으로 평행이동, 그 다음 -angle 만큼 회전
	const glm::vec2 d = position_ - rect_.position;
	const float c = std::cos(-rect_.angle);
	const float s = std::sin(-rect_.angle);
	const glm::vec2 local = glm::vec2(d.x * c - d.y * s, d.x * s + d.y * c);

	const float half = rect_.size * 0.5f;
	return (local.x >= -half && local.x <= half) && (local.y >= -half && local.y <= half);
}

#endif // !GUARD_COLLISIONBOX_H