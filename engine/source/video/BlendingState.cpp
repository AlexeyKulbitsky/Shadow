#include "BlendingState.h"

namespace sh
{
	namespace video
	{
		void BlendingState::Load(const pugi::xml_node &node)
		{
			pugi::xml_node stateNode;
			pugi::xml_attribute valAttr;
					
			stateNode = node.child("srcFactor");
			if (!stateNode.empty())
			{
				valAttr = stateNode.attribute("val");
				if (valAttr)
				{
					String value = valAttr.as_string();

					for (size_t i = 0U; i < static_cast<size_t>( BlendFactor::COUNT ); ++i)
					{
						if (value == g_blendFactorMap[i])
						{
							srcAlpha = static_cast<BlendFactor>(i);
							break;
						}
					}
				}
			}

			stateNode = node.child("dstFactor");
			if (!stateNode.empty())
			{
				valAttr = stateNode.attribute("val");
				if (valAttr)
				{
					String value = valAttr.as_string();

					for (size_t i = 0U; i < static_cast<size_t>( BlendFactor::COUNT ); ++i)
					{
						if (value == g_blendFactorMap[i])
						{
							dstAlpha = static_cast<BlendFactor>(i);
							break;
						}
					}
				}
			}

			stateNode = node.child("operation");
			if (!stateNode.empty())
			{
				valAttr = stateNode.attribute("val");
				if (valAttr)
				{
					String value = valAttr.as_string();

					for (size_t i = 0U; i < static_cast<size_t>( BlendOperation::COUNT ); ++i)
					{
						if (value == g_blendOperationMap[i])
						{
							operationAlpha = static_cast<BlendOperation>(i);
							break;
						}
					}
				}
			}


		}
	}
}