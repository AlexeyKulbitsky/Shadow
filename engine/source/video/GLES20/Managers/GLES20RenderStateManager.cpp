#include "GLES20RenderStateManager.h"

#include "../GLES20RenderPipeline.h"
#include "../GLES20Shader.h"

namespace sh
{

namespace video
{

	RenderPipelinePtr GLES20RenderStateManager::CreateRenderPipeline(const RenderPipelineDescription& description)
	{
		RenderPipelinePtr result = nullptr;
		result.reset(new GLES20RenderPipeline(description));
		return result;
	}

	ShaderPtr GLES20RenderStateManager::CreateShader(const ShaderDescription& description)
	{
		ShaderPtr result = nullptr;
		result.reset(new GLES20Shader(description));
		return result;
	}

} // video

} // sh