#ifndef SHADOW_GEOMETRY_GENERATOR_INCLUDE
#define SHADOW_GEOMETRY_GENERATOR_INCLUDE

#include "../Globals.h"

namespace sh
{
	namespace scene
	{
		class Model;

		class GeometryGenerator
		{
		public:
			static Model* GetLineModel();
		};
	}
}

#endif