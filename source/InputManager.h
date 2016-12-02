#ifndef SHADOW_INPUT_MANAGER_INCLUDE
#define SHADOW_INPUT_MANAGER_INCLUDE

#include "Globals.h"

namespace sh
{
	class InputManager
	{
	public:
		static const size_t NUMBER_OF_KEYBOARD_KEYS = 256;
		static const size_t NUMBER_OF_MOUSE_KEYS = 5;

		InputManager();

		void OnEvent(const Event& e);

		void SetKeyPressed(KeyCode code);
		void SetKeyReleased(KeyCode code);
		bool IsKeyPressed(KeyCode code);

		void SetMouseButtonPressed(MouseCode code);
		void SetMouseButtonReleased(MouseCode code);
		bool IsMouseButtonPressed(MouseCode code);

		void SetMousePositionOld(const math::Vector2i& pos);
		void SetMousePositionCurrent(const math::Vector2i& pos);

		const math::Vector2i& GetMousePositionOld() const;
		const math::Vector2i& GetMousePositionCurrent() const;

	private:
		bool m_keyboardKeys[NUMBER_OF_KEYBOARD_KEYS];
		bool m_mouseKeys[NUMBER_OF_MOUSE_KEYS];

		math::Vector2i m_mousePositionOld;
		math::Vector2i m_mousePositionCurrent;
		
	};
}

#endif