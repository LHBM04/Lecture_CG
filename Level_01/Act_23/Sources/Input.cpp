#include "Input.h"

#include <GLFW/glfw3.h> // GLFW API 및 키 코드 사용
#include <cstring>      // memset

// 정적 멤버 변수 초기화
Input::KeyState Input::keyStates[512];
Input::KeyState Input::mouseButtonStates[16];
glm::vec2 Input::mousePosition = glm::vec2(0.0f);
int Input::currentMods = 0;

void Input::Init(GLFWwindow* window) noexcept
{
    // 모든 상태를 'None'으로 초기화
    memset(keyStates, 0, sizeof(keyStates));
    memset(mouseButtonStates, 0, sizeof(mouseButtonStates));

    // GLFW 콜백 설정
    glfwSetKeyCallback(window, OnKeyInteract);
    glfwSetMouseButtonCallback(window, OnMouseButtonInteract);
    glfwSetCursorPosCallback(window, OnCursorMove);
}

void Input::Update() noexcept
{
    // 'Press' 상태를 'Held'로 변경
    // 'Release' 상태를 'None'으로 변경
    // -> 'Down'과 'Up' 이벤트가 정확히 한 프레임 동안만 true가 되도록 보장

    for (int i = 0; i < 512; ++i)
    {
        if (keyStates[i] == KeyState::Press)
        {
            keyStates[i] = KeyState::Hold;
        }
        else if (keyStates[i] == KeyState::Release)
        {
            keyStates[i] = KeyState::None;
        }
    }

    for (int i = 0; i < 16; ++i)
    {
        if (mouseButtonStates[i] == KeyState::Press)
        {
            mouseButtonStates[i] = KeyState::Hold;
        }
        else if (mouseButtonStates[i] == KeyState::Release)
        {
            mouseButtonStates[i] = KeyState::None;
        }
    }
}

//--- Public Getters ---//

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

void Input::OnCursorMove(GLFWwindow* window, double xpos, double ypos) noexcept
{
    mousePosition.x = static_cast<float>(xpos);
    mousePosition.y = static_cast<float>(ypos);
}