#ifndef SHADOW_VULKAN_DRIVER_INCLUDE
#define SHADOW_VULKAN_DRIVER_INCLUDE

#include "../Driver.h"
#include "VulkanDeleter.h"
#include "../../CreationParameters.h"

namespace sh
{
	namespace video
	{
		VkResult CreateDebugReportCallbackEXT(VkInstance instance, const VkDebugReportCallbackCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugReportCallbackEXT* pCallback);
// 		{
// 			auto func = (PFN_vkCreateDebugReportCallbackEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugReportCallbackEXT");
// 			if (func != nullptr) 
// 			{
// 				return func(instance, pCreateInfo, pAllocator, pCallback);
// 			}
// 			else 
// 			{
// 				return VK_ERROR_EXTENSION_NOT_PRESENT;
// 			}
// 		}

		void DestroyDebugReportCallbackEXT(VkInstance instance, VkDebugReportCallbackEXT callback, const VkAllocationCallbacks* pAllocator);
// 		{
// 			auto func = (PFN_vkDestroyDebugReportCallbackEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugReportCallbackEXT");
// 			if (func != nullptr) 
// 			{
// 				func(instance, callback, pAllocator);
// 			}
// 		}


		class VulkanDriver : public Driver 
		{
		public:
			VulkanDriver();
			VulkanDriver(const CreationParameters& parameters);
			~VulkanDriver();
			virtual bool Init() override;
			virtual void BeginRendering() override;
			virtual void EndRendering() override;
			virtual void DrawMesh(scene::Mesh* mesh) override;
			virtual void DrawHardwareBuffer(HardwareBuffer *buffer) const override;
			virtual HardwareBuffer* CreateHardwareBuffer(const HardwareBuffer::CreateInfo& info) const override;
			virtual void InitHardwareBuffer(HardwareBuffer *buffer, const void* verticesPointer, u32 verticesSize, const void* indicesPointer, u32 indicesSize) const override;

		private:
			struct QueueFamilyIndices 
			{
				int graphicsFamily = -1;
				int presentFamily = -1;

				bool isComplete() 
				{
					return graphicsFamily >= 0 && presentFamily >= 0;
				}
			};

			struct SwapChainSupportDetails 
			{
				VkSurfaceCapabilitiesKHR capabilities;
				std::vector<VkSurfaceFormatKHR> formats;
				std::vector<VkPresentModeKHR> presentModes;
			};

			void SetupLayersAndExtensions();
			void CreateInstance();
			void PickPhysicalDevice();
			void CreateLogicalDevice();
			void CreateSurface();
			void CreateSwapChain();
			void CreateImageViews();
			void CreateGraphicsPipeline();
			void CreateRenderPass();
			void CreateFramebuffers();
			void CreateCommandPool();
			void CreateCommandBuffers();
			void CreateSemaphores();
			void CreateVertexBuffer();

			bool isDeviceSuitable(VkPhysicalDevice device);
			QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
			bool checkDeviceExtensionSupport(VkPhysicalDevice device);
			SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
			VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
			VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR> availablePresentModes);
			VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
			static std::vector<char> readFile(const std::string& filename);
			void createShaderModule(const std::vector<char>& code, VulkanDeleter<VkShaderModule>& shaderModule);
			uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

		private:
			CreationParameters m_parameters;
			VulkanDeleter<VkInstance> m_instance{ vkDestroyInstance };			
			VulkanDeleter<VkSurfaceKHR> m_surface{ m_instance, vkDestroySurfaceKHR };
			VkPhysicalDevice m_physicalDevice = VK_NULL_HANDLE;
			VulkanDeleter<VkDevice> m_device{ vkDestroyDevice };
			VkQueue m_graphicsQueue;
			VkQueue m_presentQueue;
			VulkanDeleter<VkSwapchainKHR> m_swapChain{ m_device, vkDestroySwapchainKHR };
			std::vector<VkImage> m_swapChainImages;
			VkFormat m_swapChainImageFormat;
			VkExtent2D m_swapChainExtent;
			std::vector<VulkanDeleter<VkImageView>> m_swapChainImageViews;
			std::vector<VulkanDeleter<VkFramebuffer>> m_swapChainFramebuffers;
			

			VulkanDeleter<VkRenderPass> m_renderPass{ m_device, vkDestroyRenderPass };
			VulkanDeleter<VkPipelineLayout> m_pipelineLayout{ m_device, vkDestroyPipelineLayout };
			VulkanDeleter<VkPipeline> m_graphicsPipeline{ m_device, vkDestroyPipeline };
			VulkanDeleter<VkCommandPool> m_commandPool{ m_device, vkDestroyCommandPool };
			VulkanDeleter<VkBuffer> m_vertexBuffer{ m_device, vkDestroyBuffer };
			VulkanDeleter<VkDeviceMemory> m_vertexBufferMemory{ m_device, vkFreeMemory };
			std::vector<VkCommandBuffer> m_commandBuffers;
			VulkanDeleter<VkSemaphore> m_imageAvailableSemaphore{ m_device, vkDestroySemaphore };
			VulkanDeleter<VkSemaphore> m_renderFinishedSemaphore{ m_device, vkDestroySemaphore };


			std::vector<const char*> m_instanceLayersList;
			std::vector<const char*> m_instanceExtensionsList;
			std::vector<const char*> m_deviceLayersList;
			std::vector<const char*> m_deviceExtensionsList;
		};
	}
}

#endif
