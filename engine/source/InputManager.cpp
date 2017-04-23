#include "InputManager.h"
#include "Device.h"




namespace sh
{
	InputManager::InputManager()
	{
		for (size_t i = 0; i < NUMBER_OF_KEYBOARD_KEYS; ++i)
		{
			m_keyboardKeys[i] = false;
		}

		for (size_t i = 0; i < NUMBER_OF_MOUSE_KEYS; ++i)
		{
			m_mouseKeys[i] = false;
		}

		
	}

	void InputManager::OnMouseEvent(int x, int y, MouseEventType type, MouseCode code)
	{
		switch (type)
		{
		case MouseEventType::ButtonPressed:
			SetMouseButtonPressed(code);
			break;
		case MouseEventType::ButtonReleased:
			SetMouseButtonReleased(code);
			break;
		case MouseEventType::Moved:
			SetMousePositionCurrent(math::Vector2i(x, y));
			break;
		default:
			break;
		}
		SetMousePositionCurrent(math::Vector2i(x, y));
	}

	void InputManager::OnKeyboardEvent(KeyboardEventType type, KeyCode code)
	{
		if (type == KeyboardEventType::KeyPressed)
			SetKeyPressed(code);
		else
			SetKeyReleased(code);
	}

	/*
	void InputManager::OnEvent(const Event& e)
	{
		switch (e.type)
		{
		
		case EventType::MOUSE_INPUT_EVENT:
		{	
			switch (e.mouseEvent.type)
			{
			case MouseEventType::BUTTON_PRESSED:
				SetMouseButtonPressed(e.mouseEvent.mouseCode);
				break;
			case MouseEventType::BUTTON_RELEASED:
				SetMouseButtonReleased(e.mouseEvent.mouseCode);
				break;
			case MouseEventType::MOVED:
				SetMousePositionCurrent(math::Vector2i(e.mouseEvent.x, e.mouseEvent.y));
				break;
			default:
				break;
			}
			SetMousePositionCurrent(math::Vector2i(e.mouseEvent.x, e.mouseEvent.y));
		}
			break;
		default:
			break;
		}
	}
	*/

	void InputManager::SetKeyPressed(KeyCode code)
	{
		m_keyboardKeys[size_t(code)] = true;
	}

	void InputManager::SetKeyReleased(KeyCode code)
	{
		m_keyboardKeys[size_t(code)] = false;
	}

	bool InputManager::IsKeyPressed(KeyCode code)
	{
		return m_keyboardKeys[size_t(code)];
	}

	void InputManager::SetMouseButtonPressed(MouseCode code)
	{
		m_mouseKeys[size_t(code)] = true;
	}

	void InputManager::SetMouseButtonReleased(MouseCode code)
	{
		m_mouseKeys[size_t(code)] = false;
	}

	bool InputManager::IsMouseButtonPressed(MouseCode code)
	{
		return m_mouseKeys[size_t(code)];
	}

	void InputManager::SetMousePositionOld(const math::Vector2i& pos)
	{
		m_mousePositionOld = pos;
	}

	void InputManager::SetMousePositionCurrent(const math::Vector2i& pos)
	{
		m_mousePositionCurrent = pos;
	}

	const math::Vector2i& InputManager::GetMousePositionOld() const
	{
		return m_mousePositionOld;
	}

	const math::Vector2i& InputManager::GetMousePositionCurrent() const
	{
		return m_mousePositionCurrent;
	}
}