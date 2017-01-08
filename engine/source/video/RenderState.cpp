#include "RenderState.h"

namespace sh
{
	namespace video
	{
		RenderState::RenderState()
		{
		}

		///////////////////////////////////////////////////////////////

		RenderState::~RenderState()
		{
		}

		///////////////////////////////////////////////////////////////

		void RenderState::Load(const pugi::xml_node &node)
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
					depthStencilState.enableDepthTest = value;
				}
			}

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
						rasterizerState.frontFace = FrontFace::CLOCKWISE;
					}
					else if (value == "counterClockWise")
					{
						rasterizerState.frontFace = FrontFace::COUNTER_CLOCKWISE;
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
					if( value == "front" )
					{

					}
					else if( value == "back" )
					{

					}
				}
			}

			// Point size
			stateNode = node.child("pointSize");
			if( !stateNode.empty() )
			{
				valAttr = stateNode.attribute("val");
				if(valAttr)
				{
					float value = valAttr.as_float();	
				}
			}

			// Line width
			stateNode = node.child("lineWidth");
			if( !stateNode.empty() )
			{
				valAttr = stateNode.attribute("val");
				if(valAttr)
				{
					float value = valAttr.as_float();
				}
			}

			// Scissor test
			stateNode = node.child("scissorTest");			
			if (!stateNode.empty())
			{
				valAttr = stateNode.attribute("val");
				if (valAttr)
				{
					bool value = valAttr.as_bool();
				}
			}

			// Blending
			stateNode = node.child("blending");			
			if (!stateNode.empty())
			{
				valAttr = stateNode.attribute("val");
				if (valAttr)
				{
					bool value = valAttr.as_bool();
				}
			}

			// Culling
			stateNode = node.child("culling");			
			if (!stateNode.empty())
			{
				valAttr = stateNode.attribute("val");
				if (valAttr)
				{
					bool value = valAttr.as_bool();
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
					depthStencilState.enableStencilTest = value;
				}
			}
		}

		///////////////////////////////////////////////////////////////

		RenderState* RenderState::Clone()
		{
			RenderState* renderState = new RenderState();

			renderState->depthStencilState = depthStencilState;
			renderState->rasterizerState = rasterizerState;
			renderState->blendingState = blendingState;

			return renderState;
		}

		///////////////////////////////////////////////////////////////
	}
}