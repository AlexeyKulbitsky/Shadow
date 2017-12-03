#ifndef SHADOW_INPUT_MANAGER_INCLUDE
#define SHADOW_INPUT_MANAGER_INCLUDE

#include "Globals.h"

namespace sh
{
	class SHADOW_API InputManager
	{
	public:
		static const size_t NUMBER_OF_KEYBOARD_KEYS = 256;
		static const size_t NUMBER_OF_MOUSE_KEYS = 5;

		InputManager();

		void OnMouseEvent(int x, int y, MouseEventType type, MouseCode code);
		void OnKeyboardEvent(KeyboardEventType type, KeyCode code);

		void SetKeyPressed(KeyCode code);
		void SetKeyReleased(KeyCode code);
		bool IsKeyPressed(KeyCode code);

		void SetMouseButtonPressed(MouseCode code);
		void SetMouseButtonReleased(MouseCode code);
		bool IsMouseButtonPressed(MouseCode code);

		void SetMousePositionOld(const math::Vector2Int& pos);
		void SetMousePositionCurrent(const math::Vector2Int& pos);

		const math::Vector2Int& GetMousePositionOld() const;
		const math::Vector2Int& GetMousePositionCurrent() const;

	private:
		bool m_keyboardKeys[NUMBER_OF_KEYBOARD_KEYS];
		bool m_mouseKeys[NUMBER_OF_MOUSE_KEYS];

		math::Vector2Int m_mousePositionOld;
		math::Vector2Int m_mousePositionCurrent;
		
	};
}

#endif