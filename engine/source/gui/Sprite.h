#ifndef SHADOW_SPRITE_INCLUDE
#define SHADOW_SPRITE_INCLUDE

#include "../Globals.h"

namespace sh
{

namespace gui
{

	class Sprite
	{
	public:
		Sprite(const video::TexturePtr& texture, const math::Rectu& rect);
	};

} // gui

} // sh

#endif