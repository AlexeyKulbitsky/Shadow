#ifndef SHADOW_VULKAN_RENDER_PIPELINE_INCLUDE
#define SHADOW_VULKAN_RENDER_PIPELINE_INCLUDE

#include "../RenderPipeline.h"
#include "VulkanCommon.h"

namespace sh
{
	namespace video
	{
		class VulkanRenderPipeline : public RenderPipeline
		{
			friend class VulkanDriver;
			friend class VulkanRenderStateManager;
		public: 
			VulkanRenderPipeline();
			virtual ~VulkanRenderPipeline();

			virtual void Load(const pugi::xml_node &node) override;
			virtual void Init() override;
			virtual void Unload() override;

			virtual const ShaderProgramPtr& GetShaderProgram() const override { return nullptr; } // { return m_shaderProgram; }
			virtual const VertexInputDeclarationPtr& GetVertexInputDeclaration() const override { return m_description.vertexDeclaration; }
			virtual const UniformBufferPtr& GetUniformBuffer() const override { return nullptr; } // { return m_uniformBuffer; }
			virtual const UniformBufferPtr& GetTransformUniformBuffer() const override { return nullptr; } // { return m_transformUniformBuffer; }

			VkPipeline GetVulkanPipeline(const VertexInputDeclarationPtr& vertexDeclaration);

			VkPipeline GetVulkanId() const { return m_graphicsPipeline; }
			VkPipelineLayout GetVulkanPipelineLayout() const { return m_pipelineLayout; }

		private:
			VulkanRenderPipeline(const RenderPipelineDescription& description);

		protected:
			VkPipeline m_pipelines;

			VkPipeline m_graphicsPipeline;		
			VkPipelineLayout m_pipelineLayout;

			// Pipelines for different vertex declarations
		};
	}
}

#endif
