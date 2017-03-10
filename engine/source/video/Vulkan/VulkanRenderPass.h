#ifndef SHADOW_VULKAN_RENDER_PASS_INCLUDE
#define SHADOW_VULKAN_RENDER_PASS_INCLUDE

#include "../RenderPass.h"
#include "VulkanDeleter.h"

namespace sh
{
	namespace video
	{
		class VulkanRenderPass : public RenderPass
		{
		public:
			VulkanRenderPass();
			virtual ~VulkanRenderPass();

			void Init();

			VkRenderPass GetVulkanId() const { return m_renderPass; }

		private:
			VkRenderPass m_renderPass;
		};
	}
}

#endif