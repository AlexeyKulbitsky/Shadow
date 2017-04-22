#include "FontManager.h"
#include "../Device.h"

namespace sh
{
	FontManager::FontManager()
	{
		FT_Library  library;

		FT_Error error = FT_Init_FreeType( &library );
		if ( error )
		{
			SH_ASSERT(0, "ERROR initializing freetype library!");
		}

		FT_Face     face;

		
		//error = FT_New_Face( library, "d:/Sandbox/Shadow_engine/trunk/data/fonts/VeraMono-Italic.ttf", 0, &face);
		//if ( error )
		//{
		//	SH_ASSERT(0, "ERROR loading VeraMono-Italic.ttf font!, ");
		//}
		
		

	}
}