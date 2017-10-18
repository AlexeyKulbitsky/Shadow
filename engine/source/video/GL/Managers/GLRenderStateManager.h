#ifndef SHADOW_GL_RENDER_STATE_MANAGER_INCLUDE
#define SHADOW_GL_RENDER_STATE_MANAGER_INCLUDE

#include "../../Managers/RenderStateManager.h"

namespace sh
{

namespace video
{

	class GLRenderStateManager : public RenderStateManager
	{
	public:
		virtual RenderPipelinePtr CreateRenderPipeline(const RenderPipelineDescription& description) override;
		virtual ShaderPtr CreateShader(const ShaderDescription& description) override;
		virtual VertexInputDeclarationPtr CreateVertexDeclaration() override;
	};

} // video

} // sh

#endif