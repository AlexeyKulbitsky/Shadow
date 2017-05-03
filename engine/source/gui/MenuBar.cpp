#include "MenuBar.h"

#include "Menu.h"
#include "Button.h"
#include "Sprite.h"
#include "GuiManager.h"

#include "../video/Driver.h"

#include "../font/Font.h"
#include "../Device.h"

namespace sh
{

namespace gui
{
	MenuBar::MenuBar()
	{
		m_batchData.resize(4 * 8);

		video::Driver* driver = Device::GetInstance()->GetDriver();

		const auto& viewPort = driver->GetViewPort();
		math::Vector2u extends(viewPort.z, viewPort.w);

		m_rect.Set(0U, 0U, extends.x, 15U);

		math::Vector4f leftUp((float)m_rect.upperLeftCorner.x, (float)m_rect.upperLeftCorner.y, 0.0f, 1.0f);
		math::Vector4f rightDown((float)m_rect.lowerRightCorner.x, (float)m_rect.lowerRightCorner.y, 0.0f, 1.0f);

		math::Rectf uvRect(60.0f / 256.0f, 32.0f / 128.0f, 74.0f / 256.0f, 46.0f / 128.0f);
		math::Vector2f uvLeftUp = uvRect.upperLeftCorner;
		math::Vector2f uvRightDown = uvRect.lowerRightCorner;

		
		std::vector<float> vertices = 
		{
			leftUp.x, leftUp.y, 0.0f,			uvLeftUp.x, uvLeftUp.y,			0.7f, 0.7f, 0.7f,
			leftUp.x, rightDown.y, 0.0f,		uvLeftUp.x, uvRightDown.y,		0.7f, 0.7f, 0.7f,
			rightDown.x, rightDown.y, 0.0f,		uvRightDown.x, uvRightDown.y,	0.7f, 0.7f, 0.7f,
			rightDown.x, leftUp.y, 0.0f,		uvRightDown.x, uvLeftUp.y,		0.7f, 0.7f, 0.7f
		};
		

		m_batchData = std::move(vertices);
	}

	MenuBar::MenuBar(const SpritePtr& sprite)
	{
		m_batchData.resize(4 * 8);

		m_sprite = sprite;
		SetHeight(15U);

		UpdatePosition();
		UpdateUV(m_sprite->GetUVRect().upperLeftCorner, m_sprite->GetUVRect().lowerRightCorner);
		UpdateColor(m_sprite->GetColor());
	}

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

	void MenuBar::GetGeometry(GuiBatchData& data)
	{
		data.vertices.insert(data.vertices.end(), m_batchData.begin(), m_batchData.end());

		data.indices.push_back(data.verticesCount);
		data.indices.push_back(data.verticesCount + 1);
		data.indices.push_back(data.verticesCount + 2);

		data.indices.push_back(data.verticesCount);
		data.indices.push_back(data.verticesCount + 2);
		data.indices.push_back(data.verticesCount + 3);
		data.verticesCount += 4;
		
		for (const auto& menu : m_menus)
		{
			menu.first->GetGeometry(data);
			if (menu.second->IsVisible())
				menu.second->GetGeometry(data);
		}
	}

	void MenuBar::GetTextGeometry(GuiBatchData& data)
	{
		for (const auto& menu : m_menus)
		{
			menu.first->GetTextGeometry(data);
			if (menu.second->IsVisible())
				menu.second->GetTextGeometry(data);
		}
	}

	void MenuBar::SetPosition(u32 x, u32 y)
	{

	}

	void MenuBar::SetWidth(u32 width)
	{

	}

	void MenuBar::SetHeight(u32 height)
	{
		video::Driver* driver = Device::GetInstance()->GetDriver();
		const auto& viewPort = driver->GetViewPort();
		math::Vector2u extends(viewPort.z, viewPort.w);

		m_rect.Set(0U, 0U, extends.x, height);

		UpdatePosition();
	}

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

} // gui

} // sh