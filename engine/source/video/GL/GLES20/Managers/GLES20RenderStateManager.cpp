#include "GLES20RenderStateManager.h"

#include "../GLES20RenderPipeline.h"
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

	VertexInputDeclarationPtr GLES20RenderStateManager::CreateVertexDeclaration()
	{
		VertexInputDeclarationPtr result;
		result.reset(new GLES20VertexDeclaration());
		return result;
	}

} // video

} // sh