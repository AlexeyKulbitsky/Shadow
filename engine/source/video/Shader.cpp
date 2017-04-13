#include "Shader.h"
#include "Managers/RenderStateManager.h"

namespace sh
{

namespace video
{
	
	ShaderPtr Shader::Create(const ShaderDescription& description)
	{
		ShaderPtr result = RenderStateManager::GetInstance()->CreateShader(description);
		result->m_paramsDescription = description.paramsDescription;
		result->m_autoParamsDescription = description.autoParamsDescription;

		return result;
	}

} // video

} // sh