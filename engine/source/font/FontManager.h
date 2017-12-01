#ifndef SHADOW_FONT_MANAGER_INCLUDE
#define SHADOW_FONT_MANAGER_INCLUDE

#include "../Globals.h"

typedef struct FT_LibraryRec_  *FT_Library;

namespace sh
{

	class SHADOW_API FontManager : public Singleton < FontManager >
	{
	public:
		struct GlyphInfo 
		{
			int x0, y0, x1, y1;	// coords of glyph in the texture atlas
			int x_off, y_off;   // left & top bearing when rendering
			int advance;        // x advance when rendering
		};

		FontManager();
		const video::TexturePtr& GetTexture();
		FontPtr GenerateFont(const String& filename);

	private:
		GlyphInfo m_infos[128];
		video::TexturePtr m_texture;

		FT_Library  m_fontLibrary;
	};
}

#endif