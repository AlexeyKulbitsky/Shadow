#ifndef SHADOW_GLES20_RENDER_STATE_MANAGER_INCLUDE
#define SHADOW_GLES20_RENDER_STATE_MANAGER_INCLUDE

#include "video/GL/Managers/GLRenderStateManager.h"

namespace sh
{

namespace video
{

	class GLES20RenderStateManager : public GLRenderStateManager
	{
	public:
		RenderPipelinePtr CreateRenderPipeline(const RenderPipelineDescription& description) override final;
		//virtual VertexInputDeclarationPtr CreateVertexDeclaration() override;
	};

} // video

} // sh

#endif