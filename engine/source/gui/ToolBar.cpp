#include "ToolBar.h"

#include "Button.h"
#include "Sprite.h"
#include "Style.h"
#include "GuiManager.h"
#include "HorizontalLayout.h"

#include "../video/Driver.h"
#include "../Device.h"

#include <pugixml.hpp>

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
		if (m_layout->GetItemsCount() == 0U)
			button->SetToggled(true);

		button->SetMaximumWidth(30);
		m_layout->AddWidget(button);
	}

	/////////////////////////////////////////////////////////////////////////////////////

	void ToolBar::Load(const pugi::xml_node& node)
	{

	}

	/////////////////////////////////////////////////////////////////////////////////////

	void ToolBar::Render(video::Painter* painter)
	{
		if (!m_visible)
			return;

		painter->SetMaterial(GuiManager::GetInstance()->GetDefaultMaterial());
		video::Painter::Vertex upperLeft(m_rect.upperLeftCorner, 
										 m_sprite->GetUpperLeftUV(), 
										 m_sprite->GetColor());
		video::Painter::Vertex downRight(m_rect.lowerRightCorner,
										 m_sprite->GetLowerRightUV(),
										 m_sprite->GetColor());
		painter->DrawRect(upperLeft, downRight);

		Widget::Render(painter);
	}

	void ToolBar::RenderBackground(video::Painter* painter)
	{
		if (!m_visible)
			return;

		painter->SetMaterial(GuiManager::GetInstance()->GetDefaultMaterial());
		video::Painter::Vertex upperLeft(m_rect.upperLeftCorner,
			m_sprite->GetUpperLeftUV(),
			m_sprite->GetColor());
		video::Painter::Vertex downRight(m_rect.lowerRightCorner,
			m_sprite->GetLowerRightUV(),
			m_sprite->GetColor());
		painter->DrawRect(upperLeft, downRight);

		Widget::RenderBackground(painter);
	}

	void ToolBar::RenderText(video::Painter* painter)
	{
		Widget::RenderText(painter);
	}

	/////////////////////////////////////////////////////////////////////////////////////

	bool ToolBar::ProcessEvent(GUIEvent& ev)
	{
		if (Widget::ProcessEvent(ev))
			return true;

		bool isInside = m_rect.IsPointInside(ev.x, ev.y);
		return isInside;
	}

	/////////////////////////////////////////////////////////////////////////////////////

} // gui

} // sh