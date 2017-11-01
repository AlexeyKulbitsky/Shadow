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
		for (auto& color : m_colors)
		{
			color = math::Vector4f(1.0f);
		}
	}

	SpriteWidgetPtr SpriteWidget::Clone()
	{
		SpriteWidgetPtr result(new SpriteWidget());
		result->m_sprite = m_sprite;
		result->m_colors = m_colors;
		return result;
	}

	void SpriteWidget::SetColor(const math::Vector4f& _color)
	{
		for (auto& color : m_colors)
		{
			color = _color;
		}
	}

	void SpriteWidget::SetColor(const math::Vector4f& downLeft, const math::Vector4f& upperLeft, const math::Vector4f& upperRight, const math::Vector4f& downRight)
	{
		m_colors[0] = downLeft;
		m_colors[1] = upperLeft;
		m_colors[2] = upperRight;
		m_colors[3] = downRight;
	}

	void SpriteWidget::Render(video::Painter* painter)
	{
		if (!m_sprite)
			return;

		if (!m_visible)
			return;

		painter->SetMaterial(GuiManager::GetInstance()->GetDefaultMaterial());
// 		video::Painter::Vertex upperLeft(m_rect.upperLeftCorner, 
// 										 m_sprite->GetUVRect().upperLeftCorner, 
// 										 m_sprite->GetColor());
// 		video::Painter::Vertex downRight(m_rect.lowerRightCorner,
// 										 m_sprite->GetUVRect().lowerRightCorner,
// 										 m_sprite->GetColor());
// 		painter->DrawRect(upperLeft, downRight);

		video::Painter::Vertex downLeft(
			math::Vector2i(m_rect.upperLeftCorner.x, m_rect.lowerRightCorner.y),
			math::Vector2f(m_sprite->GetUVRect().upperLeftCorner.x, m_sprite->GetUVRect().lowerRightCorner.y),
			m_colors[0]);

		video::Painter::Vertex upperLeft(
			m_rect.upperLeftCorner,
			m_sprite->GetUVRect().upperLeftCorner,
			m_colors[1]);

		video::Painter::Vertex upperRight(
			math::Vector2i(m_rect.lowerRightCorner.x, m_rect.upperLeftCorner.y),
			math::Vector2f(m_sprite->GetUVRect().lowerRightCorner.x, m_sprite->GetUVRect().upperLeftCorner.y),
			m_colors[2]);

		video::Painter::Vertex downRight(
			m_rect.lowerRightCorner,
			m_sprite->GetUVRect().lowerRightCorner,
			m_colors[3]);

		painter->DrawQuad(downLeft, upperLeft, upperRight, downRight);
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