#include "VulkanRenderCommand.h"
#include "../../Device.h"
#include "VulkanDriver.h"
#include "VulkanVertexBuffer.h"
#include "VulkanIndexBuffer.h"
#include "VulkanUniformBuffer.h"

namespace sh
{
	namespace video
	{
		VulkanRenderCommand::VulkanRenderCommand()
		{

		}

		/////////////////////////////////////////////////////////////////////////

		VulkanRenderCommand::~VulkanRenderCommand()
		{

		}

		/////////////////////////////////////////////////////////////////////////

		void VulkanRenderCommand::Init()
		{
			VulkanDriver* driver = static_cast<VulkanDriver*>(Device::GetInstance()->GetDriver());
			VkDevice device = driver->GetVulkanDevice();
			VkRenderPass renderPass = driver->GetRenderPass();
			VkCommandPool commandPool = driver->GetCommandPool();
			const std::vector<VulkanDeleter<VkFramebuffer>>& framBuffers = driver->GetSwapChainFramebuffers();
			math::Vector4u vp = driver->GetViewPort();

			if (m_commandBuffers.size() > 0) 
			{
				vkFreeCommandBuffers(device, commandPool, m_commandBuffers.size(), m_commandBuffers.data());
			}
			m_commandBuffers.resize(framBuffers.size());

			VkCommandBufferAllocateInfo allocInfo = {};
			allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
			allocInfo.commandPool = commandPool;
			allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
			allocInfo.commandBufferCount = (uint32_t)m_commandBuffers.size();

			SH_ASSERT(vkAllocateCommandBuffers(device, &allocInfo, m_commandBuffers.data()) == VK_SUCCESS,
				"failed to allocate command buffers!");

			for (size_t i = 0; i < m_commandBuffers.size(); i++) 
			{
				VkCommandBufferBeginInfo beginInfo = {};
				beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
				beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;

				vkBeginCommandBuffer(m_commandBuffers[i], &beginInfo);

				VkRenderPassBeginInfo renderPassInfo = {};
				renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
				renderPassInfo.renderPass = renderPass;
				renderPassInfo.framebuffer = framBuffers[i];
				renderPassInfo.renderArea.offset = { 0, 0 };
				renderPassInfo.renderArea.extent.width = vp.z;
				renderPassInfo.renderArea.extent.height = vp.w;

				std::array<VkClearValue, 2> clearValues = {};
				clearValues[0].color.float32[0] = 0.7f;
				clearValues[0].color.float32[1] = 0.7f;
				clearValues[0].color.float32[2] = 0.7f;
				clearValues[0].color.float32[3] = 1.0f;
				clearValues[1].depthStencil.depth = 1.0f;
				clearValues[1].depthStencil.stencil = 0;

				renderPassInfo.clearValueCount = clearValues.size();
				renderPassInfo.pClearValues = clearValues.data();

				vkCmdBeginRenderPass(m_commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
				
				vkCmdBindPipeline(m_commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipeline->GetVulkanId());

				
				VkBuffer vertexBuffers[] = { m_vertexBuffer->GetVulkanId() };
				VkDeviceSize offsets[] = { 0 };
				vkCmdBindVertexBuffers(m_commandBuffers[i], 0, 1, vertexBuffers, offsets);

				vkCmdBindIndexBuffer(m_commandBuffers[i], m_indexBuffer->GetVulkanId(), 0, VK_INDEX_TYPE_UINT32);
				
				VulkanUniformBuffer* uniformBuffer = static_cast<VulkanUniformBuffer*>(m_pipeline->GetUniformBuffer().get());
				
				VkDescriptorSet descriptorSet = uniformBuffer->GetDescriptorSet();
				vkCmdBindDescriptorSets(m_commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, uniformBuffer->GetPipelineLayout(), 0, 1, &descriptorSet, 0, nullptr);
				
				vkCmdDrawIndexed(m_commandBuffers[i], m_indexBuffer->GetIndicesCount(), 1, 0, 0, 0);

				vkCmdEndRenderPass(m_commandBuffers[i]);

				VkResult res = vkEndCommandBuffer(m_commandBuffers[i]);
				SH_ASSERT(res == VK_SUCCESS, "failed to record command buffer!");			
			}
		}

		/////////////////////////////////////////////////////////////////////////

		void VulkanRenderCommand::SetUseIndices(bool useIndices)
		{
			m_useIndices = useIndices;
		}

		/////////////////////////////////////////////////////////////////////////

		void VulkanRenderCommand::SetVertexBuffer(VertexBuffer* vertexBuffer)
		{
			m_vertexBuffer = static_cast<VulkanVertexBuffer*>(vertexBuffer);
		}

		/////////////////////////////////////////////////////////////////////////

		void VulkanRenderCommand::SetIndexBuffer(IndexBuffer* indexBuffer)
		{
			m_indexBuffer = static_cast<VulkanIndexBuffer*>(indexBuffer);
		}

		/////////////////////////////////////////////////////////////////////////

		void VulkanRenderCommand::SetTopology(Topology topology)
		{
			m_topology = topology;
		}

		/////////////////////////////////////////////////////////////////////////

		void VulkanRenderCommand::SetVertexInputDeclaration(VertexInputDeclaration* declaration)
		{
			m_inputDeclaration = static_cast<VKVertexDeclaration*>(declaration);
		}

		/////////////////////////////////////////////////////////////////////////
	}
}