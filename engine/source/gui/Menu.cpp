#include "Menu.h"

#include "Button.h"

namespace sh
{

namespace gui
{
	Menu::Menu()
	{
		m_enabled = false;
		m_visible = false;
	}

	Menu::Menu(const String& title) : m_title(title)
	{
		m_enabled = false;
		m_visible = false;
	}

	void Menu::Show()
	{
		int a = 0; 
		a++;
	}

	void Menu::Hide()
	{
		int a = 0; 
		a++;
	}

	void Menu::AddItem(const ButtonPtr& button) 
	{ 
		u32 size = m_buttons.size();
		button->SetPosition(0, button->GetHeight() * (size + 1));
		button->SetWidth(100U);
		m_buttons.push_back(button); 
	}

	void Menu::Render(video::Painter* painter)
	{
		for (const auto& button : m_buttons)
		{
			button->Render(painter);
		}
	}

	void Menu::SetPosition(u32 x, u32 y)
	{

	}

	void Menu::SetWidth(u32 width)
	{

	}

	void Menu::SetHeight(u32 height)
	{

	}

	bool Menu::ProcessInput(u32 x, u32 y, MouseEventType type)
	{
		for (const auto& button : m_buttons)
		{
			if (button->ProcessInput(x, y, type))
				return true;
		}
		return false;
	}

} // gui

} // sh