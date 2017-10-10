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
		menu->itemSelected.Connect(std::bind(&MenuBar::MenuBarItem::OnMenuItemSelected, this, std::placeholders::_1));
		button = _button;
		button->SetText(menuName);
		button->SetToggleable(true);
		button->OnToggle.Connect(std::bind(&MenuBar::MenuBarItem::OnButtonToggled, this, std::placeholders::_1, std::placeholders::_2));
		button->OnHover.Connect(std::bind(&MenuBar::MenuBarItem::OnButtonHovered, this, std::placeholders::_1));
		m_layout.reset(new VerticalLayout());
		m_layout->AddWidget(button);
		SetMaximumWidth(50);
	}

	void MenuBar::MenuBarItem::UpdateLayout()
	{
		Widget::UpdateLayout();
		const auto& pos = m_rect.upperLeftCorner;
		const auto height = m_rect.GetHeight();
		menu->SetPosition(pos.x, pos.y + height);
	}

	void MenuBar::MenuBarItem::Render(video::Painter* painter)
	{
		// Render activation button
		Widget::Render(painter);

		// Render menu if it is visible
		if (menu->IsVisible())
			menu->Render(painter);
	}

	bool MenuBar::MenuBarItem::ProcessEvent(GUIEvent& ev)
	{
		if (button->ProcessEvent(ev))
			return true;


		if (menu->IsEnabled() && menu->ProcessEvent(ev))
		{
			//menu->SetEnabled(false);
			//menu->SetVisible(false);
			return true;
		}

		

		return false;
	}

	void MenuBar::MenuBarItem::OnButtonToggled(bool toggled, const ButtonPtr& sender)
	{
		if (toggled)
		{
			menu->SetEnabled(true);
			menu->SetVisible(true);
			parent->m_activeItem = this;
		}
		else
		{
			menu->SetEnabled(false);
			menu->SetVisible(false);
			parent->m_activeItem = nullptr;
		}
		parent->SetToggled(toggled);
	}

	void MenuBar::MenuBarItem::OnButtonHovered(const ButtonPtr& sender)
	{
		if (parent->IsToggled() && parent->m_activeItem != this)
		{
			if (parent->m_activeItem)
				parent->m_activeItem->button->SetToggled(false);
			button->SetToggled(true);
		}
	}

	void MenuBar::MenuBarItem::OnMenuItemSelected(const String& itemName)
	{
		button->SetToggled(false);
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

	void MenuBar::SetToggled(bool toggled) 
	{ 
		m_toggled = toggled;
		if (toggled)
		{
			//SetFocus(true);
			GuiManager::GetInstance()->SetFocusWidget(shared_from_this());
		}
		else
		{
			//SetFocus(false);
			if (IsInFocus())
				GuiManager::GetInstance()->SetFocusWidget(nullptr);
		}
	}

	/////////////////////////////////////////////////////////////////////////////////////

	const MenuPtr& MenuBar::AddMenu(const String& title, const ButtonPtr& button)
	{
		MenuBarItemPtr item(new MenuBarItem(title, button));
		item->parent = this;
		item->SetVisible(false);
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

	bool MenuBar::ProcessEvent(GUIEvent& ev)
	{
		if (Widget::ProcessEvent(ev))
		{
			return true;
		}

		if (m_toggled && ev.type == EventType::PointerDown)
		{
			m_activeItem->button->SetToggled(false);
			return true;
		}

		bool isInside = m_rect.IsPointInside(ev.x, ev.y);
		return isInside;
	}

} // gui

} // sh