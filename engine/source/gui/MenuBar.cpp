#include "MenuBar.h"

#include "Menu.h"
#include "Button.h"
#include "Sprite.h"
#include "Style.h"
#include "HorizontalLayout.h"
#include "GuiManager.h"

#include "../video/Driver.h"

#include "../font/Font.h"
#include "../Device.h"

namespace sh
{

namespace gui
{

	MenuBar::MenuBar()
		: Widget()
	{
		m_sprite = GuiManager::GetInstance()->GetStyle()->GetMenuBar()->m_sprite;
		SetMaximumHeight(20);

		HorizontalLayoutPtr layout(new HorizontalLayout());
		SetLayout(layout);
	}

	/////////////////////////////////////////////////////////////////////////////////////

	MenuBar::MenuBar(const SpritePtr& sprite)
		: Widget()
	{
		m_sprite = sprite;
		SetMaximumHeight(20);

		HorizontalLayoutPtr layout(new HorizontalLayout());
		SetLayout(layout);
	}

	/////////////////////////////////////////////////////////////////////////////////////

	const MenuPtr& MenuBar::AddMenu(const String& title, const ButtonPtr& button)
	{
		MenuPtr menu(new Menu(title));
		button->SetText(title);
		button->SetMaximumWidth(50);
		button->OnToggle.Connect(std::bind(&Menu::SetEnabled, menu.get(), std::placeholders::_1));
		button->OnToggle.Connect(std::bind(&Menu::SetVisible, menu.get(), std::placeholders::_1));
		button->OnToggle.Connect(std::bind(&MenuBar::OnButtonToggled, this, std::placeholders::_1, std::placeholders::_2));
		m_layout->AddWidget(button);

		m_menus.push_back(std::make_pair(button, menu));
		return m_menus[m_menus.size() - 1].second;
	}

	/////////////////////////////////////////////////////////////////////////////////////

	void MenuBar::Render(video::Painter* painter)
	{
		painter->SetMaterial(GuiManager::GetInstance()->GetDefaultMaterial());
		video::Painter::Vertex upperLeft(m_rect.upperLeftCorner, 
										 m_sprite->GetUVRect().upperLeftCorner, 
										 m_sprite->GetColor());
		video::Painter::Vertex downRight(m_rect.lowerRightCorner,
										 m_sprite->GetUVRect().lowerRightCorner,
										 m_sprite->GetColor());
		painter->DrawRect(upperLeft, downRight);

		for (const auto& menu : m_menus)
		{
			menu.first->Render(painter);
			if (menu.second->IsVisible())
				menu.second->Render(painter);
		}
	}

	/////////////////////////////////////////////////////////////////////////////////////

	void MenuBar::SetPosition(s32 x, s32 y)
	{
		Widget::SetPosition(x, y);
	}

	/////////////////////////////////////////////////////////////////////////////////////

	void MenuBar::SetWidth(s32 width)
	{
		Widget::SetWidth(width);
	}

	/////////////////////////////////////////////////////////////////////////////////////

	void MenuBar::SetHeight(s32 height)
	{
		Widget::SetHeight(height);
	}

	/////////////////////////////////////////////////////////////////////////////////////

	bool MenuBar::ProcessEvent(GUIEvent& ev)
	{
		for (const auto& menu : m_menus)
		{
			if (menu.second->IsEnabled() &&
				(ev.type == EventType::PointerUp ||
					ev.type == EventType::PointerMove) &&
				menu.second->ProcessEvent(ev))
			{
				menu.second->SetEnabled(false);
				menu.second->SetVisible(false);
				return true;
			}

			if (menu.first->ProcessEvent(ev))
				return true;
		}
		return false;
	}

	/////////////////////////////////////////////////////////////////////////////////////

	void MenuBar::UpdateLayout()
	{
		Widget::UpdateLayout();

		for (const auto& menu : m_menus)
		{
			const auto& pos = menu.first->GetPosition();
			const auto height = menu.first->GetHeight();
			menu.second->SetPosition(pos.x, pos.y + height);
		}
	}

	/////////////////////////////////////////////////////////////////////////////////////

	void MenuBar::OnButtonToggled(bool toggled, const ButtonPtr& sender)
	{
		for (const auto& menu : m_menus)
		{
			if (menu.first == sender)
			{
				menu.second->SetFocus(true);
				GuiManager::GetInstance()->SetFocusWidget(menu.second);
			}
		}
	}

	/////////////////////////////////////////////////////////////////////////////////////
} // gui

} // sh