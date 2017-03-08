#ifndef SHADOW_VULKAN_RENDER_COMMAND_INCLUDE
#define SHADOW_VULKAN_RENDER_COMMAND_INCLUDE

#include "../RenderCommand.h"
#include "VulkanVertexDeclaration.h"
#include "VulkanVertexBuffer.h"
#include "VulkanIndexBuffer.h"
#include "VulkanRenderPipeline.h"

namespace sh
{
	namespace video
	{
		class VulkanRenderCommand : public RenderCommand
		{
		public:
			VulkanRenderCommand();
			virtual ~VulkanRenderCommand();

			virtual void Init() override;

			virtual void SetUseIndices(bool useIndices) override;
			virtual void SetVertexBuffer(VertexBuffer* vertexBuffer) override;
			virtual void SetIndexBuffer(IndexBuffer* indexBuffer) override;
			virtual void SetTopology(Topology topology) override;
			virtual void SetVertexInputDeclaration(VertexInputDeclaration* declaration) override;

			virtual bool IsUseIndices() const override { return m_useIndices; }
			virtual VertexBuffer* GetVertexBuffer() override { return m_vertexBuffer; }
			virtual IndexBuffer* GetIndexBuffer() override { return m_indexBuffer; }
			virtual Topology GetTopology() const override { return m_topology; }
			virtual VertexInputDeclaration* GetVertexInputDeclaration() const override { return m_inputDeclaration; }
			const UniformsBatchPtr& GetAutoUniformsBatch() const { return m_autoUniformsBatch; }

			void SetPipeline(VulkanRenderPipeline* pipeline) { m_pipeline = pipeline; }
		protected:
			bool m_useIndices;
			VulkanVertexBuffer* m_vertexBuffer;
			VulkanIndexBuffer* m_indexBuffer;
			VKVertexDeclaration* m_inputDeclaration;
			VulkanRenderPipeline* m_pipeline;
			Topology m_topology;
			u32 m_glTopology;

			std::vector<VkCommandBuffer> m_commandBuffers;
		};
	}
}

#endif