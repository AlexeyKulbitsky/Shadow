#include "VulkanShader.h"
#include "../../Device.h"
#include "VulkanDriver.h"

namespace sh
{

namespace video
{

	VulkanShader::VulkanShader(const ShaderDescription& description)
	{
		VulkanDriver* driver = static_cast<VulkanDriver*>(Device::GetInstance()->GetDriver());
		VkDevice device = driver->GetVulkanDevice();

		io::FileSystem* fs = Device::GetInstance()->GetFileSystem();
		const String tempFileName = "aaaaaaaaaa.txt";
		fs->SaveFile(description.source, tempFileName);

		const String utilName = "glslangValidator.exe";
		const auto& fileInfo = fs->FindFile(utilName);
		if (fileInfo.name != "")
		{
			const String executeStr = fileInfo.absolutePath + " -V " + fs->GetWorkingDirectory() + "/" + tempFileName + " -o " + fs->GetWorkingDirectory() + "/Askjdh.ttt";
			system(executeStr.c_str());
			
		}
		

		//..\..\..\libs\vulkan\bin\glslangValidator.exe -V test.frag
		std::vector<char> code;
		
		// Create shader module
		VkShaderModule shaderModule;
		VkShaderModuleCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		createInfo.codeSize = code.size();
		createInfo.pCode = (uint32_t*)code.data();

		VkResult res = vkCreateShaderModule(device, &createInfo, nullptr, &shaderModule);
		SH_ASSERT(res == VK_SUCCESS, "Failed to create shader module!");

		// Init shader
		m_stageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		m_stageInfo.stage = s_vkShaderType[description.type];
		m_stageInfo.module = shaderModule;
		m_stageInfo.pName = description.entryPoint.c_str();
	}

} // video

} // sh