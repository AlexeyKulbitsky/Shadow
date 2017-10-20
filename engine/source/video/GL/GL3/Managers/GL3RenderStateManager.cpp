#include "GL3RenderStateManager.h"

#include "../GL3RenderPipeline.h"
#include "../GL3VertexDeclaration.h"

namespace sh
{

namespace video
{

	RenderPipelinePtr GL3RenderStateManager::CreateRenderPipeline(const RenderPipelineDescription& description)
	{
		RenderPipelinePtr result = nullptr;
		result.reset(new GL3RenderPipeline(description));
		return result;
	}

	VertexInputDeclarationPtr GL3RenderStateManager::CreateVertexDeclaration()
	{
		VertexInputDeclarationPtr result;
		result.reset(new GL3VertexDeclaration());
		return result;
	}

} // video

} // sh