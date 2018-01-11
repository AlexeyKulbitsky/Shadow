#include "InputManager.h"
#include "Device.h"
#include "video/Driver.h"

#if defined (SHADOW_WINDOWS)
#include <Windows.h>
#endif


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
		default:
			break;
		}

		SetMousePositionOld(m_mousePositionCurrent);
		SetMousePositionCurrent(math::Vector2Int(x, y));
	}

	void InputManager::OnKeyboardEvent(KeyboardEventType type, KeyCode code)
	{
		if (type == KeyboardEventType::KeyPressed)
			SetKeyPressed(code);
		else
			SetKeyReleased(code);
	}

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

	void InputManager::SetMousePositionOld(const math::Vector2Int& pos)
	{
		m_mousePositionOld = pos;
	}

	void InputManager::SetMousePositionCurrent(const math::Vector2Int& pos)
	{
		m_mousePositionCurrent = pos;
	}

	const math::Vector2Int& InputManager::GetMousePositionOld() const
	{
		return m_mousePositionOld;
	}

	const math::Vector2Int& InputManager::GetMousePositionCurrent() const
	{
		return m_mousePositionCurrent;
	}

	void InputManager::SetCursorVisible(bool visible)
	{
		m_isCursorVivible = visible;
#if defined (SHADOW_WINDOWS)
		ShowCursor(m_isCursorVivible);
		//HWND hwnd = (HWND)sh::Device::GetInstance()->GetWinId();
		//PostMessage(hwnd, WM_SETCURSOR, WPARAM(hwnd), (LPARAM)MAKELONG(HTCLIENT, WM_MOUSEMOVE));
#endif
	}

	bool InputManager::IsCursorVisible() const
	{
		return m_isCursorVivible;
	}

}