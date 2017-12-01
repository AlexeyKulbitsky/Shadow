#ifndef SHADOW_SPRITE_INCLUDE
#define SHADOW_SPRITE_INCLUDE

#include "../Globals.h"

namespace sh
{

namespace gui
{

	class SHADOW_API Sprite
	{
	public:
		Sprite(const video::TexturePtr& texture, const math::Rectu& rect);
		Sprite(const video::TexturePtr& texture, const math::Rectu& rect, const math::Vector4f& color);

		const video::TexturePtr& GetTexture() const { return m_texture; }
		const math::Rectu& GetRect() const { return m_rect; }
		const math::Rectf& GetUVRect() const { return m_uvRect; }
		void SetName(const String& name) { m_name = m_name; }
		const String& GetName() const { return m_name; }
		const math::Vector4f& GetColor() const { return m_color; }

	private:
		video::TexturePtr m_texture;
		math::Rectu m_rect;
		math::Rectf m_uvRect;
		math::Vector4f m_color;
		String m_name;
	};

} // gui

} // sh

#endif