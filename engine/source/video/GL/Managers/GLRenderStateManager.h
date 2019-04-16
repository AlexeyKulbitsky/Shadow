#ifndef SHADOW_GL_RENDER_STATE_MANAGER_INCLUDE
#define SHADOW_GL_RENDER_STATE_MANAGER_INCLUDE

#include "video/Managers/RenderStateManager.h"

namespace sh
{

namespace video
{

	class GLRenderStateManager : public RenderStateManager
	{
	public:
		ShaderPtr CreateShader(const ShaderDescription& description) override final;
	};

} // video

} // sh

#endif