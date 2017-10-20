#include "GLRenderStateManager.h"

#include "../GLShader.h"

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