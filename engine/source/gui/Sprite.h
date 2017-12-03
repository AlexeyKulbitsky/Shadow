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
		Sprite(const video::TexturePtr& texture, const math::Rect& rect);
		Sprite(const video::TexturePtr& texture, const math::Rect& rect, const math::Vector4& color);

		const video::TexturePtr& GetTexture() const { return m_texture; }
		const math::Rect& GetRect() const { return m_rect; }
		const math::Vector2& GetUpperLeftUV() const { return m_upperLeftUV; }
		const math::Vector2& GetLowerRightUV() const { return m_downlRightUV; }
		void SetName(const String& name) { m_name = m_name; }
		const String& GetName() const { return m_name; }
		const math::Vector4& GetColor() const { return m_color; }

	private:
		video::TexturePtr m_texture;
		math::Rect m_rect;
		// UV block
		math::Vector2 m_upperLeftUV;
		math::Vector2 m_downlRightUV;
		math::Vector4 m_color;
		String m_name;
	};

} // gui

} // sh

#endif