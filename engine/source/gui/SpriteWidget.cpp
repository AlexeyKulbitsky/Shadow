#include "SpriteWidget.h"

#include "GuiManager.h"
#include "../video/Driver.h"
#include "../serialization/ObjectFactory.h"

namespace sh
{

namespace gui
{

	SpriteWidget::SpriteWidget()
		: Widget()
	{
		for (auto& color : m_colors)
		{
			color = math::Vector4(1.0f);
		}
	}
    
    void SpriteWidget::RegisterObject()
    {
        ObjectFactory::GetInstance()->RegisterFactory<SpriteWidget>("UI");
        ObjectFactory::GetInstance()->RegisterParentProperties<SpriteWidget, Widget>();
    }

	SpriteWidgetPtr SpriteWidget::Clone()
	{
		SpriteWidgetPtr result(new SpriteWidget());
		result->m_sprite = m_sprite;
		result->m_colors = m_colors;
		return result;
	}

	void SpriteWidget::SetColor(const math::Vector4& _color)
	{
		for (auto& color : m_colors)
		{
			color = _color;
		}
	}

	void SpriteWidget::SetColor(const math::Vector4& downLeft, const math::Vector4& upperLeft, const math::Vector4& upperRight, const math::Vector4& downRight)
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
			math::Vector2Int(m_rect.upperLeftCorner.x, m_rect.lowerRightCorner.y),
			math::Vector2(m_sprite->GetUpperLeftUV().x, m_sprite->GetLowerRightUV().y),
			m_colors[0]);

		video::Painter::Vertex upperLeft(
			m_rect.upperLeftCorner,
			m_sprite->GetUpperLeftUV(),
			m_colors[1]);

		video::Painter::Vertex upperRight(
			math::Vector2Int(m_rect.lowerRightCorner.x, m_rect.upperLeftCorner.y),
			math::Vector2(m_sprite->GetLowerRightUV().x, m_sprite->GetUpperLeftUV().y),
			m_colors[2]);

		video::Painter::Vertex downRight(
			m_rect.lowerRightCorner,
			m_sprite->GetLowerRightUV(),
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
			m_sprite->GetUpperLeftUV(),
			m_sprite->GetColor());
		video::Painter::Vertex downRight(m_rect.lowerRightCorner,
			m_sprite->GetLowerRightUV(),
			m_sprite->GetColor());
		painter->DrawRect(upperLeft, downRight);
	}

} // gui

} // sh
