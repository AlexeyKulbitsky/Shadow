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

			virtual VertexBuffer* CreateVertexBuffer() override;
			virtual VertexBuffer* CreateVertexBuffer(const void* data, size_t size) override;
			virtual IndexBuffer* CreateIndexBuffer() override;
			virtual IndexBuffer* CreateIndexBuffer(const void* data, size_t size) override;
			virtual RenderCommand* CreateRenderCommand() override;

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
			void CreateDepthResources();
			void CreateCommandBuffers();
			void CreateSemaphores();
			void createVertexBuffer();
			void createIndexBuffer();
			void CreateUniformBuffer();
			void CreateDescriptorPool();
			void CreateDescriptorSetLayout();
			void CreateDescriptorSet();
			void CreateTextureImage();
			void CreateTextureImageView();
			void CreateTextureSampler();
			void LoadModel();

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
			void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VulkanDeleter<VkBuffer>& buffer, VulkanDeleter<VkDeviceMemory>& bufferMemory);
			void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
			void updateUniformBuffer();
			void createImage(
				uint32_t width, uint32_t height,
				VkFormat format,
				VkImageTiling tiling,
				VkImageUsageFlags usage,
				VkMemoryPropertyFlags properties,
				VulkanDeleter<VkImage>& image,
				VulkanDeleter<VkDeviceMemory>& imageMemory);

			VkCommandBuffer beginSingleTimeCommands();
			void endSingleTimeCommands(VkCommandBuffer commandBuffer);
			void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
			void copyImage(VkImage srcImage, VkImage dstImage, uint32_t width, uint32_t height);
			void createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, VulkanDeleter<VkImageView>& imageView);
			VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
			VkFormat findDepthFormat();
			bool hasStencilComponent(VkFormat format) 
			{
				return format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT;
			}

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
			VulkanDeleter<VkDescriptorSetLayout> m_descriptorSetLayout{ m_device, vkDestroyDescriptorSetLayout };
			VulkanDeleter<VkPipelineLayout> m_pipelineLayout{ m_device, vkDestroyPipelineLayout };
			VulkanDeleter<VkPipeline> m_graphicsPipeline{ m_device, vkDestroyPipeline };
			VulkanDeleter<VkCommandPool> m_commandPool{ m_device, vkDestroyCommandPool };

			VulkanDeleter<VkImage> m_textureImage{ m_device, vkDestroyImage };
			VulkanDeleter<VkDeviceMemory> m_textureImageMemory{ m_device, vkFreeMemory };

			VulkanDeleter<VkBuffer> m_vertexBuffer{ m_device, vkDestroyBuffer };
			VulkanDeleter<VkDeviceMemory> m_vertexBufferMemory{ m_device, vkFreeMemory };
			VulkanDeleter<VkBuffer> m_indexBuffer{ m_device, vkDestroyBuffer };
			VulkanDeleter<VkDeviceMemory> m_indexBufferMemory{ m_device, vkFreeMemory };

			VulkanDeleter<VkBuffer> m_uniformStagingBuffer{ m_device, vkDestroyBuffer };
			VulkanDeleter<VkDeviceMemory> m_uniformStagingBufferMemory{ m_device, vkFreeMemory };
			VulkanDeleter<VkBuffer> m_uniformBuffer{ m_device, vkDestroyBuffer };
			VulkanDeleter<VkDeviceMemory> m_uniformBufferMemory{ m_device, vkFreeMemory };

			std::vector<VkCommandBuffer> m_commandBuffers;
			VulkanDeleter<VkSemaphore> m_imageAvailableSemaphore{ m_device, vkDestroySemaphore };
			VulkanDeleter<VkSemaphore> m_renderFinishedSemaphore{ m_device, vkDestroySemaphore };

			VulkanDeleter<VkDescriptorPool> m_descriptorPool{ m_device, vkDestroyDescriptorPool };
			VkDescriptorSet m_descriptorSet;
			VulkanDeleter<VkImageView> m_textureImageView{ m_device, vkDestroyImageView };
			VulkanDeleter<VkSampler> m_textureSampler{ m_device, vkDestroySampler };

			VulkanDeleter<VkImage> m_depthImage{ m_device, vkDestroyImage };
			VulkanDeleter<VkDeviceMemory> m_depthImageMemory{ m_device, vkFreeMemory };
			VulkanDeleter<VkImageView> m_depthImageView{ m_device, vkDestroyImageView };


			std::vector<const char*> m_instanceLayersList;
			std::vector<const char*> m_instanceExtensionsList;
			std::vector<const char*> m_deviceLayersList;
			std::vector<const char*> m_deviceExtensionsList;
		};
	}
}

#endif
