#ifndef GUARD_APPLICATION_H
#define GUARD_APPLICATION_H

#include <functional>
#include <string>

struct GLFWwindow;

/**
 * @brief 애플리케이션을 정의합니다.
 */
class Application final
{
	using StartCallback = std::function<void()>;
	using UpdateCallback = std::function<void(float)>;
	using RenderCallback = std::function<void()>;
	using CloseCallback = std::function<void()>;
public:
    /**
    * @brief 애플리케이션 설정을 정의합니다.
    */
    struct Specification final
    {
        /**
         * @brief OpenGL 컨텍스트 상위 버전.
         */
        int majorVersion;

        /**
         * @brief OpenGL 컨텍스트 하위 버전.
         */
        int minorVersion;

        /**
         * @brief 애플리케이션 타이틀.
         */
        std::string name;

        /**
         * @brief 애플리케이션 너비.
         */
        int width;

        /**
         * @brief 애플리케이션 높이.
         */
        int height;

        /**
         * @brief 애플리케이션의 업데이트 주기.
         */
        int fps;

        /**
         * @brief 애플리케이션이 실행될 때 시작합니다.
         */
        StartCallback onStart;

        /**
         * @brief 매 프레임마다 호출됩니다.
         */
        UpdateCallback onUpdate;

        /**
         * @brief 렌더링될 때 호출됩니다.
         */
        RenderCallback onRender;

		/**
		 * @brief 애플리케이션이 종료될 때 호출됩니다.
		 */
		CloseCallback onClose;
    };

    /**
     * @brief 애플리케이션 설정을 반환합니다.
     * 
	 * @return const Application::Specification& 애플리케이션 설정
     */
    [[nodiscard]]
	static inline const Application::Specification& GetSpecification() noexcept;

	/**
	 * @brief 애플리케이션을 실행합니다.
     * 
	 * @param specification_ 애플리케이션 설정
	 */
	static int Run(const Application::Specification& specification_) noexcept;

	/**
	 * @brief 애플리케이션을 종료합니다.
	 */
	static void Quit() noexcept;

private:
	/**
	 * @brief 윈도우를 초기화합니다.
	 * 
	 * @return 성공 여부를 초기화합니다.
	 */
	static bool InitWindow() noexcept;

	/**
	 * @brief 그래픽스 API를 초기화합니다.
	 * 
	 * @return bool 성공 여부
	 */
	static bool InitGraphics() noexcept;

	/**
	 * @brief 애플리케이션 설정.
	 */
	static Application::Specification specification;

	/**
	 * @brief 윈도우 핸들.
	 */
	static GLFWwindow* window;
};

inline const Application::Specification& Application::GetSpecification() noexcept
{
    return specification;
}

#endif // !GUARD_APPLICATION_H