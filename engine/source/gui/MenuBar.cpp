#include "MenuBar.h"

#include "Menu.h"
#include "Button.h"
#include "Sprite.h"
#include "Style.h"
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
		SetHeight(15U);

		Device::GetInstance()->windowResizeEvent.Connect(std::bind(&MenuBar::OnWindowResized, this,
			std::placeholders::_1, std::placeholders::_2));
	}

	/////////////////////////////////////////////////////////////////////////////////////

	MenuBar::MenuBar(const SpritePtr& sprite)
		: Widget()
	{
		m_sprite = sprite;
		SetHeight(15U);

		Device::GetInstance()->windowResizeEvent.Connect(std::bind(&MenuBar::OnWindowResized, this,
			std::placeholders::_1, std::placeholders::_2));

	}

	/////////////////////////////////////////////////////////////////////////////////////

	const MenuPtr& MenuBar::AddMenu(const String& title, const ButtonPtr& button)
	{
		MenuPtr menu(new Menu(title));
		button->SetPosition(0, 0);
		button->SetText(title);
		button->OnToggle.Connect(std::bind(&Menu::SetEnabled, menu.get(), std::placeholders::_1));
		button->OnToggle.Connect(std::bind(&Menu::SetVisible, menu.get(), std::placeholders::_1));
		menu->SetPosition(0, 15);

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
		// Position is fixed in left upper corener of viewport
	}

	/////////////////////////////////////////////////////////////////////////////////////

	void MenuBar::SetWidth(s32 width)
	{
		// Width is always stretched to viewport's width
	}

	/////////////////////////////////////////////////////////////////////////////////////

	void MenuBar::SetHeight(s32 height)
	{
		video::Driver* driver = Device::GetInstance()->GetDriver();
		const auto& viewPort = driver->GetViewPort();
		math::Vector2u extends(viewPort.z, viewPort.w);

		m_rect.Set(0U, 0U, extends.x, height);
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

	void MenuBar::OnWindowResized(int width, int)
	{
		m_rect.Set(0U, 0U, width, m_rect.lowerRightCorner.y);
//		UpdatePosition();
	}

	/////////////////////////////////////////////////////////////////////////////////////

} // gui

} // sh