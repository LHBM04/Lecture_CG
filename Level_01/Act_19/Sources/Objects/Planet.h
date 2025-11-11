// Act_19/Sources/Objects/Planet.h

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
    explicit Planet(
        Object* const parent_   = nullptr,
        const float   distance_ = 0.0f,
        const float   angle_    = 0.0f,
        const float   speed_    = 0.0f,
		const float   tilt_     = 0.0f
    ) noexcept;

    /**
     * @brief 해당 오브젝트를 업데이트합니다.
     */
    virtual void Update() noexcept override;

    [[nodiscard]]
    inline Object* const GetParent() const noexcept
    {
        return mParent;
	}

    inline void SetParent(Object* const parent_) noexcept
    {
        mParent = parent_;
    }

    [[nodiscard]]
    inline constexpr float GetDistance() const noexcept
    {
        return mDistance;
	}

    inline void SetDistance(const float distance_) noexcept
    {
        mDistance = distance_;
    }

	[[nodiscard]]
    inline constexpr float GetAngle() const noexcept
    {
        return mAngle;
	}

    inline void SetAngle(const float angle_) noexcept
    {
        mAngle = angle_;
	}

    [[nodiscard]]
    inline constexpr float GetSpeed() const noexcept
    {
        return mSpeed;
    }

    inline void SetSpeed(const float speed_) noexcept
    {
        mSpeed = speed_;
	}

    [[nodiscard]]
    inline constexpr float GetTilt() const noexcept
    {
        return mTilt;
    }

    inline void SetTilt(const float tilt_) noexcept
    {
        mTilt = tilt_;
	}

private:
    /**
     * @brief 해당 행성의 부모.
     */
    Object* mParent;

    /**
     * @brief 해당 행성의 공전 거리.
     */
    float mDistance;

    /**
     * @brief 해당 행성의 공전 각도.
     */
    float mAngle;

    /**
     * @brief 해당 행성의 공전 속도.
     */
    float mSpeed;

    /**
     * @brief 해당 행성의 공전 기울기.
     */
    float mTilt;
};

#endif // !GUARD_PLANET_H