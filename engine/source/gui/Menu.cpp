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
		button->SetHeight(20);
		button->OnRelease.Connect(std::bind(&Menu::OnButtonReleased, this, std::placeholders::_1));
		button->OnHover.Connect(std::bind(&Menu::OnButtonHovered, this, std::placeholders::_1));
		m_layout->AddWidget(button);
	}

	void Menu::AddItem(const String& name)
	{
		ButtonPtr button(new Button(name));
		AddItem(button);
	}

	void Menu::AddSubmenu(const String& itemName, const MenuPtr& subMenu)
	{
		auto itemsCount = m_layout->GetItemsCount();
		for(size_t i = 0; i < itemsCount; ++i)
		{
			auto button = std::static_pointer_cast<Button>(m_layout->GetWidget(i));
			if (button->GetText() == itemName)
			{
				AddSubmenu(i, subMenu);
				return;
			}
		}
	}

	void Menu::AddSubmenu(size_t itemIndex, const MenuPtr& subMenu)
	{
		auto button = std::static_pointer_cast<Button>(m_layout->GetWidget(itemIndex));
		SubmenuItem item;
		item.button = button.get();
		item.menu = subMenu;
		m_submenus.push_back(item);
	}

	void Menu::Render(video::Painter* painter)
	{
		Widget::Render(painter);

		for (const auto& item : m_submenus)
		{
			if (item.menu->IsVisible())
				item.menu->Render(painter);
		}
	}

	bool Menu::ProcessEvent(GUIEvent& ev)
	{
		if (Widget::ProcessEvent(ev))
			return true;

		for (const auto& item : m_submenus)
		{
			if (item.menu->IsEnabled() && item.menu->ProcessEvent(ev))
				return true;
		}

		if (ev.type == EventType::PointerDown &&
			!m_rect.IsPointInside(ev.x, ev.y))
		{
			if (IsInFocus())
				SetFocus(false);
			return false;
		}

		return false;
	}

	void Menu::UpdateLayout()
	{
		Widget::UpdateLayout();
		for (const auto& item : m_submenus)
		{
			item.menu->SetPosition(item.button->GetPosition().x + item.button->GetWidth(), item.button->GetPosition().y);
		}
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
				if (IsInFocus())
					SetFocus(false);
				return;
			}
		}
	}

	void Menu::OnButtonHovered(const ButtonPtr& sender)
	{
		for (const auto& item : m_submenus)
		{
			item.menu->SetVisible(false);
			item.menu->SetEnabled(false);

			if (item.button == sender.get())
			{
				item.menu->SetVisible(true);
				item.menu->SetEnabled(true);
			}
		}
	}

} // gui

} // sh