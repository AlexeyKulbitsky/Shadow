#ifndef SHADOW_RASTERIZATION_STATE_INCLUDE
#define SHADOW_RASTERIZATION_STATE_INCLUDE

#include "../Globals.h"

namespace sh
{
	namespace video
	{
		/*
		enum class SHADOW_API FrontFace
		{
			CLOCKWISE = 0,
			COUNTER_CLOCKWISE,

			COUNT
		};

		static const char* const g_frontFaceMap[] = 
		{
			"ClockWise",
			"CounterClockWise"
		};

		////////////////////////////////////////////////////////////

		enum class SHADOW_API CullFace
		{
			FRONT = 0,
			BACK,
			NONE,

			COUNT
		};

		static const char* const g_cullFaceMap[] = 
		{
			"Front",
			"Back",
			"None"
		};

		////////////////////////////////////////////////////////////

		enum class SHADOW_API FillMode
		{
			SOLID = 0,
			WIREFRAME,

			COUNT
		};

		static const char* const g_fillModeMap[] = 
		{
			"Solid",
			"WireFrame"
		};
		*/
		////////////////////////////////////////////////////////////

		class SHADOW_API RasterizationState
		{
		public:
			RasterizationState* Clone();

			CullFace    cullFace = CullFace::BACK;
			FrontFace	frontFace = FrontFace::COUNTER_CLOCKWISE;
			FillMode    fillMode = FillMode::SOLID;
			bool		polygonOffsetEnabled = false;
			float		polygonOffsetFactor = 0.0f; //OffsetFactor
			float		polygonOffsetUnits = 0.0f; //DepthBias
			float		pointSize = 1.0f;
			float		lineWidth = 1.0f;

			void Load(const pugi::xml_node &node);
		};

		////////////////////////////////////////////////////////////

		inline RasterizationState* RasterizationState::Clone()
		{
			RasterizationState* result = new RasterizationState();
			(*result) = (*this);
			return result;
		}
	}
}

#endif