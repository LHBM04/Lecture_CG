#ifndef GUARD_PLANET_H
#define GUARD_PLANET_H

#include "Object.h"

class Planet final
    : public Object
{
public:
    /**
     * @struct Properties
     *
     * @brief 행성이 가지는 속성을 정의합니다.
     */
    struct Properties
    {
        /**
         * @brief 공전 주체.
         */
        Object* parent;

        /**
         * @brief 공전 거리.
         */
        float distance;

        /**
         * @brief 공전 각도.
         */
        float angle;

        /**
         * @brief 공전 속도.
         */
        float speed;
    };

    /**
     * @brief 생성자.
     *
     * @param properties_ 생성할 행성의 속성.
     */
    explicit Planet(const Planet::Properties& properties_) noexcept;

    /**
     * @brief 해당 오브젝트를 업데이트합니다.
     */
    virtual void Update() noexcept override;

    /**
     * @brief 해당 오브젝트의 공전 정보를 반환합니다.
     *
     * @return RevolveInfo 해당 오브젝트의 공전 정보.
     */
    [[nodiscard]]
    inline constexpr Properties GetInfo() const noexcept;

    /**
     * @brief 해당 오브젝트의 공전 정보를 설정합니다.
     *
     * @param info_ 설정할 공전 정보.
     */
    inline void SetInfo(const Properties& info_) noexcept;

private:
    /**
     * @brief 해당 오브젝트의 공전 정보.
     */
    Planet::Properties properties;
};

inline constexpr Planet::Properties Planet::GetInfo() const noexcept
{
    return properties;
}

inline void Planet::SetInfo(const Planet::Properties& info_) noexcept
{
    properties = info_;
}

#endif // !GUARD_PLANET_H