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
		return driver->CreateShader(description);
	}

} // video

} // sh