#include "DepthStencilState.h"

namespace sh
{
	namespace video
	{
		void DepthStencilState::Load(const pugi::xml_node &node)
		{
			pugi::xml_node stateNode;
			pugi::xml_attribute valAttr;

			// Depth test			
			stateNode = node.child("depthTest");
			if (!stateNode.empty())
			{
				valAttr = stateNode.attribute("val");
				if (valAttr)
				{
					bool value = valAttr.as_bool();
					enableDepthTest = value;
				}
			}

			// Stencil test
			stateNode = node.child("stencilTest");
			if (!stateNode.empty())
			{
				valAttr = stateNode.attribute("val");
				if (valAttr)
				{
					bool value = valAttr.as_bool();
					enableStencilTest = value;
				}
			}
		}
	}
}