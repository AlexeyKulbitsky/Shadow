#ifndef SHADOW_LIGHT_INCLUDE
#define SHADOW_LIGHT_INCLUDE

#include "../Globals.h"

namespace sh
{
	namespace scene
	{
		class Light
		{
		public:
			enum class Type
			{
				POINT,
				DIRECTIONAL,
				SPOT
			};

			Light();
			~Light();

		private:
			math::Vector3f m_positionDirection;
		};
	}
}

#endif