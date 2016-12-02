#ifndef SHADOW_RESOURCE_INCLUDE
#define SHADOW_RESOURCE_INCLUDE

#include "../Globals.h"

namespace sh
{
	class Resource
	{
	public:
		enum class Group
		{
			TEXTURES = 0,
			MODELS = 1,
			TECHNIQUES = 2,

			COUNT
		};
	};
}

#endif