#ifndef SHADOW_FONT_INCLUDE
#define SHADOW_FONT_INCLUDE

#include "../Globals.h"

namespace sh
{
	struct GlyphDescription 
	{
		s32 x0, y0, x1, y1;	// coords of glyph in the texture atlas
		s32 x_off, y_off;   // left & top bearing when rendering
		s32 width, height;
		s32 advance;        // x advance when rendering
	};

	class Font
	{
		friend class FontManager;
	public:
		const video::TexturePtr& GetTextureAtlas() const { return m_textureatlas; }
		const GlyphDescription& GetGlyphDescription(u32 asciCode) const { return m_descriptions[asciCode]; }

		static FontPtr Create(const String& filename);

	private:
		GlyphDescription m_descriptions[128];
		video::TexturePtr m_textureatlas;
	};

} // sh

#endif