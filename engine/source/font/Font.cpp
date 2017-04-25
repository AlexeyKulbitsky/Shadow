#include "Font.h"
#include "FontManager.h"

namespace sh
{

	FontPtr Font::Create(const String& filename)
	{
		return FontManager::GetInstance()->CreateFont(filename);
	}

} // sh