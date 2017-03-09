#ifndef SHADOW_VULKAN_UNIFORM_BUFFER_INCLUDE
#define SHADOW_VULKAN_UNIFORM_BUFFER_INCLUDE

#include "../UniformBuffer.h"
#include "VulkanDeleter.h"

namespace sh
{
	namespace video
	{
		class VulkanUniformBuffer : public UniformBuffer
		{
		public:
			VulkanUniformBuffer();
			virtual ~VulkanUniformBuffer();

			virtual void Load(const pugi::xml_node &node) override;
			virtual void Init() override;
			virtual void Upload() override;
			virtual UniformBuffer* Clone() override;
			virtual void SetShaderProgram(ShaderProgram* shaderProgram) override;

			// Vulkan-specific inteface
			VkDescriptorSetLayout& GetDescriptorSetLayout() { return m_descriptorSetLayout; }
			VkPipelineLayout& GetPipelineLayout() { return m_pipelineLayout; }
			VkDescriptorSet GetDescriptorSet() const { return m_descriptorSet; }

		private:
			void LoadUniforms(const pugi::xml_node &node);
			void LoadSamplers(const pugi::xml_node &node);
			void createDecriptorPool();
			void createDescriptorSetLayout();
			void createDescriptorSet();

			void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
			void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
			uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

		private:
			VkDescriptorPool m_descriptorPool;
			VkDescriptorSet m_descriptorSet;
			VkDescriptorSetLayout m_descriptorSetLayout;
			VkPipelineLayout m_pipelineLayout;

			VkBuffer m_uniformStagingBuffer = VK_NULL_HANDLE;
			VkDeviceMemory m_uniformStagingBufferMemory = VK_NULL_HANDLE;	
			VkBuffer m_uniformBuffer = VK_NULL_HANDLE;			
			VkDeviceMemory m_uniformBufferMemory = VK_NULL_HANDLE;

			friend class VulkanRenderPipeline;
		};
	}
}

#endif
