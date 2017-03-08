#ifndef SHADOW_VULKAN_DRIVER_INCLUDE
#define SHADOW_VULKAN_DRIVER_INCLUDE

#include "../Driver.h"
#include "VulkanDeleter.h"
#include "../../CreationParameters.h"

namespace sh
{
	namespace video
	{
		static VkPolygonMode const s_vkPolygoneMode[] =
		{
			VK_POLYGON_MODE_FILL,
			VK_POLYGON_MODE_LINE
		};
		
		static VkFrontFace const s_vkFrontFace[] =
		{
			VK_FRONT_FACE_CLOCKWISE,
			VK_FRONT_FACE_COUNTER_CLOCKWISE			
		};

		static VkCullModeFlagBits const s_vkCullFace[] =
		{		
			VK_CULL_MODE_FRONT_BIT,
			VK_CULL_MODE_BACK_BIT,
			VK_CULL_MODE_NONE
		};

		static VkCompareOp const s_vkCompareFunction[] =
		{
			VK_COMPARE_OP_LESS,
			VK_COMPARE_OP_LESS_OR_EQUAL,
			VK_COMPARE_OP_EQUAL,
			VK_COMPARE_OP_GREATER_OR_EQUAL,
			VK_COMPARE_OP_GREATER,
			VK_COMPARE_OP_NOT_EQUAL,
			VK_COMPARE_OP_ALWAYS,
			VK_COMPARE_OP_NEVER
		};

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
			virtual void Render(scene::Mesh* mesh) override;

			void DrawBuffer();

			virtual VertexBufferPtr CreateVertexBuffer(HardwareBuffer::Usage usage) const override;
			virtual IndexBufferPtr CreateIndexBuffer(HardwareBuffer::Usage usage) const override;
			virtual UniformBufferPtr CreateUniformBuffer() const override;
			virtual RenderCommandPtr CreateRenderCommand() const override;
			virtual ShaderProgramPtr CreateShaderProgram() const override;
			virtual VertexInputDeclaration* CreateVertexInputDeclaration() const override;
			virtual RenderPipelinePtr CreateRenderPipeline() const override;

			// Vulkan-specific interface
			VkDevice GetVulkanDevice() const { return m_device; }
			VkPhysicalDevice GetVulkanPhysicalDevice() const { return m_physicalDevice; }
			VkDescriptorPool GetDescriptorPool() const { return m_descriptorPool; }
			VkFormat GetSwapChainImageFormat() const { return m_swapChainImageFormat; }
			VkCommandPool GetCommandPool() const { return m_commandPool; }
			const std::vector<VulkanDeleter<VkFramebuffer>>& GetSwapChainFramebuffers() const { return m_swapChainFramebuffers; }
		
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
			void createUniformBuffer();
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
		public:
			uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
		private:	
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

			// Working command for immediate execution (buffer creation)
		public:
			VkCommandBuffer beginSingleTimeCommands();
			void endSingleTimeCommands(VkCommandBuffer commandBuffer);

		private:
			void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
			void copyImage(VkImage srcImage, VkImage dstImage, uint32_t width, uint32_t height);
			void createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, VulkanDeleter<VkImageView>& imageView);
			VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
		public:
			VkFormat findDepthFormat();
		private:
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
