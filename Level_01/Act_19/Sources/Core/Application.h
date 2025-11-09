#ifndef GUARD_APPLICATION_H
#define GUARD_APPLICATION_H

#include <functional>
#include <memory>
#include <string>

class Input;
class Time;

class Application final
{
    friend class Input;
    friend class Time;
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
        std::function<void()> onStart;

        /**
         * @brief 고정 시간마다 호출됩니다.
         */
        std::function<void()> onFixedUpdate;

        /**
         * @brief 매 프레임마다 호출됩니다.
         */
        std::function<void()> onUpdate;

        /**
         * @brief 프레임 렌더링이 끝난 후 호출됩니다.
         */
        std::function<void()> onLateUpdate;

        /**
         * @brief 렌더링될 때 호출됩니다.
         */
        std::function<void()> onRender;
    };

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

    /**
     * @brief 애플리케이션의 현재 사양을 반환합니다.
     *
     * @return const Application::Specification& 애플리케이션의 현재 사양.
     */
    [[nodiscard]]
    static inline const Application::Specification& GetSpecification() noexcept;

private:
    Application() = delete;
    ~Application() = delete;

    Application(const Application&) = delete;
    Application(Application&&) = delete;

    Application& operator=(const Application&) = delete;
    Application& operator=(Application&&) = delete;

    void* operator new(std::size_t) = delete;
    void* operator new[](std::size_t) = delete;
    void  operator delete(void*) = delete;
    void  operator delete[](void*) = delete;

    /**
     * @brief 애플리케이션이 업데이트될 때 호출됩니다.
     */
    static void OnUpdate() noexcept;

    /**
     * @brief 애플리케이션이 그려질 때 호출됩니다.
     */
    static void OnDisplay() noexcept;

    /**
     * @brief 애플리케이션의 크기가 재설정되었을 때 호출됩니다.
     *
     * @param width_  애플리케이션의 새 너비
     * @param height_ 애플리케이션의 새 높이
     */
    static void OnReshape(int width_, int height_) noexcept;

    /**
     * @brief 해당 애플리케이션의 사양.
     */
    static Application::Specification specification;

    /**
     * @brief 고정 업데이트 호출을 위한 타이머.
     */
    static float fixedUpdateTime;
};

inline const Application::Specification& Application::GetSpecification() noexcept
{
    return specification;
}

#endif // !GUARD_APPLICATION_H
