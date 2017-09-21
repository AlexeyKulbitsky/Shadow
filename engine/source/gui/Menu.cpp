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
	
	Menu::~Menu()
	{
	}

	void Menu::Show()
	{
	}

	void Menu::Hide()
	{
	}

	void Menu::AddItem(const ButtonPtr& button) 
	{ 
		button->SetMaximumHeight(20);
		m_layout->AddWidget(button);
	}

	void Menu::AddItem(const String& name)
	{
		ButtonPtr button(new Button(name));
		button->SetHeight(20);
		button->OnRelease.Connect(std::bind(&Menu::OnButtonReleased, this, std::placeholders::_1));
		AddItem(button);
	}

	void Menu::Render(video::Painter* painter)
	{
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
		//if (!m_isInFocus)
		//	return false;

		if (ev.type == EventType::PointerDown && 
			!m_rect.IsPointInside(ev.x, ev.y))
		{
			SetFocus(false);
			return true;
		}
		return Widget::ProcessEvent(ev);
	}

	void Menu::OnButtonReleased(const ButtonPtr& sender)
	{
		u32 itemsCount = m_layout->GetItemsCount();
		for (u32 i = 0U; i < itemsCount; ++i)
		{
			const auto& button = std::static_pointer_cast<Button>(m_layout->GetWidget(i));
			if (button == sender)
			{
				itemSelected(sender->GetText());
				itemIndexSelected(i);
				SetFocus(false);
				return;
			}
		}
	}

} // gui

} // sh