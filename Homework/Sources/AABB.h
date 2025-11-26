#pragma once

#include "PCH.h"

/**
 * @brief 충돌 박스를 정의합니다.
 */
struct AABB final
{
    /**
     * @brief 
     */
    glm::vec3 min;

    /**
     * @brief 
     */
    glm::vec3 max;

    /**
     * @brief 지정한 두 객체 간의 충돌 여부를 반환합니다.
     * 
     * @param lhs_ 검사할 첫 번째 객체
     * @param rhs_ 검사할 두 번째 객체
     * 
	 * @return bool 충돌 여부
     */
    [[nodiscard]]
    static inline bool CheckCollision(const AABB& lhs_, const AABB& rhs_)
    {
        return (lhs_.min.x <= rhs_.max.x && lhs_.max.x >= rhs_.min.x) &&
               (lhs_.min.y <= rhs_.max.y && lhs_.max.y >= rhs_.min.y) &&
               (lhs_.min.z <= rhs_.max.z && lhs_.max.z >= rhs_.min.z);
    }
};