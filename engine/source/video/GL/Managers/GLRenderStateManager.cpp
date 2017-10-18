#include "GLRenderStateManager.h"

#include "../GLES20/GLES20RenderPipeline.h"
#include "../GLShader.h"
#include "../GLES20/GLES20VertexDeclaration.h"

namespace sh
{

namespace video
{

	RenderPipelinePtr GLRenderStateManager::CreateRenderPipeline(const RenderPipelineDescription& description)
	{
		RenderPipelinePtr result = nullptr;
		result.reset(new GLES20RenderPipeline(description));
		return result;
	}

	ShaderPtr GLRenderStateManager::CreateShader(const ShaderDescription& description)
	{
		ShaderPtr result = nullptr;
		result.reset(new GLShader(description));
		return result;
	}

	VertexInputDeclarationPtr GLRenderStateManager::CreateVertexDeclaration()
	{
		VertexInputDeclarationPtr result;
		result.reset(new GLES20VertexDeclaration());
		return result;
	}

} // video

} // sh