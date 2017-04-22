#ifndef SHADOW_FONT_MANAGER_INCLUDE
#define SHADOW_FONT_MANAGER_INCLUDE

#include "../Globals.h"

namespace sh
{
	class FontManager : public Singleton < FontManager >
	{
	public:
		FontManager();
	};
}

#endif