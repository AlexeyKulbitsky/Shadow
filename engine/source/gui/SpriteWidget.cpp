#include "SpriteWidget.h"

#include "GuiManager.h"
#include "../video/Driver.h"

namespace sh
{

namespace gui
{

	SpriteWidget::SpriteWidget()
		: Widget()
	{

	}

	SpriteWidgetPtr SpriteWidget::Clone()
	{
		SpriteWidgetPtr result(new SpriteWidget());
		result->m_sprite = m_sprite;
		return result;
	}

	void SpriteWidget::Render(video::Painter* painter)
	{
		if (!m_sprite)
			return;

		if (!m_visible)
			return;

		painter->SetMaterial(GuiManager::GetInstance()->GetDefaultMaterial());
		video::Painter::Vertex upperLeft(m_rect.upperLeftCorner, 
										 m_sprite->GetUVRect().upperLeftCorner, 
										 m_sprite->GetColor());
		video::Painter::Vertex downRight(m_rect.lowerRightCorner,
										 m_sprite->GetUVRect().lowerRightCorner,
										 m_sprite->GetColor());
		painter->DrawRect(upperLeft, downRight);
	}

	void SpriteWidget::RenderBackground(video::Painter* painter)
	{
		if (!m_sprite)
			return;

		if (!m_visible)
			return;

		painter->SetMaterial(GuiManager::GetInstance()->GetDefaultMaterial());
		video::Painter::Vertex upperLeft(m_rect.upperLeftCorner,
			m_sprite->GetUVRect().upperLeftCorner,
			m_sprite->GetColor());
		video::Painter::Vertex downRight(m_rect.lowerRightCorner,
			m_sprite->GetUVRect().lowerRightCorner,
			m_sprite->GetColor());
		painter->DrawRect(upperLeft, downRight);
	}

} // gui

} // sh