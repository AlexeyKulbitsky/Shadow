#include "RenderState.h"

namespace sh
{
	namespace video
	{
		RenderState::RenderState()
		{
			Init();
		}

		RenderState::~RenderState()
		{

		}

		void RenderState::Init()
		{
			SetFrontFace(FrontFace::COUNTER_CLOCKWISE);
			SetCullFace(CullFace::BACK);
			SetPolygonMode(PolygonMode::FILL);
			SetPointSize(1.0f);
			SetLineWidth(1.0f);
			EnableDepthTest(true);
			EnableScissorTest(false);
			EnableBlending(false);
			EnableCullFace(true);
			EnableStencilTest(false);
		}

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
					EnableDepthTest(value);
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
						SetFrontFace(FrontFace::CLOCKWISE);
					}
					else if (value == "counterClockWise")
					{
						SetFrontFace(FrontFace::COUNTER_CLOCKWISE);
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
					SetPointSize(value);
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
					SetLineWidth(value);
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
					EnableScissorTest(value);
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
					EnableBlending(value);
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
					EnableCullFace(value);
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
					EnableStencilTest(value);
				}
			}
		}
	}
}