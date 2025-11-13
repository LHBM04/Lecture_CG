#ifndef GUARD_INPUT_MANAGER_H
#define GUARD_INPUT_MANAGER_H

#include <array>

#include <GLFW/glfw3.h>

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
     * 
     * @param window GLFW 윈도우 포인터.
     */
    static void Init(GLFWwindow* window) noexcept;

    /**
     * @brief 매 프레임 호출되어야 하는 업데이트 함수.
     */
    static void Update() noexcept;

    /**
     * @brief 키가 '눌린 순간'인지 확인합니다. (해당 프레임만 true)
     */
    [[nodiscard]]
    static bool IsKeyPressed(int key) noexcept;

    /**
     * @brief 키가 '떼어진 순간'인지 확인합니다. (해당 프레임만 true)
     */
    [[nodiscard]]
    static bool IsKeyReleased(int key) noexcept;

    /**
     * @brief 키를 '누르고 있는 중'인지 확인합니다. (눌린 순간 + 누르고 있는 중)
     */
    [[nodiscard]]
    static bool IsKeyHeld(int key) noexcept;

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

    /**
     * @brief 대충 만든 마우스 위치 변화량.
     * 
	 * @return glm::vec2 마우스 위치 변화량
     */
    [[nodiscard]]
    static glm::vec2 GetMouseDelta() noexcept
    {
        static glm::vec2 lastPosition = mousePosition;
        glm::vec2 delta = mousePosition - lastPosition;
        lastPosition = mousePosition;
        return delta;
	}

private:
    /**
	* @class KeyState
    * 
    * @brief 입력 상태를 정의합니다.
    */
    enum class KeyState : unsigned char
    {
        /**
         * @brief 아무 상태도 아님.
         */
        None,

        /**
         * @brief 눌린 상태.
         */
        Press,

        /**
		 * @brief 눌려져 있는 상태.
         */
        Hold,

        /**
         * @brief 뗀 상태.
         */
        Release
    };

    Input() = delete;
    ~Input() = delete;
    
    Input(const Input&) = delete;
    Input(Input&&) = delete;
    
    Input& operator=(const Input&) = delete;
    Input& operator=(Input&&) = delete;
    
    void* operator new(std::size_t) = delete;
    void* operator new[](std::size_t) = delete;
    void operator delete(void*) = delete;
    void operator delete[](void*) = delete;

    static void OnKeyInteract(GLFWwindow* window, int key, int scancode, int action, int mods) noexcept;
    static void OnMouseButtonInteract(GLFWwindow* window, int button, int action, int mods) noexcept;
    static void OnMouseMove(GLFWwindow* window, double xpos, double ypos) noexcept;

    /**
     * @brief 키 상태.
     */
    static std::array<KeyState, GLFW_KEY_LAST> keyStates;

    /**
     * @brief 마우스 버튼 상태.
     */
    static std::array<KeyState, GLFW_MOUSE_BUTTON_LAST> mouseButtonStates;

    /**
     * @brief 마우스 커서의 위치.
     */
    static glm::vec2 mousePosition;

    /**
     * @brief 현재 활성화된 수정자 키 플래그.
	 */
    static int currentMods;
};

#endif // !GUARD_INPUT_MANAGER_H