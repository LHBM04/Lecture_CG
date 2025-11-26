#pragma once

#include "PCH.h"

struct GLFWwindow;

/**
 * @brief 애플리케이션을 정의합니다.
 */
class Application final
{
public:
	/**
	 * @brief 애플리케이션 내 사용될 리소스를 불러오는 콜백을 정의합니다.
	 */
	using LoadCallback = std::function<void()>;

	/**
	 * @brief 애플리케이션이 매 프레임마다 업데이트되는 콜백을 정의합니다.
	 */
	using TickCallback = std::function<void(const float)>;

	/**
	 * @brief 애플리케이션이 매 프레임마다 그려지는 콜백을 정의합니다.
	 */
	using DisplayCallback = std::function<void()>;

	/**
	 * @brief 애플리케이션이 닫힐 때 호출되는 콜백을 정의합니다.
	 */
	using CloseCallback = std::function<void()>;

	/**
	 * @brief 애플리케이션 설정을 정의합니다.
	 */
	struct Configuration final
	{
		/**
		 * @brief 창의 너비.
		 */
		int width;

		/**
		 * @brief 창의 높이.
		 */
		int height;

		/**
		 * @brief 창의 제목.
		 */
		const char* title;

		/**
		 * @brief 전체 화면 모드 여부.
		 */
		bool shouldFullscreen;

		/**
		 * @brief 창 테두리 표시 여부.
		 */
		bool shouldDecorate;

		/**
		 * @brief 창 크기 조절 가능 여부.
		 */
		bool shouldResizable;

		/**
		 * @brief 수직 동기화(VSync) 사용 여부.
		 */
		bool shouldVSync;

		/**
		 * @brief 창이 켜질 때 호출되는 콜백 함수.
		 */
		LoadCallback onLoad;

		/**
		 * @brief 매 프레임마다 호출되는 콜백 함수.
		 */
		TickCallback onTick;

		/**
		 * @brief 창이 그려질 때 호출되는 콜백 함수.
		 */
		DisplayCallback onDisplay;

		/**
		 * @brief 창이 닫힐 때 호출되는 콜백 함수.
		 */
		CloseCallback onClose;
	};

	/**
	 * @brief 애플리케이션 설정을 반환합니다.
	 * 
	 * @return Application::Configuration& 애플리케이션 설정
	 */
	[[nodiscard]]
	static inline constexpr Application::Configuration& GetSpecification() noexcept;

	/**
	 * @brief 애플리케이션을 실행합니다.
	 * 
	 * @param configuration_ 실행할 애플리케이션의 설정
	 * 
	 * @return int 종료 코드
	 */
	static int Run(const Application::Configuration& configuration_) noexcept;

	/**
	 * @brief 애플리케이션을 종료합니다.
	 * 
	 * @param exitCode_ 종료 코드
	 */
	static void Quit(const int exitCode_) noexcept;

private:
#pragma region Deleted Functions
	Application() = delete;
	~Application() = delete;

	Application(const Application&) = delete;
	Application(Application&&) = delete;

	Application& operator=(const Application&) = delete;
	Application& operator=(Application&&) = delete;

	void* operator new(std::size_t) = delete;
	void* operator new[](std::size_t) = delete;
	void operator delete(void*) = delete;
	void operator delete[](void*) = delete;
#pragma endregion

	/**
	 * @brief 애플리케이션을 초기화합니다.
	 * 
	 * @return bool 초기화 성공 여부
	 */
	static bool Initialize() noexcept;

	/**
	 * @brief 애플리케이션 내 리소스를 불러옵니다.
	 */
	static void Load() noexcept;

	/**
	 * @brief 애플리케이션을 업데이트합니다.
	 */
	static void Tick(const float deltaTime_) noexcept;

	/**
	 * @brief 애플리케이션을 그립니다.
	 */
	static void Display() noexcept;

	/**
	 * @brief 애플리케이션 내 리소스를 정리하고, 애플리케이션을 닫습니다.
	 */
	static void Close() noexcept;

	/**
	 * @brief 애플리케이션 설정.
	 */
	static Application::Configuration configuration;

	/**
	* @brief GLFW 윈도우 핸들.
	*/
	static GLFWwindow* window;
};

inline constexpr Application::Configuration& Application::GetSpecification() noexcept
{
	return configuration;
}