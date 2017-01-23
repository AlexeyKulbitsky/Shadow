#ifndef SHADOW_RENDER_STATE_INCLUDE
#define SHADOW_RENDER_STATE_INCLUDE

#include "../Globals.h"
#include "DepthStencilState.h"
#include "RasterizationState.h"
#include "BlendingState.h"

namespace sh
{
	namespace video
	{
		class SHADOW_API RenderState
		{
		public:
			RenderState();
			virtual ~RenderState();
			virtual void Load(const pugi::xml_node &node);
			virtual void Apply(){}
			virtual RenderState* Clone();

			BlendingState blendingState;
			RasterizationState rasterizerState;
			DepthStencilState depthStencilState;
		};
	}
}

#endif