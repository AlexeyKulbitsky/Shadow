#include "ButtonGroup.h"

namespace sh
{

namespace gui
{

	void ButtonGroup::AddButton(const ButtonPtr& button)
	{
		button->OnToggle.Connect(std::bind(&ButtonGroup::OnButtonToggled, this, std::placeholders::_1, std::placeholders::_2));
		m_buttons.push_back(button);
	}

	void ButtonGroup::OnButtonToggled(bool togggled, const ButtonPtr& sender)
	{
		if (togggled)
		{
			for (const auto& button : m_buttons)
			{
				if (button != sender)
				{
					button->SetToggled(false);
				}
			}
			buttonToggled(sender);
		}
	}

} // gui

} // sh