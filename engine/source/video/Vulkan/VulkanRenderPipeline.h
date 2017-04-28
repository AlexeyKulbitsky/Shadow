#ifndef SHADOW_VULKAN_RENDER_PIPELINE_INCLUDE
#define SHADOW_VULKAN_RENDER_PIPELINE_INCLUDE

#include "../RenderPipeline.h"
#include "VulkanCommon.h"

namespace sh
{
	namespace video
	{
		class VulkanVertexDeclaration;

		class VulkanRenderPipeline : public RenderPipeline
		{
			friend class VulkanDriver;
			friend class VulkanRenderStateManager;
		public: 
			VulkanRenderPipeline();
			virtual ~VulkanRenderPipeline();

			virtual void Load(const pugi::xml_node &node) override;
			virtual void Init(const VertexInputDeclarationPtr& vertexDeclaration) override;
			virtual void Unload() override;

			virtual const VertexInputDeclarationPtr& GetVertexInputDeclaration() const override { return m_description.vertexDeclaration; }

			VkPipeline GetVulkanPipeline(const VertexInputDeclarationPtr& vertexDeclaration);

			//VkPipeline GetVulkanId() const { return m_graphicsPipeline; }
			VkPipelineLayout GetVulkanPipelineLayout() const { return m_pipelineLayout; }

		private:
			VulkanRenderPipeline(const RenderPipelineDescription& description);

		protected:
			//VkPipeline m_pipelines;

			//VkPipeline m_graphicsPipeline;		
			VkPipelineLayout m_pipelineLayout;
			//VkDescriptorSetLayout m_descriptorSetLayout;

			// Pipelines for different vertex declarations
			std::vector<VulkanVertexDeclaration*> m_declarations;
			std::vector<VkPipeline> m_intenalPipelines;
		};
	}
}

#endif
