#ifndef GUARD_TIME_H
#define GUARD_TIME_H

#include <chrono>
#include <cstddef>

class Application;

/**
 * @class Time
 *
 * @brief 시간을 관리합니다.
 */
class Time
{
    friend class Application;
public:
    /**
     * @brief 이전 프레임과 현재 프레임 사이의 간격을 반환합니다.
     *
     * @return float 이전 프레임과 현재 프레임 사이의 간격.
     */
    [[nodiscard]]
    static inline float GetDeltaTime() noexcept;

    /**
     * @brief 이전 프레임과 현재 프레임 사이의 비율이 조정되지 않은 간격을 반환합니다.
     *
     * @return float 이전 프레임과 현재 프레임 사이의 비율이 조정되지 않은 간격.
     */
    [[nodiscard]]
    static inline float GetUnscaledDeltaTime() noexcept;

    /**
     * @brief 고정된 시간 간격을 반환합니다.
     *
     * @return float 고정된 시간 간격.
     */
    [[nodiscard]]
    static inline float GetFixedDeltaTime();

    /**
     * @brief 비율이 조정되지 않은 고정된 시간 간격을 반환합니다.
     *
     * @return float 비율이 조정되지 않은 고정된 시간 간격.
     */
    [[nodiscard]]
    static inline float GetUnscaledFixedDeltaTime();

    /**
     * @brief 시간 배율을 반환합니다.
     *
     * @return float 시간 배율.
     */
    [[nodiscard]]
    static inline float GetTimeScale() noexcept;

    /**
     * @brief 경과된 시간을 반환합니다.
     *
     * @return float 경과된 시간.
     */
    [[nodiscard]]
    static inline float GetElapsedTime() noexcept;

    /**
     * @brief 시간 배율을 설정합니다.
     *
     * @param value_ 설정할 시간 배율.
     */
    static inline void SetTimeScale(const float value_) noexcept;

    /**
     * @brief 비율이 조정되지 않은 고정된 시간 간격을 설정합니다.
     *
     * @param value_ 설정할 비율이 조정되지 않은 고정된 시간 간격.
     */
    static inline void SetUnscaledFixedDeltaTime(const float value_) noexcept;

private:
    using TimePoint = std::chrono::time_point<std::chrono::high_resolution_clock>;
    using Duration  = std::chrono::duration<float>;

    Time() = delete;
    ~Time() = delete;

    Time(const Time&) = delete;
    Time(Time&&) = delete;

    Time& operator=(const Time&) = delete;
    Time& operator=(Time&&) = delete;

    void* operator new(std::size_t) = delete;
    void* operator new[](std::size_t) = delete;
    void  operator delete(void*) = delete;
    void  operator delete[](void*) = delete;

    /**
     * @brief 시간을 초기화합니다.
     */
    static void Initialize() noexcept;

    /**
     * @brief 시간을 갱신합니다.
     */
    static void Update() noexcept;

    /**
     * @brief 시간 배율.
     */
    static float timeScale;

    /**
     * @brief 비율이 조정되지 않은 이전 프레임과 현재 프레임 사이의 간격.
     */
    static float unscaledDeltaTime;

    /**
     * @brief 비율이 조정되지 않은 고정된 시간 간격.
     */
    static float unscaledFixedDeltaTime;

    /**
     * @brief 마지막 시간.
     */
    static TimePoint lastTime;
};

inline float Time::GetDeltaTime() noexcept
{
    return unscaledDeltaTime * timeScale;
}

inline float Time::GetUnscaledDeltaTime() noexcept
{
    return unscaledDeltaTime;
}

inline float Time::GetFixedDeltaTime()
{
    return unscaledFixedDeltaTime * timeScale;
}

inline float Time::GetUnscaledFixedDeltaTime()
{
    return unscaledFixedDeltaTime;
}

inline float Time::GetTimeScale() noexcept
{
    return timeScale;
}

inline float Time::GetElapsedTime() noexcept
{
    const TimePoint currentTime = std::chrono::high_resolution_clock::now();
    const Duration  duration    = currentTime - lastTime;

    return duration.count();
}

inline void Time::SetTimeScale(const float value_) noexcept
{
    timeScale = value_;
}

inline void Time::SetUnscaledFixedDeltaTime(const float value_) noexcept
{
    unscaledFixedDeltaTime = value_;
}

#endif // !GUARD_TIME_H