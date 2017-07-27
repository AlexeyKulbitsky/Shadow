#include "Sprite.h"
#include "../video/Texture.h"

namespace sh
{

namespace gui
{

	Sprite::Sprite(const video::TexturePtr& texture, const math::Rectu& rect)
		: Sprite(texture, rect, math::Vector3f(1.0f))
	{
	}

	Sprite::Sprite(const video::TexturePtr& texture, const math::Rectu& rect, const math::Vector3f& color)
	{
		m_texture = texture;
		m_rect = rect;
		m_color = color;
		f32 textureWidth = static_cast<f32>(m_texture->GetDescription().width);
		f32 textureHeight = static_cast<f32>(m_texture->GetDescription().height);
		math::Vector2f leftUp(0.0f);
		leftUp.x = static_cast<f32>(m_rect.upperLeftCorner.x) / textureWidth;
		leftUp.y = static_cast<f32>(m_rect.upperLeftCorner.y) / textureHeight;

		math::Vector2f rightDown(0.0f);
		rightDown.x = static_cast<f32>(m_rect.lowerRightCorner.x) / textureWidth;
		rightDown.y = static_cast<f32>(m_rect.lowerRightCorner.y) / textureHeight;

		m_uvRect.Set(leftUp, rightDown);
	}

} // gui

} // sh