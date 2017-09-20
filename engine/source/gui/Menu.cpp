#include "Menu.h"

#include "Button.h"
#include "VerticalLayout.h"

namespace sh
{

namespace gui
{
	Menu::Menu()
	{
		m_enabled = false;
		m_visible = false;
		m_rect.Set(0, 0, 100, 300);
		VerticalLayoutPtr layout(new VerticalLayout());
		SetLayout(layout);
	}

	Menu::Menu(const String& title) : m_title(title)
	{
		m_enabled = false;
		m_visible = false;
		m_rect.Set(0, 0, 100, 300);
		VerticalLayoutPtr layout(new VerticalLayout());
		SetLayout(layout);
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
		//u32 size = m_buttons.size();
		//button->SetPosition(0, button->GetHeight() * (size + 1));
		//button->SetWidth(100U);
		//m_buttons.push_back(button); 
		button->SetMaximumHeight(20);
		m_layout->AddWidget(button);
	}

	void Menu::AddItem(const String& name)
	{
		ButtonPtr button(new Button(name));
		button->SetHeight(20);
		AddItem(button);
	}

	void Menu::Render(video::Painter* painter)
	{
		//for (const auto& button : m_buttons)
		//{
		//	button->Render(painter);
		//}
		Widget::Render(painter);
	}

	void Menu::SetPosition(s32 x, s32 y)
	{
		Widget::SetPosition(x, y);
	}

	void Menu::SetWidth(s32 width)
	{
		Widget::SetWidth(width);
	}

	void Menu::SetHeight(s32 height)
	{
		Widget::SetHeight(height);
	}

	bool Menu::ProcessEvent(GUIEvent& ev)
	{
		if (!m_isInFocus)
			return false;

		if (ev.type == EventType::PointerDown && 
			!m_rect.IsPointInside(ev.x, ev.y))
		{
			SetFocus(false);
			return true;
		}
		return Widget::ProcessEvent(ev);
	}

} // gui

} // sh