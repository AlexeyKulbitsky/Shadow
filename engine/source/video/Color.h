#ifndef SHADOW_COLOR_INCLUDE
#define SHADOW_COLOR_INCLUDE

#include "../Globals.h"

namespace sh
{
	namespace video
	{
		
		class SHADOW_API Color
		{
		public:
			float red;
			float green;
			float blue;
			float alpha;

			Color() : red(0.0f), green(0.0f), blue(0.0f), alpha(1.0f) {}
			Color(float r, float g, float b, float a) : red(r), green(g), blue(b), alpha(a) {}
			Color(float r, float g, float b) : red(r), green(g), blue(b), alpha(1.0f) {}
		};
	}
}
#endif