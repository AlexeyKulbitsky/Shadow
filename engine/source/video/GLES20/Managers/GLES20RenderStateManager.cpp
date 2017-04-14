#include "GLES20RenderStateManager.h"

#include "../GLES20RenderPipeline.h"
#include "../GLES20Shader.h"
#include "../GLES20Sampler.h"
#include "../GLES20VertexDeclaration.h"

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

	SamplerPtr GLES20RenderStateManager::CreateSampler(const SamplerDescription& description)
	{
		SamplerPtr result;
		result.reset(new GLES20Sampler(description));
		return result;
	}

	VertexInputDeclarationPtr GLES20RenderStateManager::CreateVertexDeclaration()
	{
		VertexInputDeclarationPtr result;
		result.reset(new GLES20VertexDeclaration());
		return result;
	}

} // video

} // sh