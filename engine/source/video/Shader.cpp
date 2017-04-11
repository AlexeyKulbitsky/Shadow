#include "Shader.h"
#include "../Device.h"
#include "Driver.h"

namespace sh
{

namespace video
{
	
	ShaderPtr Shader::Create(const ShaderDescription& description)
	{
		Driver* driver = Device::GetInstance()->GetDriver();
		ShaderPtr result = driver->CreateShader(description);
		result->m_paramsDescription = description.paramsDescription;

		return result;
	}

} // video

} // sh