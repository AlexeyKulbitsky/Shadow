#include "RasterizationState.h"

#include <pugixml.hpp>

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
						frontFace = FrontFace::FF_CLOCKWISE;
					}
					else if (value == "counterClockWise")
					{
						frontFace = FrontFace::FF_COUNTER_CLOCKWISE;
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
						cullFace = CullFace::CF_FRONT;
					}
					else if (value == "back")
					{
						cullFace = CullFace::CF_BACK;
					}
					else if (value == "none")
					{
						cullFace = CullFace::CF_NONE;
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