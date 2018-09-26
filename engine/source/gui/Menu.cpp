#include "Menu.h"

#include "Button.h"
#include "GuiManager.h"
#include "VerticalLayout.h"
#include "../serialization/ObjectFactory.h"

namespace sh
{

namespace gui
{
	Menu::Menu()
	{
		m_enabled = false;
		m_visible = false;
		m_rect.Set(0, 0, 150, 300);
		VerticalLayoutPtr layout(new VerticalLayout());
		SetLayout(layout);
	}

	Menu::Menu(const String& title) : m_title(title)
	{
		m_enabled = false;
		m_visible = false;
		m_rect.Set(0, 0, 150, 300);
		VerticalLayoutPtr layout(new VerticalLayout());
		SetLayout(layout);
	}
    
    void Menu::RegisterObject()
    {
        ObjectFactory::GetInstance()->RegisterFactory<Menu>("UI");
        ObjectFactory::GetInstance()->RegisterParentProperties<Menu, Widget>();
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
		const s32 height = (m_layout->GetItemsCount() + 1) * 20;
		SetHeight(height);
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

		// Listen to submenu item selection
		item.menu->itemSelected.Connect(std::bind(&Menu::OnSubmenuItemSelected, this, std::placeholders::_1));
		// Update submenu position
		item.menu->SetPosition(item.button->GetPosition().x + item.button->GetWidth(), item.button->GetPosition().y);
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

	void Menu::RenderBackground(video::Painter* painter)
	{
		Widget::RenderBackground(painter);

		for (const auto& item : m_submenus)
		{
			if (item.menu->IsVisible())
				item.menu->RenderBackground(painter);
		}
	}

	void Menu::RenderText(video::Painter* painter)
	{
		Widget::RenderText(painter);

		for (const auto& item : m_submenus)
		{
			if (item.menu->IsVisible())
				item.menu->RenderText(painter);
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
				GuiManager::GetInstance()->SetFocusWidget(nullptr);
				//SetFocus(false);
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

	void Menu::AdjustSize()
	{

	}

	void Menu::OnButtonReleased(const ButtonPtr& sender)
	{
		u32 itemsCount = m_layout->GetItemsCount();
		for (u32 i = 0U; i < itemsCount; ++i)
		{
			const auto& button = std::static_pointer_cast<Button>(m_layout->GetWidget(i));
			if (button == sender)
			{
				if (IsInFocus())
					GuiManager::GetInstance()->SetFocusWidget(nullptr);
				itemSelected(sender->GetText());
				itemIndexSelected(i);
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

	void Menu::OnSubmenuItemSelected(const String& itemName)
	{
		if (IsInFocus())
			GuiManager::GetInstance()->SetFocusWidget(nullptr);
		itemSelected(itemName);
	}

} // gui

} // sh
