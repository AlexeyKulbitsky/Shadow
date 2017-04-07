#ifndef SHADOW_RENDER_STATE_MANAGER_INCLUDE
#define SHADOW_RENDER_STATE_MANAGER_INCLUDE

#include "../Globals.h"
#include "../RenderPipeline.h"

namespace sh
{

namespace video
{

	class RenderStateManager
	{
	public: 
		static RenderStateManager* GetInstance() { return s_instance; }

		virtual RenderPipelinePtr CreateRenderPipeline(const RenderPipelineDescription& description) = 0;

	protected:
		static RenderStateManager* s_instance;
	};

} // video

} // sh

#endif