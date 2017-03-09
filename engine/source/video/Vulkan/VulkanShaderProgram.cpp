#include "VulkanShaderProgram.h"
#include "../../Device.h"

#include <fstream>

namespace sh
{
	namespace video
	{
		VulkanShaderProgram::VulkanShaderProgram(VkDevice device)
			: m_device(device)
		{

		}

		///////////////////////////////////////////////////////////////////////

		VulkanShaderProgram::~VulkanShaderProgram()
		{
			for (size_t i = 0, sz = m_shaders.size(); i < sz; ++i)
			{
				vkDestroyShaderModule(m_device, m_shaders[i].shaderModule, nullptr);
			}

			vkDestroyPipeline(m_device, m_graphicsPipeline, nullptr);
		}

		///////////////////////////////////////////////////////////////////////

		void VulkanShaderProgram::Load(const char* filename)
		{
			pugi::xml_document doc;
			pugi::xml_parse_result result = doc.load_file(filename);
			pugi::xml_node shaderProgramNode = doc.first_child();

			// Load uniforms
			pugi::xml_node uniformsNode = shaderProgramNode.child("uniforms");
			LoadUniforms(uniformsNode);

			// Load attributes
			pugi::xml_node attributesNode = shaderProgramNode.child("attributes");
			LoadAttributes(attributesNode);

			// Load vertex shader data
			pugi::xml_node vertexShaderSrcNode = shaderProgramNode.child("vertexShader");
			if (vertexShaderSrcNode)
			{
				pugi::xml_node generatedPathNode = vertexShaderSrcNode.child("generated");
				std::string path = generatedPathNode.attribute("path").as_string();

				std::vector<char> vertShaderCode = ReadFile(path);

				VulkanShader vertexShader;
				vertexShader.type = ShaderType::VERTEX;
				CompileShader(vertShaderCode, vertexShader.shaderModule);
				m_shaders.push_back(vertexShader);

				VkPipelineShaderStageCreateInfo vertShaderStageInfo = {};
				vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
				vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
				vertShaderStageInfo.module = vertexShader.shaderModule;
				vertShaderStageInfo.pName = "main";

				m_shaderStages.push_back(vertShaderStageInfo);
			}

			// Load fragment shader data
			pugi::xml_node fragmentShaderSrcNode = shaderProgramNode.child("fragmentShader");
			if (fragmentShaderSrcNode)
			{
				pugi::xml_node generatedPathNode = vertexShaderSrcNode.child("generated");
				std::string path = generatedPathNode.attribute("path").as_string();

				std::vector<char> fragmentShaderCode = ReadFile(path);

				VulkanShader fragmentShader;
				fragmentShader.type = ShaderType::FRAGMENT;
				CompileShader(fragmentShaderCode, fragmentShader.shaderModule);
				m_shaders.push_back(fragmentShader);

				VkPipelineShaderStageCreateInfo fragmentShaderStageInfo = {};
				fragmentShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
				fragmentShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
				fragmentShaderStageInfo.module = fragmentShader.shaderModule;
				fragmentShaderStageInfo.pName = "main";

				m_shaderStages.push_back(fragmentShaderStageInfo);
			}
		}

		///////////////////////////////////////////////////////////////////////

		void VulkanShaderProgram::Load(const pugi::xml_node &node)
		{
			// Load vertex shader data
			pugi::xml_node vertexShaderSrcNode = node.child("vertexShader");
			if (vertexShaderSrcNode)
			{
				pugi::xml_node sourceNode = vertexShaderSrcNode.child("source");
				String path = sourceNode.attribute("val").as_string();

				const sh::io::FileInfo& info = Device::GetInstance()->GetFileSystem()->FindFile(path);
				if (info.absolutePath == "")
				{
					SH_ASSERT(0, "Can not find shader file %s in file system!!!", path.c_str());
				}

				std::vector<char> vertShaderCode = ReadFile(info.absolutePath);

				VulkanShader vertexShader;
				vertexShader.type = ShaderType::VERTEX;
				CompileShader(vertShaderCode, vertexShader.shaderModule);
				m_shaders.push_back(vertexShader);

				VkPipelineShaderStageCreateInfo vertShaderStageInfo = {};
				vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
				vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
				vertShaderStageInfo.module = vertexShader.shaderModule;
				vertShaderStageInfo.pName = "main";

				m_shaderStages.push_back(vertShaderStageInfo);
			}

			// Load fragment shader data
			pugi::xml_node fragmentShaderSrcNode = node.child("fragmentShader");
			if (fragmentShaderSrcNode)
			{
				pugi::xml_node sourceNode = fragmentShaderSrcNode.child("source");
				String path = sourceNode.attribute("val").as_string();

				const sh::io::FileInfo& info = Device::GetInstance()->GetFileSystem()->FindFile(path);
				if (info.absolutePath == "")
				{
					SH_ASSERT(0, "Can not find shader file %s in file system!!!", path.c_str());
				}

				std::vector<char> fragmentShaderCode = ReadFile(info.absolutePath);			

				VulkanShader fragmentShader;
				fragmentShader.type = ShaderType::FRAGMENT;
				CompileShader(fragmentShaderCode, fragmentShader.shaderModule);
				m_shaders.push_back(fragmentShader);

				VkPipelineShaderStageCreateInfo fragmentShaderStageInfo = {};
				fragmentShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
				fragmentShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
				fragmentShaderStageInfo.module = fragmentShader.shaderModule;
				fragmentShaderStageInfo.pName = "main";

				m_shaderStages.push_back(fragmentShaderStageInfo);
			}
		}

