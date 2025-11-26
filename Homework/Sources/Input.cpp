#include "Input.h"

void Input::Update() noexcept
{
	for (KeyState& keyState : keyStates)
	{
		switch (keyState)
		{
			case KeyState::Press:
			{
				keyState = KeyState::Hold;
				break;
			}
			case KeyState::Release:
			{
				keyState = KeyState::Idle;
				break;
			}
			default:
			{
				break;
			}
		}
	}

	for (KeyState& mouseButtonState : mouseButtonStates)
	{
		switch (mouseButtonState)
		{
			case KeyState::Press:
			{
				mouseButtonState = KeyState::Hold;
				break;
			}
			case KeyState::Release:
			{
				mouseButtonState = KeyState::Idle;
				break;
			}
			default:
			{
				break;
			}
		}
	}
}

void Input::OnKeyInteract(int key, int scancode, int action, int mods) noexcept
{
	if (key < 0 || key >= static_cast<int>(MAX_KEYS))
	{
		return;
	}
	switch (action)
	{
		case GLFW_PRESS:
		{
			keyStates[key] = KeyState::Press;
			break;
		}
		case GLFW_RELEASE:
		{
			keyStates[key] = KeyState::Release;
			break;
		}
		default:
		{
			break;
		}
	}

	currentMods = mods;
}

void Input::OnMouseButtonInteract(int button, int action, int mods) noexcept
{
	if (button < 0 || button >= MAX_MOUSE_BUTTONS)
	{
		return;
	}
	switch (action)
	{
		case GLFW_PRESS:
		{
			mouseButtonStates[button] = KeyState::Press;
			break;
		}
		case GLFW_RELEASE:
		{
			mouseButtonStates[button] = KeyState::Release;
			break;
		}
		default:
		{
			break;
		}
	}
	currentMods = mods;
}

void Input::OnCursorMove(double xpos, double ypos) noexcept
{
	lastMousePosition = currentMousePosition;
	currentMousePosition = glm::vec2(static_cast<float>(xpos), static_cast<float>(ypos));
}

std::array<Input::KeyState, Input::MAX_KEYS> Input::keyStates = { Input::KeyState::Idle };

std::array<Input::KeyState, Input::MAX_MOUSE_BUTTONS> Input::mouseButtonStates = { Input::KeyState::Idle };

glm::vec2 Input::lastMousePosition = glm::vec2(0.0f, 0.0f);

glm::vec2 Input::currentMousePosition = glm::vec2(0.0f, 0.0f);

int Input::currentMods = 0;