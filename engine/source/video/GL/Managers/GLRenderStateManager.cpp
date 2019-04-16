#include "video/GL/Managers/GLRenderStateManager.h"
#include "video/GL/GLShader.h"

namespace sh
{

namespace video
{

	ShaderPtr GLRenderStateManager::CreateShader(const ShaderDescription& description)
	{
		ShaderPtr result = nullptr;
		result.reset(new GLShader(description));
		return result;
	}

} // video

} // sh