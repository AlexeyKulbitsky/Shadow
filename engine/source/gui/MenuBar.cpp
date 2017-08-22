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

		UpdatePosition();
		UpdateUV(m_sprite->GetUVRect().upperLeftCorner, m_sprite->GetUVRect().lowerRightCorner);
		UpdateColor(m_sprite->GetColor());

		Device::GetInstance()->windowResizeEvent.Connect(std::bind(&MenuBar::OnWindowResized, this,
			std::placeholders::_1, std::placeholders::_2));
	}

	/////////////////////////////////////////////////////////////////////////////////////

	MenuBar::MenuBar(const SpritePtr& sprite)
		: Widget()
	{
		m_sprite = sprite;
		SetHeight(15U);

		UpdatePosition();
		UpdateUV(m_sprite->GetUVRect().upperLeftCorner, m_sprite->GetUVRect().lowerRightCorner);
		UpdateColor(m_sprite->GetColor());

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

		m_menus.push_back(std::make_pair(button, menu));
		return m_menus[m_menus.size() - 1].second;
	}

	/////////////////////////////////////////////////////////////////////////////////////

	void MenuBar::GetGeometry(GuiBatchData& data)
	{
		Widget::GetGeometry(data);
		
		for (const auto& menu : m_menus)
		{
			menu.first->GetGeometry(data);
			if (menu.second->IsVisible())
				menu.second->GetGeometry(data);
		}
	}

	/////////////////////////////////////////////////////////////////////////////////////

	void MenuBar::GetTextGeometry(GuiBatchData& data)
	{
		for (const auto& menu : m_menus)
		{
			menu.first->GetTextGeometry(data);
			if (menu.second->IsVisible())
				menu.second->GetTextGeometry(data);
		}
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

	void MenuBar::SetPosition(u32 x, u32 y)
	{
		// Position is fixed in left upper corener of viewport
	}

	/////////////////////////////////////////////////////////////////////////////////////

	void MenuBar::SetWidth(u32 width)
	{
		// Width is always stretched to viewport's width
	}

	/////////////////////////////////////////////////////////////////////////////////////

	void MenuBar::SetHeight(u32 height)
	{
		video::Driver* driver = Device::GetInstance()->GetDriver();
		const auto& viewPort = driver->GetViewPort();
		math::Vector2u extends(viewPort.z, viewPort.w);

		m_rect.Set(0U, 0U, extends.x, height);

		UpdatePosition();
	}

	/////////////////////////////////////////////////////////////////////////////////////

	bool MenuBar::ProcessInput(u32 x, u32 y, MouseEventType type)
	{
		for (const auto& menu : m_menus)
		{
			if (menu.second->IsEnabled() &&
				(type == MouseEventType::ButtonReleased ||
				type == MouseEventType::Moved) &&
				menu.second->ProcessInput(x, y, type))
			{
				menu.second->SetEnabled(false);
				menu.second->SetVisible(false);
				return true;
			}

			if (menu.first->ProcessInput(x, y, type))
				return true;
		}
		return false;
	}

	/////////////////////////////////////////////////////////////////////////////////////

	void MenuBar::OnWindowResized(int width, int)
	{
		m_rect.Set(0U, 0U, width, m_rect.lowerRightCorner.y);
		UpdatePosition();
	}

	/////////////////////////////////////////////////////////////////////////////////////

} // gui

} // sh