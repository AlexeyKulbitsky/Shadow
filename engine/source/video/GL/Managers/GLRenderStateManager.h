#ifndef SHADOW_GL_RENDER_STATE_MANAGER_INCLUDE
#define SHADOW_GL_RENDER_STATE_MANAGER_INCLUDE

#include "../../Managers/RenderStateManager.h"

namespace sh
{

namespace video
{

	class GLRenderStateManager : public RenderStateManager
	{
	public:
		virtual ShaderPtr CreateShader(const ShaderDescription& description) override;
	};

} // video

} // sh

#endif