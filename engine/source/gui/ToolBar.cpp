#include "ToolBar.h"

#include "Button.h"
#include "Sprite.h"
#include "Style.h"
#include "GuiManager.h"
#include "HorizontalLayout.h"

#include "../video/Driver.h"
#include "../Device.h"

namespace sh
{

namespace gui
{

	ToolBar::ToolBar()
		: Widget()
	{
		SetMaximumHeight(30);
		m_sprite = GuiManager::GetInstance()->GetStyle()->GetToolBar()->m_sprite;

		HorizontalLayoutPtr layout(new HorizontalLayout());
		SetLayout(layout);
	}

	/////////////////////////////////////////////////////////////////////////////////////

	ToolBar::ToolBar(const SpritePtr& sprite)
		: Widget()
	{
		SetMaximumHeight(30);
		m_sprite = sprite;

		HorizontalLayoutPtr layout(new HorizontalLayout());
		SetLayout(layout);
	}

	/////////////////////////////////////////////////////////////////////////////////////

	void ToolBar::AddItem(const ButtonPtr& button)
	{
		button->SetMaximumWidth(30);
		m_layout->AddWidget(button);

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

} // gui

} // sh