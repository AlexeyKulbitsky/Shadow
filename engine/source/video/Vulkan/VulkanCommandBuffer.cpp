#include "VulkanCommandBuffer.h"
#include "../../Device.h"
#include "VulkanDriver.h"

namespace sh
{

namespace video
{

	VulkanCommandBuffer::~VulkanCommandBuffer()
	{
		
	}

	////////////////////////////////////////////////////////////////////

	void VulkanCommandBuffer::Begin()
	{
		Driver* driver = Device::GetInstance()->GetDriver();
		VulkanDriver* vulkanDriver = static_cast<VulkanDriver*>(driver);
		const VkCommandBufferInheritanceInfo& inheritanceInfo = vulkanDriver->GetInheritanceInfo();


		VkCommandBufferBeginInfo beginInfo;
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.pInheritanceInfo = &inheritanceInfo;
		beginInfo.flags = VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT | VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
		beginInfo.pNext = nullptr;

		VkResult res = vkBeginCommandBuffer(m_commandBuffer, &beginInfo);
		SH_ASSERT(res == VK_SUCCESS, "Can not begin Vulkan command buffer!");
	}

	////////////////////////////////////////////////////////////////////

	void VulkanCommandBuffer::End()
	{
		VkResult res = vkEndCommandBuffer(m_commandBuffer);
		SH_ASSERT(res == VK_SUCCESS, "Can not end Vulkan command buffer!");
	}

	////////////////////////////////////////////////////////////////////

	void VulkanCommandBuffer::BeginRenderPass()
	{

	}

	////////////////////////////////////////////////////////////////////

	void VulkanCommandBuffer::EndRenderPass()
	{

	}

	////////////////////////////////////////////////////////////////////

	void VulkanCommandBuffer::Append(VulkanCommandBuffer* secondary)
	{
		m_secondaryCommandBuffers.push_back(secondary->GetVulkanId());
	}

	////////////////////////////////////////////////////////////////////

	void VulkanCommandBuffer::Execute()
	{
		if (m_secondaryCommandBuffers.size() > 0)
		{
			vkCmdExecuteCommands(m_commandBuffer, m_secondaryCommandBuffers.size(), m_secondaryCommandBuffers.data());
		}
	}

	////////////////////////////////////////////////////////////////////

	VulkanCommandBuffer::VulkanCommandBuffer(const CommandBufferDescription& description)
	{
		VulkanDriver* driver = static_cast<VulkanDriver*>(Device::GetInstance()->GetDriver());
		VkDevice device = driver->GetVulkanDevice();
		VkCommandPool commandPool = driver->GetCommandPool();

		// Allocate command buffer
		VkCommandBufferAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.commandPool = commandPool;
		allocInfo.level = s_vkCommandBufferType[description.type];
		allocInfo.commandBufferCount = 1;
		allocInfo.pNext = nullptr;

		VkResult res = vkAllocateCommandBuffers(device, &allocInfo, &m_commandBuffer);
		SH_ASSERT(res == VK_SUCCESS, "Failed to allocate command buffer!");

		m_secondaryCommandBuffers.reserve(1000U);
	}

} // video

} // sh