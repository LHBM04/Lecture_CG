#ifndef GUARD_INPUT_MANAGER_H
#define GUARD_INPUT_MANAGER_H

#include <cstddef>
#include <glm/vec2.hpp>

// GLFW 헤더가 필요합니다.
struct GLFWwindow;

/**
 * @brief GLFW 입력을 관리하는 정적 클래스.
 */
class Input final
{
public:
    /**
     * @brief 입력 매니저를 초기화하고 GLFW 콜백을 설정합니다.
     * @param window GLFW 윈도우 포인터.
     */
    static void Init(GLFWwindow* window) noexcept;

    /**
     * @brief 매 프레임 호출되어야 하는 업데이트 함수.
     * (Pressed -> Held, Released -> None 상태 변경)
     */
    static void Update() noexcept;

    /**
     * @brief 키가 '눌린 순간'인지 확인합니다. (해당 프레임만 true)
     */
    [[nodiscard]]
    static bool IsKeyDown(int key) noexcept;

    /**
     * @brief 키가 '떼어진 순간'인지 확인합니다. (해당 프레임만 true)
     */
    [[nodiscard]]
    static bool IsKeyUp(int key) noexcept;

    /**
     * @brief 키를 '누르고 있는 중'인지 확인합니다. (눌린 순간 + 누르고 있는 중)
     */
    [[nodiscard]]
    static bool IsKey(int key) noexcept;

    /**
     * @brief 마우스 버튼이 '눌린 순간'인지 확인합니다.
     */
    [[nodiscard]]
    static bool IsMouseButtonDown(int button) noexcept;

    /**
     * @brief 마우스 버튼이 '떼어진 순간'인지 확인합니다.
     */
    [[nodiscard]]
    static bool IsMouseButtonUp(int button) noexcept;

    /**
     * @brief 마우스 버튼을 '누르고 있는 중'인지 확인합니다.
     */
    [[nodiscard]]
    static bool IsMouseButton(int button) noexcept;

    [[nodiscard]]
    static bool IsModPressed(int mod) noexcept
    {
        // 비트 AND 연산으로 해당 모드 플래그가 켜져 있는지 확인
        return (currentMods & mod) == mod;
    }

    /**
     * @brief 현재 마우스 커서 위치를 반환합니다.
     */
    [[nodiscard]]
    static glm::vec2 GetMousePosition() noexcept;

private:
    /**
     * @brief 입력 상태 (프레임 단위 감지를 위함)
     */
    enum class KeyState : unsigned char
    {
        None,     // 아무것도 안 함
        Pressed,  // '눌린 순간' (이번 프레임)
        Held,     // '누르고 있는 중' (이전부터)
        Released  // '뗀 순간' (이번 프레임)
    };

    // GLFW 콜백 함수
    static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) noexcept;
    static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods) noexcept;
    static void CursorPosCallback(GLFWwindow* window, double xpos, double ypos) noexcept;

    // 상태 저장 배열
    static KeyState keyStates[512]; // GLFW_KEY_LAST가 보통 348이지만 여유있게
    static KeyState mouseButtonStates[16]; // GLFW_MOUSE_BUTTON_LAST가 보통 7
    static glm::vec2 mousePosition;
    static int currentMods;

private:
    // 정적 클래스이므로 생성/삭제/복사 금지
    Input() = delete;
    ~Input() = delete;
    Input(const Input&) = delete;
    Input(Input&&) = delete;
    Input& operator=(const Input&) = delete;
    Input& operator=(Input&&) = delete;
    void* operator new(std::size_t) = delete;
    void* operator new[](std::size_t) = delete;
    void  operator delete(void*) = delete;
    void  operator delete[](void*) = delete;
};

#endif // !GUARD_INPUT_MANAGER_H