#ifndef SHADOW_GLES20_RENDER_STATE_MANAGER_INCLUDE
#define SHADOW_GLES20_RENDER_STATE_MANAGER_INCLUDE

#include "../../Managers/RenderStateManager.h"

namespace sh
{

namespace video
{

	class GLES20RenderStateManager : public RenderStateManager
	{
		friend class GLES20Driver;
	public:
		virtual RenderPipelinePtr CreateRenderPipeline(const RenderPipelineDescription& description) override;
		virtual ShaderPtr CreateShader(const ShaderDescription& description) override;
		virtual SamplerPtr CreateSampler(const SamplerDescription& description) override;
		virtual VertexInputDeclarationPtr CreateVertexDeclaration() override;
	};

} // video

} // sh

#endif