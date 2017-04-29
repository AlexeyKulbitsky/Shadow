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
		leftUp.x = (leftUp.x / extends.x) * 2.0f - 1.0f;
		leftUp.y = ((extends.y - leftUp.y) / extends.y) * 2.0f - 1.0f;

		math::Vector4f rightDown((float)m_rect.lowerRightCorner.x, (float)m_rect.lowerRightCorner.y, 0.0f, 1.0f);
		rightDown.x = (rightDown.x / extends.x) * 2.0f - 1.0f;
		rightDown.y = ((extends.y - rightDown.y) / extends.y) * 2.0f - 1.0f;

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
		UpdateUV();
		UpdateColor();
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

	void MenuBar::GetText(GuiBatchData& data)
	{
		for (const auto& menu : m_menus)
		{
			menu.first->GetText(data);
			if (menu.second->IsVisible())
				menu.second->GetText(data);
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
			if (menu.second->IsEnabled() && menu.second->ProcessInput(x, y, type))
				return true;

			if (menu.first->ProcessInput(x, y, type))
				return true;
		}
		return false;
	}

	void MenuBar::UpdatePosition()
	{
		video::Driver* driver = Device::GetInstance()->GetDriver();
		const auto& viewPort = driver->GetViewPort();
		math::Vector2u extends(viewPort.z, viewPort.w);

		math::Vector4f leftUp((float)m_rect.upperLeftCorner.x, (float)m_rect.upperLeftCorner.y, 0.0f, 1.0f);
		leftUp.x = (leftUp.x / extends.x) * 2.0f - 1.0f;
		leftUp.y = ((extends.y - leftUp.y) / extends.y) * 2.0f - 1.0f;

		math::Vector4f rightDown((float)m_rect.lowerRightCorner.x, (float)m_rect.lowerRightCorner.y, 0.0f, 1.0f);
		rightDown.x = (rightDown.x / extends.x) * 2.0f - 1.0f;
		rightDown.y = ((extends.y - rightDown.y) / extends.y) * 2.0f - 1.0f;

		m_batchData[0] = leftUp.x; m_batchData[1] = leftUp.y; m_batchData[2] = 0.0f;
		m_batchData[8] = leftUp.x; m_batchData[9] = rightDown.y; m_batchData[10] = 0.0f;
		m_batchData[16] = rightDown.x; m_batchData[17] = rightDown.y; m_batchData[18] = 0.0f;
		m_batchData[24] = rightDown.x; m_batchData[25] = leftUp.y; m_batchData[26] = 0.0f;
	}

	void MenuBar::UpdateUV()
	{
		const auto& uvLeftUp = m_sprite->GetUVRect().upperLeftCorner;
		const auto& uvRightDown = m_sprite->GetUVRect().lowerRightCorner;

		m_batchData[3] = uvLeftUp.x; m_batchData[4] = uvLeftUp.y; 
		m_batchData[11] = uvLeftUp.x; m_batchData[12] = uvRightDown.y; 
		m_batchData[19] = uvRightDown.x; m_batchData[20] = uvRightDown.y; 
		m_batchData[27] = uvRightDown.x; m_batchData[28] = uvLeftUp.y; 
	}

	void MenuBar::UpdateColor()
	{
		const auto& color = m_sprite->GetColor();

		m_batchData[5] = color.x; m_batchData[6] = color.y; m_batchData[7] = color.z;
		m_batchData[13] = color.x; m_batchData[14] = color.y; m_batchData[15] = color.z; 
		m_batchData[21] = color.x; m_batchData[22] = color.y; m_batchData[23] = color.z; 
		m_batchData[29] = color.x; m_batchData[30] = color.y;  m_batchData[31] = color.z; 
	}

} // gui

} // sh