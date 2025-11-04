#include "Input.h"

#include <gl/freeglut_std.h>

void Input::Update()
{
    previousKeyStates         = currentKeyStates;
    previousMouseButtonStates = currentMouseButtonStates;
    previousMousePosition     = currentMousePosition;
}

bool Input::IsKeyPressed(const int key_) noexcept
{
    return currentKeyStates[key_] && !previousKeyStates[key_];
}

bool Input::IsKeyHeld(const int key_) noexcept
{
    return currentKeyStates[key_] && previousKeyStates[key_];
}

bool Input::IsKeyReleased(const int key_) noexcept
{
    return !currentKeyStates[key_] && previousKeyStates[key_];
}

bool Input::IsMouseButtonPressed(const int button_) noexcept
{
    return currentMouseButtonStates[button_] && !previousMouseButtonStates[button_];
}

bool Input::IsMouseButtonHeld(const int button_) noexcept
{
    return currentMouseButtonStates[button_] && previousMouseButtonStates[button_];
}

bool Input::IsMouseButtonReleased(const int button_) noexcept
{
    return !currentMouseButtonStates[button_] && previousMouseButtonStates[button_];
}

glm::vec2 Input::GetMousePosition() noexcept
{
    return currentMousePosition;
}

glm::vec2 Input::GetMouseDelta() noexcept
{
    return currentMousePosition - previousMousePosition;
}

void Input::OnKeyPressed(unsigned char key_, int x_, int y_) noexcept
{
    currentKeyStates[static_cast<int>(key_)] = true;
    currentMousePosition = {x_, y_};
}

void Input::OnKeyReleased(unsigned char key_, int x_, int y_) noexcept
{
    currentKeyStates[static_cast<int>(key_)] = false;
    currentMousePosition = {x_, y_};
}

void Input::OnSpecialKeyPressed(int key_, int x_, int y_) noexcept
{
    currentKeyStates[key_] = true;
    currentMousePosition = {x_, y_};
}

void Input::OnSpecialKeyReleased(int key_, int x_, int y_) noexcept
{
    currentKeyStates[key_] = false;
    currentMousePosition = {x_, y_};
}

void Input::OnMouseClicked(int button_, int state_, int x_, int y_) noexcept
{
    currentMouseButtonStates[button_] = (state_ == GLUT_DOWN);
    currentMousePosition = {x_, y_};
}

void Input::OnMouseMoved(int x_, int y_) noexcept
{
    currentMousePosition = {x_, y_};
}

std::unordered_map<int, bool> Input::currentKeyStates;

std::unordered_map<int, bool> Input::previousKeyStates;

std::array<bool, 8> Input::currentMouseButtonStates  = {};

std::array<bool, 8> Input::previousMouseButtonStates = {};

glm::vec2 Input::currentMousePosition  = {0.0f, 0.0f};

glm::vec2 Input::previousMousePosition = {0.0f, 0.0f};