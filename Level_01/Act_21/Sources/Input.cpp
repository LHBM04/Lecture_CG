#include "Input.h"

#include <cstring>      // memset

// 정적 멤버 변수 초기화
//Input::KeyState Input::keyStates[512];
std::array<Input::KeyState, GLFW_KEY_LAST> Input::keyStates{};
// Input::KeyState Input::mouseButtonStates[16];
std::array<Input::KeyState, GLFW_MOUSE_BUTTON_LAST> Input::mouseButtonStates{};
glm::vec2 Input::mousePosition = glm::vec2(0.0f);
int Input::currentMods = 0;

void Input::Init(GLFWwindow* window) noexcept
{
    // 모든 상태를 'None'으로 초기화
    std::memset(keyStates.data(), 0, sizeof(keyStates));
    std::memset(mouseButtonStates.data(), 0, sizeof(mouseButtonStates));

    // GLFW 콜백 설정
    glfwSetKeyCallback(window, OnKeyInteract);
    glfwSetMouseButtonCallback(window, OnMouseButtonInteract);
    glfwSetCursorPosCallback(window, OnMouseMove);
}

void Input::Update() noexcept
{
	for (auto& keyState : keyStates)
    {
        if (keyState == KeyState::Press)
        {
            keyState = KeyState::Hold;
        }
        else if (keyState == KeyState::Release)
        {
            keyState = KeyState::None;
        }
    }

	for (auto& mouseButtonState : mouseButtonStates)
    {
        if (mouseButtonState == KeyState::Press)
        {
            mouseButtonState = KeyState::Hold;
        }
        else if (mouseButtonState == KeyState::Release)
        {
            mouseButtonState = KeyState::None;
        }
    }
}

bool Input::IsKeyPressed(int key) noexcept
{
    if (key < 0 || key >= 512) return false;
    return keyStates[key] == KeyState::Press;
}

bool Input::IsKeyReleased(int key) noexcept
{
    if (key < 0 || key >= 512) return false;
    return keyStates[key] == KeyState::Release;
}

bool Input::IsKeyHeld(int key) noexcept
{
    if (key < 0 || key >= 512) return false;
    return keyStates[key] == KeyState::Press || keyStates[key] == KeyState::Hold;
}

bool Input::IsMouseButtonDown(int button) noexcept
{
    if (button < 0 || button >= 16) return false;
    return mouseButtonStates[button] == KeyState::Press;
}

bool Input::IsMouseButtonUp(int button) noexcept
{
    if (button < 0 || button >= 16) return false;
    return mouseButtonStates[button] == KeyState::Release;
}

bool Input::IsMouseButton(int button) noexcept
{
    if (button < 0 || button >= 16) return false;
    return mouseButtonStates[button] == KeyState::Press || mouseButtonStates[button] == KeyState::Hold;
}

glm::vec2 Input::GetMousePosition() noexcept
{
    return mousePosition;
}

//--- GLFW Callbacks ---//

void Input::OnKeyInteract(GLFWwindow* window, int key, int scancode, int action, int mods) noexcept
{
    currentMods = mods;

    if (key < 0 || key >= 512) return; // 범위를 벗어난 키 무시

    if (action == GLFW_PRESS)
    {
        keyStates[key] = KeyState::Press;
    }
    else if (action == GLFW_RELEASE)
    {
        keyStates[key] = KeyState::Release;
    }
}

void Input::OnMouseButtonInteract(GLFWwindow* window, int button, int action, int mods) noexcept
{
    currentMods = mods;

    if (button < 0 || button >= 16) return;

    if (action == GLFW_PRESS)
    {
        mouseButtonStates[button] = KeyState::Press;
    }
    else if (action == GLFW_RELEASE)
    {
        mouseButtonStates[button] = KeyState::Release;
    }
}

void Input::OnMouseMove(GLFWwindow* window, double xpos, double ypos) noexcept
{
    mousePosition.x = static_cast<float>(xpos);
    mousePosition.y = static_cast<float>(ypos);
}