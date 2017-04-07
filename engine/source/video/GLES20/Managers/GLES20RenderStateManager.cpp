#include "GLES20RenderStateManager.h"

#include "../GLES20RenderPipeline.h"

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

	void GLES20RenderStateManager::CreateInstance()
	{
		s_instance = new GLES20RenderStateManager();
	}

	void GLES20RenderStateManager::DestroyInstance()
	{
		delete s_instance;
		s_instance = nullptr;
	}

} // video

} // sh