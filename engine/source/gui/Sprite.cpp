#include "Sprite.h"
#include "../video/Texture.h"

namespace sh
{

namespace gui
{

	Sprite::Sprite(const video::TexturePtr& texture, const math::Rect& rect)
		: Sprite(texture, rect, math::Vector4(1.0f))
	{
	}

	Sprite::Sprite(const video::TexturePtr& texture, const math::Rect& rect, const math::Vector4& color)
	{
		m_texture = texture;
		m_rect = rect;
		m_color = color;
		f32 textureWidth = static_cast<f32>(m_texture->GetDescription().width);
		f32 textureHeight = static_cast<f32>(m_texture->GetDescription().height);
		m_upperLeftUV.x = static_cast<f32>(m_rect.upperLeftCorner.x) / textureWidth;
		m_upperLeftUV.y = static_cast<f32>(m_rect.upperLeftCorner.y) / textureHeight;

		m_downlRightUV.x = static_cast<f32>(m_rect.lowerRightCorner.x) / textureWidth;
		m_downlRightUV.y = static_cast<f32>(m_rect.lowerRightCorner.y) / textureHeight;
	}

} // gui

} // sh