		///////////////////////////////////////////////////////////////////////

		void VulkanShaderProgram::Unload()
		{

		}

		///////////////////////////////////////////////////////////////////////

		ShaderProgram* VulkanShaderProgram::Clone()
		{
			return nullptr;
		}

		///////////////////////////////////////////////////////////////////////

		void VulkanShaderProgram::BindProgram()
		{

		}

		///////////////////////////////////////////////////////////////////////

		void VulkanShaderProgram::UnbindProgram()
		{

		}

		///////////////////////////////////////////////////////////////////////

		void VulkanShaderProgram::LoadUniforms(const pugi::xml_node &node)
		{
			if (node)
			{
				pugi::xml_node uniformNode = node.first_child();
				while (!uniformNode.empty())
				{
					std::string name = uniformNode.name();
					printf("uniform : %s\n", name.c_str());

					uniformNode = uniformNode.next_sibling();
				}
			}
		}

		///////////////////////////////////////////////////////////////////////

		void VulkanShaderProgram::LoadAttributes(const pugi::xml_node &node)
		{
			if (node)
			{
				pugi::xml_node attributeNode = node.first_child();
				while (!attributeNode.empty())
				{
					VulkanVertexAttribute attribute;

					std::string name = attributeNode.name();
					if (name == "position")
					{
						attribute.semantic = AttributeSemantic::POSITION;
						printf("Postion attribute\n");
					}
					else if (name == "normal")
					{
						attribute.semantic = AttributeSemantic::NORMAL;
						printf("Normal attribute\n");
					}
					else if (name == "color")
					{
						attribute.semantic = AttributeSemantic::COLOR;
						printf("Color attribute\n");
					}
					else if (name == "uv")
					{
						attribute.semantic = AttributeSemantic::UV;
						printf("UV attribute\n");
					}
					else
					{
						printf("Unknown attribute\n");
					}

					attribute.name = attributeNode.attribute("name").as_string();
					attribute.vulkanAttribute.location = attributeNode.attribute("location").as_uint();

					pugi::xml_attribute typeAttr = attributeNode.attribute("type");
					name = typeAttr.as_string();

					if (name == "vec3")
					{
						attribute.vulkanAttribute.format = VK_FORMAT_R32G32B32_SFLOAT;
					}
					else if (name == "vec2")
					{
						attribute.vulkanAttribute.format = VK_FORMAT_R32G32_SFLOAT;
					}

					m_vertexDeclaration.AddAttribute(attribute);

					attributeNode = attributeNode.next_sibling();
				}
			}
			printf("Done\n");
		}

		///////////////////////////////////////////////////////////////////////

		std::vector<char> VulkanShaderProgram::ReadFile(const std::string& filename)
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

		///////////////////////////////////////////////////////////////////////

		void VulkanShaderProgram::CompileShader(const std::vector<char>& code, VkShaderModule& shaderModule)
		{
			VkShaderModuleCreateInfo createInfo = {};
			createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
			createInfo.codeSize = code.size();
			createInfo.pCode = (uint32_t*)code.data();

			SH_ASSERT(vkCreateShaderModule(m_device, &createInfo, nullptr, &shaderModule) == VK_SUCCESS,
				"failed to create shader module!");
		}

		///////////////////////////////////////////////////////////////////////
	}
}