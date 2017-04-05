#ifndef SHADOW_VULKAN_SHADER_PROGRAM_INCLUDE
#define SHADOW_VULKAN_SHADER_PROGRAM_INCLUDE

#include "../ShaderProgram.h"
#include "VulkanVertexDeclaration.h"

namespace sh
{
	namespace video
	{
		struct VulkanShaderTemp
		{
			VkShaderModule shaderModule;
			ShaderType type;
		};

		class VulkanShaderProgram : public ShaderProgram
		{
		public:
			explicit VulkanShaderProgram(VkDevice device);
			virtual ~VulkanShaderProgram();

			virtual void Load(const char* filename) override;
			virtual void Load(const pugi::xml_node &node) override;
			virtual ShaderProgram* Clone() override;
			virtual void Unload() override;
			virtual void BindProgram() override;
			virtual void UnbindProgram() override;

			// Get shader info
			const std::vector<VkPipelineShaderStageCreateInfo>& GetShaderStages() const { return m_shaderStages; }
			const VkPipelineShaderStageCreateInfo* GetShaderStagesPointer() const { return m_shaderStages.data(); }
			size_t GetShaderStagesCount() const { return m_shaderStages.size(); }

		private:
			void LoadUniforms(const pugi::xml_node &node);
			void LoadAttributes(const pugi::xml_node &node);
			std::vector<char> ReadFile(const std::string& filename);
			void CompileShader(const std::vector<char>& code, VkShaderModule& shaderModule);

		private:
			VkDevice m_device = VK_NULL_HANDLE;
			VulkanVertexDeclaration m_vertexDeclaration;
			std::vector<VulkanShaderTemp> m_shaders;
			std::vector<VkPipelineShaderStageCreateInfo> m_shaderStages;
			
			VkPipeline m_graphicsPipeline;
		};
	}
}

#endif
