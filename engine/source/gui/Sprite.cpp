#include "Sprite.h"

namespace sh
{

namespace gui
{

	Sprite::Sprite(const video::TexturePtr& texture, const math::Rectu& rect)
	{
		m_texture = texture;
		m_rect = rect;
	}

} // gui

} // sh