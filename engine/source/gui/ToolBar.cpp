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
		: Widget()
	{

		video::Driver* driver = Device::GetInstance()->GetDriver();

		const auto& viewPort = driver->GetViewPort();
		math::Vector2u extends(viewPort.z, viewPort.w);

		m_rect.Set(0U, 15U, extends.x, 45U);

		m_sprite = GuiManager::GetInstance()->GetStyle()->GetToolBar()->m_sprite;

		Device::GetInstance()->windowResizeEvent.Connect(std::bind(&ToolBar::OnWindowResized, this,
			std::placeholders::_1, std::placeholders::_2));
	}

	/////////////////////////////////////////////////////////////////////////////////////

	ToolBar::ToolBar(const SpritePtr& sprite)
		: Widget()
	{
		video::Driver* driver = Device::GetInstance()->GetDriver();

		const auto& viewPort = driver->GetViewPort();
		math::Vector2u extends(viewPort.z, viewPort.w);

		m_rect.Set(0U, 15U, extends.x, 45U);

		m_sprite = sprite;

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

	void ToolBar::Render(video::Painter* painter)
	{
		painter->SetMaterial(GuiManager::GetInstance()->GetDefaultMaterial());
		video::Painter::Vertex upperLeft(m_rect.upperLeftCorner, 
										 m_sprite->GetUVRect().upperLeftCorner, 
										 m_sprite->GetColor());
		video::Painter::Vertex downRight(m_rect.lowerRightCorner,
										 m_sprite->GetUVRect().lowerRightCorner,
										 m_sprite->GetColor());
		painter->DrawRect(upperLeft, downRight);

		for (const auto& button : m_buttons)
		{
			button->Render(painter);
		}
	}

	/////////////////////////////////////////////////////////////////////////////////////

	bool ToolBar::ProcessEvent(GUIEvent& ev)
	{
		for (const auto& button : m_buttons)
		{
			if (button->ProcessEvent(ev))
				return true;
		}
		return false;
	}

	/////////////////////////////////////////////////////////////////////////////////////

	void ToolBar::OnWindowResized(int width, int)
	{
		m_rect.Set(0U, 15U, width, 45U);
	}

	/////////////////////////////////////////////////////////////////////////////////////

} // gui

} // sh