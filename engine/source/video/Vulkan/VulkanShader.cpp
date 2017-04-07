#include "VulkanShader.h"
#include "../../Device.h"
#include "VulkanDriver.h"

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

		io::FileSystem* fs = Device::GetInstance()->GetFileSystem();
		
		String extension;
		switch (description.type)
		{
		case ST_VERTEX:
			extension = ".vert";
			break;
		case ST_FRAGMENT:
			extension = ".frag";
			break;
		case ST_GEOMETRY:
			extension = ".geom";
			break;
		case ST_TESSELATION_EVALUATION:
			extension = ".tese";
			break;
		case ST_TESSELATION_CONTROL:
			extension = ".tesc";
			break;
		case ST_COMPUTE:
			extension = ".comp";
			break;
		default:
			break;
		}
		const String tempFileName = "temp";
		fs->SaveFile(description.source, tempFileName + extension);

		const String utilName = "glslangValidator.exe";
		const auto& fileInfo = fs->FindFile(utilName);
		if (fileInfo.name != "")
		{
			const String executeStr = fileInfo.absolutePath + " -V " + fs->GetWorkingDirectory() + "/" + tempFileName + extension + " -o " + fs->GetWorkingDirectory() + tempFileName + extension + ".spv";
			system(executeStr.c_str());		
		}
		
		std::vector<char> code = std::move(ReadFile(fs->GetWorkingDirectory() + tempFileName + extension + ".spv"));

		// Create shader module		
		VkShaderModuleCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		createInfo.codeSize = code.size();
		createInfo.pCode = (uint32_t*)code.data();
		
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
		m_stageInfo.pSpecializationInfo = NULL;
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