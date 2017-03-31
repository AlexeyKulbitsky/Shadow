#ifndef SHADOW_VULKAN_RENDER_PIPELINE_INCLUDE
#define SHADOW_VULKAN_RENDER_PIPELINE_INCLUDE

#include "../RenderPipeline.h"
#include "VulkanDeleter.h"

namespace sh
{
	namespace video
	{
		class VulkanRenderPipeline : public RenderPipeline
		{
		public: 
			VulkanRenderPipeline();
			virtual ~VulkanRenderPipeline();

			virtual RenderPipelinePtr Clone() override;

			virtual void Load(const pugi::xml_node &node) override;
			virtual void Unload() override;

			virtual const ShaderProgramPtr& GetShaderProgram() const override { return m_shaderProgram; }
			virtual const VertexInputDeclarationPtr& GetVertexInputDeclaration() const override { return m_vertexInputDeclaration; }
			virtual const UniformBufferPtr& GetUniformBuffer() const override { return m_uniformBuffer; }
			virtual const UniformBufferPtr& GetTransformUniformBuffer() const override { return m_transformUniformBuffer; }

			void Init();

			VkPipeline GetVulkanId() const { return m_graphicsPipeline; }
			VkPipelineLayout GetVulkanPipelineLayout() const { return m_pipelineLayout; }

		private:

		protected:
			ShaderProgramPtr m_shaderProgram;
			VertexInputDeclarationPtr m_vertexInputDeclaration;
			UniformBufferPtr m_uniformBuffer;
			UniformBufferPtr m_transformUniformBuffer;

			VkPipeline m_graphicsPipeline;		
			VkPipelineLayout m_pipelineLayout;
		};
	}
}

#endif
