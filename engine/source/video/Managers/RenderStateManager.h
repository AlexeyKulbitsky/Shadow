#ifndef SHADOW_RENDER_STATE_MANAGER_INCLUDE
#define SHADOW_RENDER_STATE_MANAGER_INCLUDE

#include "utility/Singleton.h"
#include "video/RenderPipeline.h"
#include "video/Shader.h"

namespace sh
{

namespace video
{

	class RenderStateManager : public Singleton<RenderStateManager>
	{
	public:
        virtual ~RenderStateManager();
		virtual RenderPipelinePtr CreateRenderPipeline(const RenderPipelineDescription& description) = 0;
		virtual ShaderPtr CreateShader(const ShaderDescription& description) = 0;
		//virtual SamplerPtr CreateSampler(const SamplerDescription& description);
		//virtual VertexInputDeclarationPtr CreateVertexDeclaration() = 0;
		//virtual GpuPipelineParamsInfoPtr CreateGpuPipelineParamsInfo(const GpuPipelineParamsDescription& description);
	};

} // video

} // sh

#endif