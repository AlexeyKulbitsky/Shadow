#ifndef SHADOW_VULKAN_RENDER_COMMAND_INCLUDE
#define SHADOW_VULKAN_RENDER_COMMAND_INCLUDE

#include "VulkanVertexDeclaration.h"
#include "VulkanVertexBuffer.h"
#include "VulkanIndexBuffer.h"
#include "VulkanRenderPipeline.h"

namespace sh
{
	namespace video
	{
		class VulkanRenderCommand
		{
		public:
			VulkanRenderCommand();
			virtual ~VulkanRenderCommand();

			virtual void Init();

			virtual void SetUseIndices(bool useIndices);
			virtual void SetVertexBuffer(VertexBuffer* vertexBuffer);
			virtual void SetIndexBuffer(IndexBuffer* indexBuffer);
			virtual void SetTopology(Topology topology);
			virtual void SetVertexInputDeclaration(VertexInputDeclaration* declaration);

			virtual bool IsUseIndices() const  { return m_useIndices; }
			virtual VertexBuffer* GetVertexBuffer()  { return m_vertexBuffer; }
			virtual IndexBuffer* GetIndexBuffer()  { return m_indexBuffer; }
			virtual Topology GetTopology() const  { return m_topology; }
			virtual VertexInputDeclaration* GetVertexInputDeclaration() const  { return m_inputDeclaration; }

			// Vulkan-specific interface
			void SetPipeline(VulkanRenderPipeline* pipeline) { m_pipeline = pipeline; }
			VkCommandBuffer GetCommandBuffer(uint32_t index) { return m_commandBuffers[index]; }
			void Update(const VkCommandBufferInheritanceInfo& inheritanceInfo);

		protected:
			bool m_useIndices;
			VulkanVertexBuffer* m_vertexBuffer;
			VulkanIndexBuffer* m_indexBuffer;
			VulkanVertexDeclaration* m_inputDeclaration;
			VulkanRenderPipeline* m_pipeline;
			Topology m_topology;
			u32 m_glTopology;

			std::vector<VkCommandBuffer> m_commandBuffers;
		};
	}
}

#endif