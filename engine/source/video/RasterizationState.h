#ifndef SHADOW_RASTERIZATION_STATE_INCLUDE
#define SHADOW_RASTERIZATION_STATE_INCLUDE

#include "../Globals.h"

namespace sh
{
	namespace video
	{
		class SHADOW_API RasterizationState
		{
		public:
			CullFace    cullFace = CF_BACK;
			FrontFace	frontFace = FrontFace::FF_COUNTER_CLOCKWISE;
			FillMode    fillMode = FM_SOLID;
			bool		polygonOffsetEnabled = false;
			float		polygonOffsetFactor = 0.0f; //OffsetFactor
			float		polygonOffsetUnits = 0.0f; //DepthBias
			float		pointSize = 1.0f;
			float		lineWidth = 1.0f;

			void Load(const pugi::xml_node &node);
		};
	}
}

#endif