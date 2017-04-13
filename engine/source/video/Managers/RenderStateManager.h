#ifndef SHADOW_RENDER_STATE_MANAGER_INCLUDE
#define SHADOW_RENDER_STATE_MANAGER_INCLUDE

#include "../Globals.h"
#include "../RenderPipeline.h"
#include "../Shader.h"

namespace sh
{

namespace video
{

	class RenderStateManager : public Singleton<RenderStateManager>
	{
	public: 
		virtual RenderPipelinePtr CreateRenderPipeline(const RenderPipelineDescription& description) = 0;
		virtual ShaderPtr CreateShader(const ShaderDescription& description) = 0;
	};

} // video

} // sh

#endif