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

	private:
		static void CreateInstance();
		static void DestroyInstance();
	};

} // video

} // sh

#endif