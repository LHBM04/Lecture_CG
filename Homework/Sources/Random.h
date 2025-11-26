#pragma once

#include "PCH.h"

/**
 * @brief 난수 정의 유틸리티.
 */
class Random final
{
public:
	/**
	 * @brief 정수형 난수를 반환합니다.
	 * 
	 * @param min_ 생성할 난수의 최소 범위
	 * @param max_ 생성할 난수의 최대 범위
	 * 
	 * @return int 생성한 난수
	 */
	[[nodiscard]]
	static inline int GetInt(const int min_, const int max_) noexcept
	{
		std::uniform_int_distribution<int> dist(min_, max_);
		return dist(gen);
	}

	/**
	 * @brief 실수형 난수를 반환합니다.
	 * 
	 * @param min_ 생성할 난수의 최소 범위
	 * @param max_ 생성할 난수의 최대 범위
	 * 
	 * @return float 생성한 난수
	 */
	[[nodiscard]]
	static inline float GetFloat(const float min_, const float max_) noexcept
	{
		std::uniform_real_distribution<float> dist(min_, max_);
		return dist(gen);
	}

private:
#pragma region Deleted Functions
	Random() = delete;
	~Random() = delete;
	
	Random(const Random&) = delete;
	Random(Random&&) = delete;
	
	Random& operator=(const Random&) = delete;
	Random& operator=(Random&&) = delete;
	
	void* operator new(std::size_t) = delete;
	void* operator new[](std::size_t) = delete;
	void operator delete(void*) = delete;
	void operator delete[](void*) = delete;
#pragma endregion

	/**
	 * @brief 
	 */
	static std::random_device rd;

	/**
	 * @brief 
	 */
	static std::mt19937 gen;
};