#include "ToolBar.h"

#include "Button.h"
#include "Sprite.h"
#include "Style.h"
#include "GuiManager.h"

#include "../video/Driver.h"
#include "../Device.h"

namespace sh
{

namespace gui
{

	ToolBar::ToolBar()
	{
		m_batchData.resize(4 * 8);

		video::Driver* driver = Device::GetInstance()->GetDriver();

		const auto& viewPort = driver->GetViewPort();
		math::Vector2u extends(viewPort.z, viewPort.w);

		m_rect.Set(0U, 15U, extends.x, 45U);

		m_sprite = GuiManager::GetInstance()->GetStyle()->GetToolBar()->m_sprite;
		/*
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
		*/
		UpdatePosition();
		UpdateUV(m_sprite->GetUVRect().upperLeftCorner, m_sprite->GetUVRect().lowerRightCorner);
		UpdateColor(m_sprite->GetColor());

		Device::GetInstance()->windowResizeEvent.Connect(std::bind(&ToolBar::OnWindowResized, this,
			std::placeholders::_1, std::placeholders::_2));
	}

	/////////////////////////////////////////////////////////////////////////////////////

	ToolBar::ToolBar(const SpritePtr& sprite)
	{
		m_batchData.resize(4 * 8);

		video::Driver* driver = Device::GetInstance()->GetDriver();

		const auto& viewPort = driver->GetViewPort();
		math::Vector2u extends(viewPort.z, viewPort.w);

		m_rect.Set(0U, 15U, extends.x, 45U);

		m_sprite = sprite;

		UpdatePosition();
		UpdateUV(m_sprite->GetUVRect().upperLeftCorner, m_sprite->GetUVRect().lowerRightCorner);
		UpdateColor(m_sprite->GetColor());

		Device::GetInstance()->windowResizeEvent.Connect(std::bind(&ToolBar::OnWindowResized, this,
			std::placeholders::_1, std::placeholders::_2));
	}

	/////////////////////////////////////////////////////////////////////////////////////

	void ToolBar::AddItem(const ButtonPtr& button)
	{
		u32 size = m_buttons.size();
		button->SetPosition(30 * (size), 15U);
		button->SetWidth(30U);
		button->SetHeight(30U);

		m_buttons.push_back(button); 
	}

	/////////////////////////////////////////////////////////////////////////////////////

	void ToolBar::Load(const pugi::xml_node& node)
	{

	}

	/////////////////////////////////////////////////////////////////////////////////////

	void ToolBar::GetGeometry(GuiBatchData& data)
	{
		GuiElement::GetGeometry(data);

		for (const auto& button : m_buttons)
		{
			button->GetGeometry(data);
		}
	}

	/////////////////////////////////////////////////////////////////////////////////////

	bool ToolBar::ProcessInput(u32 x, u32 y, MouseEventType type)
	{
		for (const auto& button : m_buttons)
		{
			if (button->ProcessInput(x, y, type))
				return true;
		}
		return false;
	}

	/////////////////////////////////////////////////////////////////////////////////////

	void ToolBar::OnWindowResized(int width, int)
	{
		m_rect.Set(0U, 15U, width, 45U);
		UpdatePosition();
	}

	/////////////////////////////////////////////////////////////////////////////////////

} // gui

} // sh