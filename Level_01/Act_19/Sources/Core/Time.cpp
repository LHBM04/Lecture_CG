#include "Time.h"

void Time::Initialize() noexcept
{
    lastTime = std::chrono::high_resolution_clock::now();
}

void Time::Update() noexcept
{
    const TimePoint currentTime = std::chrono::high_resolution_clock::now();
    const Duration  duration    = currentTime - lastTime;

    unscaledDeltaTime = duration.count();

    lastTime = currentTime;
}

float Time::timeScale = 1.0f;

float Time::unscaledDeltaTime = 0.0f;

float Time::unscaledFixedDeltaTime = 0.02f;

std::chrono::time_point<std::chrono::high_resolution_clock> Time::lastTime = {};