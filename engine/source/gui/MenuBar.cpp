#include "MenuBar.h"

#include "Menu.h"
#include "Button.h"
#include "Sprite.h"
#include "Style.h"
#include "HorizontalLayout.h"
#include "VerticalLayout.h"
#include "GuiManager.h"

#include "../video/Driver.h"

#include "../font/Font.h"
#include "../Device.h"

namespace sh
{

namespace gui
{

	MenuBar::MenuBarItem::MenuBarItem(const String& menuName, const ButtonPtr& _button)
	{
		menu.reset(new Menu(menuName));
		button = _button;
		button->SetText(menuName);
		button->SetToggleable(true);
		button->OnToggle.Connect(std::bind(&MenuBar::MenuBarItem::OnButtonToggled, this, std::placeholders::_1, std::placeholders::_2));
		m_layout.reset(new VerticalLayout());
		m_layout->AddWidget(button);
		SetMaximumWidth(50);
	}

	void MenuBar::MenuBarItem::UpdateLayout()
	{
		Widget::UpdateLayout();
		const auto& pos = button->GetPosition();
		const auto height = button->GetHeight();
		menu->SetPosition(pos.x, pos.y + height);
	}

	void MenuBar::MenuBarItem::Render(video::Painter* painter)
	{
		Widget::Render(painter);
		menu->Render(painter);
	}

	bool MenuBar::MenuBarItem::ProcessEvent(GUIEvent& ev)
	{
		if (menu->IsEnabled() &&
			(ev.type == EventType::PointerUp ||
				ev.type == EventType::PointerMove) &&
			menu->ProcessEvent(ev))
		{
			menu->SetEnabled(false);
			menu->SetVisible(false);
			return true;
		}

		if (button->ProcessEvent(ev))
			return true;

		return false;
	}

	void MenuBar::MenuBarItem::OnButtonToggled(bool toggled, const ButtonPtr& sender)
	{
		if (toggled)
		{
			menu->SetEnabled(true);
			menu->SetVisible(true);
			this->SetFocus(true);
			GuiManager::GetInstance()->SetFocusWidget(shared_from_this());
		}
		else
		{
			menu->SetEnabled(false);
			menu->SetVisible(false);
			this->SetFocus(false);
			GuiManager::GetInstance()->SetFocusWidget(nullptr);
		}
	}

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
		MenuBarItemPtr item(new MenuBarItem(title, button));
		m_layout->AddWidget(item);
		m_menuItems.push_back(item);
		return m_menuItems[m_menuItems.size() - 1]->menu;
	}

	/////////////////////////////////////////////////////////////////////////////////////

	void MenuBar::Render(video::Painter* painter)
	{
		if (!m_visible)
			return;
		painter->SetMaterial(GuiManager::GetInstance()->GetDefaultMaterial());
		video::Painter::Vertex upperLeft(m_rect.upperLeftCorner, 
										 m_sprite->GetUVRect().upperLeftCorner, 
										 m_sprite->GetColor());
		video::Painter::Vertex downRight(m_rect.lowerRightCorner,
										 m_sprite->GetUVRect().lowerRightCorner,
										 m_sprite->GetColor());
		painter->DrawRect(upperLeft, downRight);

		for (const auto& menuItem : m_menuItems)
		{
			menuItem->Render(painter);
		}
	}

	/////////////////////////////////////////////////////////////////////////////////////

} // gui

} // sh