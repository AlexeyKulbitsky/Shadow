#ifndef SHADOW_RASTERIZATION_STATE_INCLUDE
#define SHADOW_RASTERIZATION_STATE_INCLUDE

#include "../Globals.h"

namespace sh
{
	namespace video
	{
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

		////////////////////////////////////////////////////////////

		struct SHADOW_API RasterizationState
		{
			CullFace    cullFace = CullFace::BACK;
			FrontFace	frontFace = FrontFace::COUNTER_CLOCKWISE;
			FillMode    fillMode = FillMode::SOLID;
			bool		polygonOffsetEnabled = false;
			float		polygonOffsetFactor = 0.0f; //OffsetFactor
			float		polygonOffsetUnits = 0.0f; //DepthBias
			float		pointSize = 1.0f;
			float		lineWidth = 1.0f;

			//virtual void Load(const pugi::xml_node &node);
		};

		////////////////////////////////////////////////////////////

		/*
		void RasterizationState::Load(const pugi::xml_node &node)
		{
			pugi::xml_node stateNode;
			pugi::xml_attribute valAttr;

			// Front Face
			stateNode = node.child("frontFace");
			if (!stateNode.empty())
			{
				valAttr = stateNode.attribute("val");
				if (valAttr)
				{
					String value = valAttr.as_string();
					if (value == "clockWise")
					{
						frontFace = FrontFace::CLOCKWISE;
					}
					else if (value == "counterClockWise")
					{
						frontFace = FrontFace::COUNTER_CLOCKWISE;
					}
				}
			}

			// Cull face
			stateNode = node.child("cullFace");
			if (!stateNode.empty())
			{
				valAttr = stateNode.attribute("val");
				if (valAttr)
				{
					String value = valAttr.as_string();
					if (value == "front")
					{
						cullFace = CullFace::FRONT;
					}
					else if (value == "back")
					{
						cullFace = CullFace::BACK;
					}
					else if (value == "none")
					{
						cullFace = CullFace::NONE;
					}
				}
			}

			// Point size
			stateNode = node.child("pointSize");
			if (!stateNode.empty())
			{
				valAttr = stateNode.attribute("val");
				if (valAttr)
				{
					pointSize = valAttr.as_float();
				}
			}

			// Line width
			stateNode = node.child("lineWidth");
			if (!stateNode.empty())
			{
				valAttr = stateNode.attribute("val");
				if (valAttr)
				{
					lineWidth = valAttr.as_float();
				}
			}
		}
		*/
	}
}

#endif