#ifndef GUARD_SPHERE_H
#define GUARD_SPHERE_H

#include "Model.h"

/**
 * @class Sphere
 *
 * @brief 구 모델을 정의합니다.
 */
class Sphere final
    : public Model
{
public:
    /**
     * @brief 생성자.
     *
     * @param radius_ 생성할 구의 반지름.
     * @param slices_ 생성할 구의 경도.
     * @param stacks_ 생성할 구의 위도.
     */
    explicit Sphere(float radius_ = 1.0f,
                    int   slices_ = 16,
                    int   stacks_ = 16) noexcept;

    /**
     * @brief 렌더링 시 호출됩니다.
     */
    virtual void Render() const noexcept override;

    /**
     * @brief 구의 반지름을 반환합니다.
     *
     * @return float 구의 반지름.
     */
    [[nodiscard]]
    inline constexpr float GetRadius() const noexcept;

    /**
     * @brief 구의 반지름을 설정합니다.
     *
     * @param radius_ 설정할 구의 반지름.
     */
    inline void SetRadius(float radius_) noexcept;

    /**
     * @brief 구의 경도를 반환합니다.
     *
     * @return int 구의 경도.
     */
    [[nodiscard]]
    inline constexpr int GetSlices() const noexcept;

    /**
     * @brief 구의 경도를 설정합니다.
     *
     * @param slices_ 설정할 구의 경도.
     */
    inline void SetSlices(int slices_) noexcept;

    /**
     * @brief 구의 위도를 설정합니다.
     *
     * @param stacks_ 설정할 구의 위도.
     */
    [[nodiscard]]
    inline constexpr int GetStacks() const noexcept;

    /**
     * @brief 구의 위도를 설정합니다.
     *
     * @param stacks_ 설정할 구의 위도.
     */
    inline void SetStacks(int stacks_) noexcept;

private:
    /**
     * @brief 구의 반지름.
     */
    float radius;

    /**
     * @brief 구의 경도
     */
    int slices;

    /**
     * @brief 구의 위도.
     */
    int stacks;
};

inline constexpr float Sphere::GetRadius() const noexcept
{
    return radius;
}

inline void Sphere::SetRadius(const float radius_) noexcept
{
    radius = radius_;
}

inline constexpr int Sphere::GetSlices() const noexcept
{
    return slices;
}

inline void Sphere::SetSlices(const int slices_) noexcept
{
    slices = slices_;
}

inline constexpr int Sphere::GetStacks() const noexcept
{
    return stacks;
}

inline void Sphere::SetStacks(const int stacks_) noexcept
{
    stacks = stacks_;
}

#endif // !GUARD_SPHERE_H