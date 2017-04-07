#ifndef SHADOW_VULKAN_SHADER_INCLUDE
#define SHADOW_VULKAN_SHADER_INCLUDE

#include "../Shader.h"
#include "VulkanCommon.h"

namespace sh
{

namespace video
{

	class VulkanShader : public Shader
	{
		friend class VulkanDriver;
	public:
		VkPipelineShaderStageCreateInfo GetVulkanId() const { return m_stageInfo; }

	private:
		VulkanShader(const ShaderDescription& description);
		std::vector<char> ReadFile(const String& filename);

	private:
		VkPipelineShaderStageCreateInfo m_stageInfo;
	};

} // video

} // sh

#endif
