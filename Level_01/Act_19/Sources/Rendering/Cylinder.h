#ifndef GUARD_CYLINDER_H
#define GUARD_CYLINDER_H

#include "Model.h"

/**
 * @class Cylinder
 *
 * @brief 원기둥 모델을 정의합니다.
 */
class Cylinder final
    : public Model
{
public:
    /**
     * @brief 생성자.
     *
     * @param baseRadius_ 생성할 원기둥의 밑면 반지름.
     * @param topRadius_  생성할 원기둥의 윗면 반지름.
     * @param height_     생성할 원기둥의 높이.
     * @param slices_      생성할 원기둥의 경도.
     * @param stacks_      생성할 원기둥의 위도.
     */
    explicit Cylinder(float baseRadius_,
                      float topRadius_,
                      float height_,
                      int   slices_,
                      int   stacks_) noexcept;

    /**
     * @brief 해당 원기둥을 렌더링합니다.
     */
    virtual void Render() const noexcept override;

    /**
     * @brief 밑면 반지름을 반환합니다.
     *
     * @return float 밑면 반지름.
     */
    [[nodiscard]]
    inline constexpr float GetBaseRadius() const noexcept;

    /**
     * @brief 밑면 반지름을 설정합니다.
     *
     * @param baseRadius_ 설정할 밑면 반지름.
     */
    inline void SetBaseRadius(float baseRadius_) noexcept;

    /**
     * @brief 윗면 반지름을 반환합니다.
     *
     * @return float 윗면 반지름.
     */
    [[nodiscard]]
    constexpr float GetTopRadius() const noexcept;

    /**
     * @brief 윗면 반지름을 설정합니다.
     *
     * @param topRadius_ 설정할 윗면 반지름.
     */
    inline void SetTopRadius(float topRadius_) noexcept;

    /**
     * @brief 원기둥 높이를 반환합니다.
     *
     * @return float 원기둥 높이.
     */
    [[nodiscard]]
    inline constexpr float GetHeight() const noexcept;

    /**
     * @brief 원기둥 높이를 설정합니다.
     *
     * @param height_ 설정할 원기둥 높이.
     */
    inline void SetHeight(float height_) noexcept;

    /**
     * @brief 원기둥의 경도를 반환합니다.
     *
     * @return int 원기둥 경도.
     */
    [[nodiscard]]
    inline constexpr int GetSlices() const noexcept;

    /**
     * @brief 원기둥의 경도를 설정합니다.
     *
     * @param slices_ 설정할 원기둥 경도.
     */
    inline void SetSlices(int slices_) noexcept;

    /**
     * @brief 원기둥의 위도를 반환합니다.
     *
     * @return int 원기둥 위도.
     */
    [[nodiscard]]
    inline constexpr int GetStacks() const noexcept;

    /**
     * @brief 원기둥의 위도를 설정합니다.
     *
     * @param stacks_ 설정할 원기둥 위도.
     */
    inline void SetStacks(int stacks_) noexcept;

private:
    /**
     * @brief 밑면 반지름.
     */
    float baseRadius;

    /**
     * @brief 윗면 반지름.
     */
    float topRadius;

    /**
     * @brief 원기둥 높이.
     */
    float height;

    /**
     * @brief 해당 원기둥의 경도.
     */
    int slices;

    /**
     * @brief 해당 원기둥의 위도.
     */
    int stacks;
};

inline constexpr float Cylinder::GetBaseRadius() const noexcept
{
    return baseRadius;
}

inline void Cylinder::SetBaseRadius(const float baseRadius_) noexcept
{
    baseRadius = baseRadius_;
}

inline constexpr float Cylinder::GetTopRadius() const noexcept
{
    return topRadius;
}

inline void Cylinder::SetTopRadius(const float topRadius_) noexcept
{
    topRadius = topRadius_;
}

inline constexpr float Cylinder::GetHeight() const noexcept
{
    return height;
}

inline void Cylinder::SetHeight(const float height_) noexcept
{
    height = height_;
}

inline constexpr int Cylinder::GetSlices() const noexcept
{
    return slices;
}

inline void Cylinder::SetSlices(const int slices_) noexcept
{
    slices = slices_;
}

inline constexpr int Cylinder::GetStacks() const noexcept
{
    return stacks;
}

inline void Cylinder::SetStacks(const int stacks_) noexcept
{
    stacks = stacks_;
}

#endif // !GUARD_CYLINDER_H