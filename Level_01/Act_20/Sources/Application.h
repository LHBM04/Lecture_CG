#ifndef GUARD_PROGRAM_H
#define GUARD_PROGRAM_H

#include <functional>

/**
 * @class Application
 * 
 * @brief 애플리케이션을 정의합니다.
 */
class Application final
{
	/**
	 * @brief 애플리케이션이 로드될 때 호출되는 콜백.
	 */
	using StartCallback = std::function<void()>;

	/**
	 * @brief 애플리케이션이 유휴 상태일 때 호출되는 콜백.
	 */
	using UpdateCallback = std::function<void()>;

	/**
	 * @brief 애플리케이션이 렌더링될 때 호출되는 콜백.
	 */
	using RenderCallback = std::function<void()>;

	/**
	 * @brief 애플리케이션이 종료될 때 호출되는 콜백.
	 */
	using QuitCallback = std::function<void()>;

public:
	/**
	 * @struct Config
	 * 
	 * @brief 프로그램의 설정을 정의합니다.
	 */
	struct Config final
	{
		/**
		 * @brief 
		 */
		int argc;

		/**
		 * @brief 
		 */
		char** argv;

		/**
		 * @brief 애플리케이션의 초당 프레임 수.
		 */
		int fps;

		/**
		 * @brief 애플리케이션의 로드 콜백 함수.
		 */
		StartCallback onStart;

		/**
		 * @brief 애플리케이션의 업데이트 콜백 함수.
		 */
		UpdateCallback onUpdate;

		/**
		 * @brief 애플리케이션의 렌더 콜백 함수.
		 */
		RenderCallback onRender;

		/**
		 * @brief 애플리케이션의 종료 콜백 함수.
		 */
		QuitCallback onQuit;
	};

	/**
	 * @brief 프로그램을 실행합니다.
	 * 
	 * @param config_ 프로그램 설정
	 */
	static int Run(const Config& config_) noexcept;

	/**
	 * @brief 프로그램을 종료합니다.
	 */
	static void Quit() noexcept;

	/**
	 * @brief 프로그램의 설정을 반환합니다.
	 * 
	 * @return Config 프로그램 설정
	 */
	inline Config GetConfig() const noexcept;

	/**
	 * @brief 프로그램의 설정을 설정합니다.
	 * 
	 * @param config_ 프로그램 설정
	 */
	inline void SetConfig(const Config& config_) noexcept;

private:
	/**
	 * @brief 애플리케이션이 로드될 때 호출됩니다.
	 */
	static void OnLoad() noexcept;

	/**
	 * @brief 애플리케이션이 유휴 상태일 때 호출됩니다.
	 * 
	 * @param value_ 타이머 값
	 */
	static void OnTimer(int value_) noexcept;

	/**
	 * @brief 애플리케이션이 렌더링될 때 호출됩니다.
	 */
	static void OnDisplay() noexcept;

	/**
	 * @brief 애플리케이션이 종료될 때 호출됩니다.
	 */
	static void OnClose() noexcept;

	/**
	 * @brief 프로그램 설정.
	 */
	static Config config;
};

inline Application::Config Application::GetConfig() const noexcept
{
	return config;
}

inline void Application::SetConfig(const Config& config_) noexcept
{
	config = config_;
}

#endif // GUARD_PROGRAM_H