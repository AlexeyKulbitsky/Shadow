#include "video/Shader.h"
#include "video/Managers/RenderStateManager.h"

namespace sh
{

namespace video
{
    ShaderType Shader::GetType() const 
    { 
        return m_type; 
    }

	ShaderPtr Shader::Create(const ShaderDescription& description)
	{
		ShaderPtr result = RenderStateManager::GetInstance()->CreateShader(description);
		//result->m_paramsDescription = description.paramsDescription;
		//result->m_autoParamsDescription = description.autoParamsDescription;

		return result;
	}

} // video

} // sh