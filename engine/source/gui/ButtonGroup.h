#ifndef SHADOW_BUTTON_GROUP_INCLUDE
#define SHADOW_BUTTON_GROUP_INCLUDE

#include "Button.h"

namespace sh
{

namespace gui
{

	class SHADOW_API ButtonGroup
	{
	public:
		void AddButton(const ButtonPtr& button);

		Event<void, const ButtonPtr&> buttonToggled;

	private:
		void OnButtonToggled(bool togggled, const ButtonPtr& sender);

	private:
		std::vector<ButtonPtr> m_buttons;
	};

} // gui

} // sh

#endif
