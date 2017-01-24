#include "RasterizationState.h"

namespace sh
{
	namespace video
	{
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
	}
}