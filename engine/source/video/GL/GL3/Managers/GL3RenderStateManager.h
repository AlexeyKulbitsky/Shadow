#ifndef SHADOW_GL3_RENDER_STATE_MANAGER_INCLUDE
#define SHADOW_GL3_RENDER_STATE_MANAGER_INCLUDE

#include "../../Managers/GLRenderStateManager.h"

namespace sh
{

namespace video
{

	class GL3RenderStateManager : public GLRenderStateManager
	{
	public:
		virtual RenderPipelinePtr CreateRenderPipeline(const RenderPipelineDescription& description) override;
		virtual VertexInputDeclarationPtr CreateVertexDeclaration() override;
	};

} // video

} // sh

#endif