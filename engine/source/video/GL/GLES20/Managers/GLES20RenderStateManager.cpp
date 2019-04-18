#include "GLES20RenderStateManager.h"

#include "video/GL/GLES20/Managers/GLES20RenderStateManager.h"
#include "video/GL/GLES20/GLES20RenderPipeline.h"
#include "video/GL/GLES20/GLES20VertexInputDeclaration.h"

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
		result.reset(new GLES20VertexInputDeclaration());
		return result;
	}

} // video

} // sh