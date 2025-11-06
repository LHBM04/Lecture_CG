// Act_19/Sources/Objects/Planet.h

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
        // ... (기존 멤버)
        Object* parent;
        float distance;
        float angle;
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
    inline constexpr Properties GetInfo() const noexcept
    {
        return properties;
    }

    /**
     * @brief 해당 오브젝트의 공전 정보를 설정합니다.
     *
     * @param info_ 설정할 공전 정보.
     */
    inline void SetInfo(const Properties& info_) noexcept
    {
        properties = info_;
    }

    /**
     * @brief 모든 행성의 공전 궤도 Z축 기울기.
     * Main.cpp에서 이 값을 변경합니다.
     */
    static float s_OrbitTiltZ;

private:
    /**
     * @brief 해당 오브젝트의 공전 정보.
     */
    Planet::Properties properties;
};

// ... (기존 인라인 함수들)

#endif // !GUARD_PLANET_H