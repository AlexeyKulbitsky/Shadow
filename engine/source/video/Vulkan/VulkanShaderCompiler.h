#ifndef SHADOW_VULKAN_SHADER_COMPILER_INCLUDE
#define SHADOW_VULKAN_SHADER_COMPILER_INCLUDE

#include "../../Globals.h"


namespace sh
{

namespace video
{
	class VulkanShaderCompiler : public Singleton<VulkanShaderCompiler>
	{
	public:
		VulkanShaderCompiler();
		std::vector<unsigned int> CompileShader(const String& source, ShaderType type);
	};
} // video

} // sh

#endif