#pragma once

#include "PCH.h"

class Application;

/**
 * @brief 키/버튼 입력을 감지합니다.
 */
class Input final
{
    friend class Application;

public:
    /**
     * @brief 키/버튼 입력 상태를 갱신합니다.
     */
    static void Update() noexcept;

    /**
     * @brief 키가 '눌린 순간'인지 확인합니다. (해당 프레임만 true)
     */
    [[nodiscard]]
    static inline constexpr bool IsKeyPressed(int key) noexcept;

    /**
     * @brief 키가 '떼어진 순간'인지 확인합니다. (해당 프레임만 true)
     */
    [[nodiscard]]
    static inline constexpr bool IsKeyReleased(int key) noexcept;

    /**
     * @brief 키를 '누르고 있는 중'인지 확인합니다. (눌린 순간 + 누르고 있는 중)
     */
    [[nodiscard]]
    static inline constexpr bool IsKeyHeld(int key) noexcept;

    /**
     * @brief 마우스 버튼이 '눌린 순간'인지 확인합니다.
     */
    [[nodiscard]]
    static inline constexpr bool IsMouseButtonPressed(int button) noexcept;

    /**
     * @brief 마우스 버튼이 '떼어진 순간'인지 확인합니다.
     */
    [[nodiscard]]
    static inline constexpr bool IsMouseButtonReleased(int button) noexcept;

    /**
     * @brief 마우스 버튼을 '누르고 있는 중'인지 확인합니다.
     */
    [[nodiscard]]
    static inline constexpr bool IsMouseButtonHeld(int button) noexcept;

    /**
     * @brief 수정 키(Shift, Ctrl, Alt 등)가 눌려있는지 확인합니다.
	 */
    [[nodiscard]]
    static inline constexpr bool IsModified(int mod) noexcept;

    /**
     * @brief 현재 마우스 커서 위치를 반환합니다.
     */
    [[nodiscard]]
    static inline constexpr glm::vec2 GetMousePosition() noexcept;

    /**
     * @brief 마우스 커서의 이동량을 반환합니다.
	 */
    [[nodiscard]]
    static inline constexpr glm::vec2 GetMouseDelta() noexcept;

private:
    /**
     * @brief 키/버튼이 취할 수 있는 상태을 정의합니다.
     */
    enum class KeyState : unsigned char
    {
        /**
         * @brief 아무것도 안 함
         */
        None,

        /**
         * @brief 눌린 순간
         */
        Press,

        /**
         * @brief 눌려 있는 중
         */
        Hold,

        /**
         * @brief 뗀 순간
         */
        Release
    };

#pragma region Deleted Functions
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
#pragma endregion

    /**
     * @brief 키와의 상호작용 발생 시 호출됩니다.
     * 
     * @param key      눌린 키
     * @param scancode 스캔코드
     * @param action   키 액션
     * @param mods     수정키
     */
    static void OnKeyInteract(int key, int scancode, int action, int mods) noexcept;

    /**
	 * @brief 마우스 버튼과의 상호작용 발생 시 호출됩니다.
     * 
     * @param button 눌린 마우스 버튼
     * @param action 버튼 액션
     * @param mods   수정키
     */
    static void OnMouseButtonInteract(int button, int action, int mods) noexcept;

    /**
	 * @brief 마우스 커서가 이동할 때 호출됩니다.
     * 
	 * @param xpos 현재 마우스 커서의 X 좌표
	 * @param ypos 현재 마우스 커서의 Y 좌표
     */
    static void OnCursorMove(double xpos, double ypos) noexcept;

    /**
     * @brief 감지할 수 있는 키의 최대 개수.
     */
    static constexpr std::size_t MAX_KEYS = GLFW_KEY_LAST;

	/**
	 * @brief 감지할 수 있는 마우스 버튼의 최대 개수.
	 */
	static constexpr std::size_t MAX_MOUSE_BUTTONS = GLFW_MOUSE_BUTTON_LAST;

	/**
	 * @brief 
	 */
	static std::array<KeyState, MAX_KEYS> keyStates;

	/**
	 * @brief 
	 */
	static std::array<KeyState, MAX_MOUSE_BUTTONS> mouseButtonStates;

    /**
     * @brief 이전 프레임의 마우스 커서 위치.
	 */
	static glm::vec2 lastMousePosition;

    /**
	 * @brief 현재 프레임의 마우스 커서 위치.
     */
    static glm::vec2 currentMousePosition;

    /**
     * @brief 현재 눌린 
     */
    static int currentMods;
};

inline constexpr bool Input::IsKeyPressed(int key) noexcept
{
    return key >= 0 && key < static_cast<int>(MAX_KEYS) && keyStates[key] == KeyState::Press;
}

inline constexpr bool Input::IsKeyReleased(int key) noexcept
{
    return key >= 0 && key < static_cast<int>(MAX_KEYS) && keyStates[key] == KeyState::Release;
}

inline constexpr bool Input::IsKeyHeld(int key) noexcept
{
    return key >= 0 && key < static_cast<int>(MAX_KEYS) &&
           (keyStates[key] == KeyState::Press || keyStates[key] == KeyState::Hold);
}

inline constexpr bool Input::IsMouseButtonPressed(int button) noexcept
{
    return button >= 0 && button < static_cast<int>(MAX_MOUSE_BUTTONS) &&
           mouseButtonStates[button] == KeyState::Press;
}

inline constexpr bool Input::IsMouseButtonReleased(int button) noexcept
{
    return button >= 0 && button < static_cast<int>(MAX_MOUSE_BUTTONS) &&
           mouseButtonStates[button] == KeyState::Release;
}

inline constexpr bool Input::IsMouseButtonHeld(int button) noexcept
{
    return button >= 0 && button < static_cast<int>(MAX_MOUSE_BUTTONS) &&
           (mouseButtonStates[button] == KeyState::Press || mouseButtonStates[button] == KeyState::Hold);
}

inline constexpr bool Input::IsModified(int mod) noexcept
{
    return (currentMods & mod) == mod;
}

inline constexpr glm::vec2 Input::GetMousePosition() noexcept
{
    return currentMousePosition;
}

inline constexpr glm::vec2 Input::GetMouseDelta() noexcept
{
    return currentMousePosition - lastMousePosition;
}