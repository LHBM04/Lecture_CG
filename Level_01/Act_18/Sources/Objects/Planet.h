#ifndef GUARD_PLANET_H
#define GUARD_PLANET_H

#include "Object.h"

class Planet final
    : public Object
{
public:
    /**
     * @brief 생성자.
     */
    explicit Planet() noexcept;

    /**
     * @brief 해당 오브젝트를 업데이트합니다.
     */
    virtual void Update() noexcept override;

    /**
     * @brief 해당 오브젝트의 선택 상태를 반환합니다.
     *
     * @return bool 해당 오브젝트의 선택 상태.
     */
    [[nodiscard]]
    inline constexpr bool IsSelected() const noexcept;

    /**
     * @brief 해당 오브젝트의 선택 상태를 설정합니다.
     *
     * @param isSelected_ 설정할 선택 상태.
     */
    inline void SetSelected(const bool selected) noexcept;

private:
    /**
     * @brief 해당 오브젝트를 이동시킵니다.
     */
    void Translate() noexcept;

    /**
     * @brief 해당 오브젝트를 회전시킵니다.
     */
    void Rotate() noexcept;

    /**
     * @brief 해당 오브젝트의 크기를 조절합니다.
     */
    void Scale() noexcept;

    /**
     * @brief 해당 오브젝트의 선택 상태.
     */
    bool isSelected;

    /**
     * @brief 해당 오브젝트의 이동 속도.
     */
    float translateFactor;

    /**
     * @brief 해당 오브젝트의 회전 속도.
     */
    float rotateFactor;

    /**
     * @brief 해당 오브젝트의 크기 조절 속도.
     */
    float scaleFactor;
};

inline constexpr bool Planet::IsSelected() const noexcept
{
    return isSelected;
}

inline void Planet::SetSelected(const bool selected) noexcept
{
    isSelected = selected;
}

#endif // !GUARD_PLANET_H