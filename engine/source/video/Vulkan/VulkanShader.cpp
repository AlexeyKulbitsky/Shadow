#include "VulkanShader.h"
#include "../../Device.h"
#include "VulkanDriver.h"
#include "VulkanShaderCompiler.h"

#include <fstream>

namespace sh
{

namespace video
{

	VulkanShader::VulkanShader(const ShaderDescription& description)
	{
		m_type = description.type;

		VulkanDriver* driver = static_cast<VulkanDriver*>(Device::GetInstance()->GetDriver());
		VkDevice device = driver->GetVulkanDevice();

		std::vector<unsigned int> code = VulkanShaderCompiler::GetInstance()->CompileShader(description.source, description.type);

		// Create shader module		
		VkShaderModuleCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		createInfo.codeSize = code.size() * sizeof(unsigned int);
		createInfo.pCode = code.data();
		
		VkShaderModule shaderModule;
		VkResult res = vkCreateShaderModule(device, &createInfo, nullptr, &shaderModule);
		SH_ASSERT(res == VK_SUCCESS, "Failed to create shader module!");

		// Init shader
		m_stageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		m_stageInfo.stage = s_vkShaderType[description.type];
		m_stageInfo.module = shaderModule;
		m_stageInfo.pName = "main";//description.entryPoint.c_str();
		m_stageInfo.flags = 0;
		m_stageInfo.pNext = nullptr;
		m_stageInfo.pSpecializationInfo = nullptr;
	}


	std::vector<char> VulkanShader::ReadFile(const String& filename)
	{
		std::ifstream file(filename, std::ios::ate | std::ios::binary);

		SH_ASSERT(file.is_open(), "failed to open file!");

		size_t fileSize = (size_t)file.tellg();
		std::vector<char> buffer(fileSize);

		file.seekg(0);
		file.read(buffer.data(), fileSize);
		file.close();

		return buffer;
	}


} // video

} // sh