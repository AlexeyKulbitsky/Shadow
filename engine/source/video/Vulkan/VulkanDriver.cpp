#include "VulkanDriver.h"
#include "../../scene/Mesh.h"

#include "VulkanVertexBuffer.h"
#include "VulkanIndexBuffer.h"
#include "VulkanRenderPipeline.h"
#include "VulkanShaderProgram.h"
#include "VulkanUniformBuffer.h"
#include "VulkanRenderCommand.h"

#include <iostream>
#include <stdexcept>
#include <functional>
#include <fstream>
#include <algorithm>
#include <vector>
#include <cstring>
#include <set>

#include <stb_image.h>

namespace sh
{
	namespace video
	{

		std::vector<uint32_t> indices;

		VkResult CreateDebugReportCallbackEXT(VkInstance instance, const VkDebugReportCallbackCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugReportCallbackEXT* pCallback)
		{
			auto func = (PFN_vkCreateDebugReportCallbackEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugReportCallbackEXT");
			if (func != nullptr)
			{
				return func(instance, pCreateInfo, pAllocator, pCallback);
			}
			else
			{
				return VK_ERROR_EXTENSION_NOT_PRESENT;
			}
		}

		void DestroyDebugReportCallbackEXT(VkInstance instance, VkDebugReportCallbackEXT callback, const VkAllocationCallbacks* pAllocator)
		{
			auto func = (PFN_vkDestroyDebugReportCallbackEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugReportCallbackEXT");
			if (func != nullptr)
			{
				func(instance, callback, pAllocator);
			}
		}

		static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objType, uint64_t obj, size_t location, int32_t code, const char* layerPrefix, const char* msg, void* userData) 
		{
			std::cerr << "validation layer: " << msg << std::endl;

			return VK_FALSE;
		}

		VulkanDriver::VulkanDriver()
		{

		}

		VulkanDriver::VulkanDriver(const CreationParameters& parameters)
		{
			m_parameters = parameters;
		}

		VulkanDriver::~VulkanDriver()
		{
			vkDeviceWaitIdle(m_device);
		}

		bool VulkanDriver::Init()
		{
			InitGlobalUniforms();

			SetupLayersAndExtensions();			
			CreateInstance();		
			CreateSurface();			
			PickPhysicalDevice();		
			CreateLogicalDevice();		
			CreateSwapChain();		
			CreateImageViews();
			CreateRenderPass();
			CreateCommandPool();
			CreateDepthResources();
			CreateFramebuffers();
			CreateSemaphores();

			/*			
			CreateDepthResources();
			CreateFramebuffers();
			CreateTextureImage();
			CreateTextureImageView();
			CreateTextureSampler();			
			*/
			return true;
		}

		void VulkanDriver::BeginRendering()
		{
			
		}
		void VulkanDriver::EndRendering()
		{

		}

		////////////////////////////////////////////////////////////////////////////////////////////////////////////

		void VulkanDriver::Render(scene::Mesh* mesh)
		{
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////////////

		void VulkanDriver::Render(const RenderCommandPtr& command)
		{
			VulkanRenderCommand* vulkanRenderCommand = static_cast<VulkanRenderCommand*>(command.get());
			
			vkAcquireNextImageKHR(m_device, m_swapChain, 500000/*std::numeric_limits<uint64_t>::max()*/, m_imageAvailableSemaphore, VK_NULL_HANDLE, &m_currentImageIndex);
			
			VkCommandBuffer commandBuffer = vulkanRenderCommand->GetCommandBuffer(m_currentImageIndex);
		
			VkSubmitInfo submitInfo = {};
			submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

			VkSemaphore waitSemaphores[] = { m_imageAvailableSemaphore };
			VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
			submitInfo.waitSemaphoreCount = 1;
			submitInfo.pWaitSemaphores = waitSemaphores;
			submitInfo.pWaitDstStageMask = waitStages;

			submitInfo.commandBufferCount = 1;
			submitInfo.pCommandBuffers = &commandBuffer;

			VkSemaphore signalSemaphores[] = { m_renderFinishedSemaphore };
			submitInfo.signalSemaphoreCount = 1;
			submitInfo.pSignalSemaphores = signalSemaphores;

			VkResult res = vkQueueSubmit(m_graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
			SH_ASSERT(res == VK_SUCCESS, "failed to submit draw command buffer!");


			VkPresentInfoKHR presentInfo = {};
			presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

			presentInfo.waitSemaphoreCount = 1;
			presentInfo.pWaitSemaphores = signalSemaphores;

			VkSwapchainKHR swapChains[] = { m_swapChain };
			presentInfo.swapchainCount = 1;
			presentInfo.pSwapchains = swapChains;

			presentInfo.pImageIndices = &m_currentImageIndex;

			vkQueuePresentKHR(m_presentQueue, &presentInfo);
		
		}
		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////

		VertexBufferPtr VulkanDriver::CreateVertexBuffer(HardwareBuffer::Usage usage) const
		{
			VertexBufferPtr result = nullptr;
			result.reset(new VulkanVertexBuffer(usage));
			return result;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////////////

		IndexBufferPtr VulkanDriver::CreateIndexBuffer(HardwareBuffer::Usage usage) const
		{
			IndexBufferPtr result = nullptr;
			result.reset(new VulkanIndexBuffer(usage));
			return result;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////////////

		UniformBufferPtr VulkanDriver::CreateUniformBuffer() const
		{
			UniformBufferPtr result = nullptr;
			result.reset(new VulkanUniformBuffer());
			return result;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////////////

		RenderCommandPtr VulkanDriver::CreateRenderCommand() const
		{
			RenderCommandPtr result = nullptr;
			result.reset(new VulkanRenderCommand());
			return result;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////////////

		ShaderProgramPtr VulkanDriver::CreateShaderProgram() const
		{
			ShaderProgramPtr result = nullptr;
			result.reset(new VulkanShaderProgram(m_device));
			return result;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////////////

		VertexInputDeclaration* VulkanDriver::CreateVertexInputDeclaration() const
		{
			return new VulkanVertexDeclaration();
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////////////

		RenderPipelinePtr VulkanDriver::CreateRenderPipeline() const
		{
			RenderPipelinePtr result = nullptr;
			result.reset(new VulkanRenderPipeline());
			return result;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////////////

		void VulkanDriver::SetupLayersAndExtensions()
		{
			uint32_t extensionCount = 0;
			vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
			std::cout << "Number of extesions " << extensionCount << std::endl;

			std::vector<VkExtensionProperties> extensions(extensionCount);
			vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());
			std::cout << "available extensions:" << std::endl;

			for (const auto& extension : extensions)
			{
				std::cout << "\t" << extension.extensionName << std::endl;
			}
			// Needed for surface to draw on
			m_instanceExtensionsList.push_back(VK_KHR_SURFACE_EXTENSION_NAME);
			// Needed for platform-specific surface creation
			m_instanceExtensionsList.push_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);
			// Neede for swapping buffer and picture presentation
			m_deviceExtensionsList.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);

			uint32_t layerCount;
			vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

			std::vector<VkLayerProperties> availableLayers(layerCount);
			vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

			std::cout << "available validation layers:" << std::endl;
			for (const auto& layer : availableLayers)
			{
				std::cout << "\t" << layer.layerName << std::endl;
			}
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////////////

		// Main entry to Vulkan API
		void VulkanDriver::CreateInstance()
		{
			VkApplicationInfo appInfo = {};
			appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
			appInfo.pApplicationName = "Test application";
			appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
			appInfo.pEngineName = "Shadow Engine";
			appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
			appInfo.apiVersion = VK_API_VERSION_1_0;

			VkInstanceCreateInfo createInfo = {};
			createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
			createInfo.pApplicationInfo = &appInfo;
			createInfo.enabledExtensionCount = m_instanceExtensionsList.size();
			createInfo.ppEnabledExtensionNames = m_instanceExtensionsList.data();

			SH_ASSERT(vkCreateInstance(&createInfo, nullptr, m_instance.Replace()) == VK_SUCCESS, "failed to create instance!");		
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////////////

		void VulkanDriver::PickPhysicalDevice()
		{
			uint32_t deviceCount = 0;
			vkEnumeratePhysicalDevices(m_instance, &deviceCount, nullptr);

			std::vector<VkPhysicalDevice> devices(deviceCount);
			vkEnumeratePhysicalDevices(m_instance, &deviceCount, devices.data());

			std::cout << "available physical devices:" << std::endl;
			for (auto device : devices)
			{
				if (isDeviceSuitable(device)) 
				{
					m_physicalDevice = device;
					break;
				}
			}

			SH_ASSERT(m_physicalDevice != VK_NULL_HANDLE, "failed to find a suitable GPU!");

			VkPhysicalDeviceProperties deviceProperties;
			vkGetPhysicalDeviceProperties(m_physicalDevice, &deviceProperties);
			std::cout << "Physical device properties:\n";
			std::cout << "\t API version: " << deviceProperties.apiVersion << std::endl;
			std::cout << "\t Device ID: " << deviceProperties.deviceID << std::endl;
			std::cout << "\t Device name: " << deviceProperties.deviceName << std::endl;
			std::cout << "\t Device type: " << (deviceProperties.deviceType) << std::endl;
			std::cout << "\t Driver version: " << deviceProperties.driverVersion << std::endl;
			std::cout << "\t Vendor ID: " << deviceProperties.vendorID << std::endl;
		}
		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////

		void VulkanDriver::CreateLogicalDevice()
		{
			QueueFamilyIndices indices = findQueueFamilies(m_physicalDevice);

			std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
			std::set<int> uniqueQueueFamilies = { indices.graphicsFamily, indices.presentFamily };

			float queuePriority = 1.0f;
			for (int queueFamily : uniqueQueueFamilies) 
			{
				VkDeviceQueueCreateInfo queueCreateInfo = {};
				queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
				queueCreateInfo.queueFamilyIndex = queueFamily;
				queueCreateInfo.queueCount = 1;
				queueCreateInfo.pQueuePriorities = &queuePriority;
				queueCreateInfos.push_back(queueCreateInfo);
			}

			VkPhysicalDeviceFeatures deviceFeatures = {};

			VkDeviceCreateInfo createInfo = {};
			createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

			createInfo.pQueueCreateInfos = queueCreateInfos.data();
			createInfo.queueCreateInfoCount = (uint32_t)queueCreateInfos.size();

			createInfo.pEnabledFeatures = &deviceFeatures;

			createInfo.enabledExtensionCount = m_deviceExtensionsList.size();
			createInfo.ppEnabledExtensionNames = m_deviceExtensionsList.data();

			SH_ASSERT(vkCreateDevice(m_physicalDevice, &createInfo, nullptr, m_device.Replace()) == VK_SUCCESS,
				"failed to create logical device!");

			vkGetDeviceQueue(m_device, indices.graphicsFamily, 0, &m_graphicsQueue);
			vkGetDeviceQueue(m_device, indices.presentFamily, 0, &m_presentQueue);
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////////////

		void VulkanDriver::CreateSurface()
		{
#if defined SHADOW_WINDOWS
			VkWin32SurfaceCreateInfoKHR createInfo{};
			createInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
			createInfo.hinstance = GetModuleHandle(nullptr);
			createInfo.hwnd = static_cast<HWND>(m_parameters.WinId);
			SH_ASSERT(vkCreateWin32SurfaceKHR(m_instance, &createInfo, nullptr, m_surface.Replace()) == VK_SUCCESS,
				"failed to create window surface!");
#else
			SH_ASSERT(0, "Unimplemented for non-Windows platform");
#endif
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////////////

		void VulkanDriver::CreateSwapChain()
		{
			SwapChainSupportDetails swapChainSupport = querySwapChainSupport(m_physicalDevice);

			VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
			VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
			VkExtent2D extent = chooseSwapExtent(swapChainSupport.capabilities);

			uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
			if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount) 
			{
				imageCount = swapChainSupport.capabilities.maxImageCount;
			}

			VkSwapchainCreateInfoKHR createInfo = {};
			createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
			createInfo.surface = m_surface;

			createInfo.minImageCount = imageCount;
			createInfo.imageFormat = surfaceFormat.format;
			createInfo.imageColorSpace = surfaceFormat.colorSpace;
			createInfo.imageExtent = extent;
			createInfo.imageArrayLayers = 1;
			createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

			QueueFamilyIndices indices = findQueueFamilies(m_physicalDevice);
			uint32_t queueFamilyIndices[] = { (uint32_t)indices.graphicsFamily, (uint32_t)indices.presentFamily };

			if (indices.graphicsFamily != indices.presentFamily) 
			{
				createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
				createInfo.queueFamilyIndexCount = 2;
				createInfo.pQueueFamilyIndices = queueFamilyIndices;
			}
			else 
			{
				createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
			}

			createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
			createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
			createInfo.presentMode = presentMode;
			createInfo.clipped = VK_TRUE;

			createInfo.oldSwapchain = VK_NULL_HANDLE;

			SH_ASSERT(vkCreateSwapchainKHR(m_device, &createInfo, nullptr, m_swapChain.Replace()) == VK_SUCCESS,
				"failed to create swap chain!");
			

			vkGetSwapchainImagesKHR(m_device, m_swapChain, &imageCount, nullptr);
			m_swapChainImages.resize(imageCount);
			vkGetSwapchainImagesKHR(m_device, m_swapChain, &imageCount, m_swapChainImages.data());

			m_swapChainImageFormat = surfaceFormat.format;
			m_swapChainExtent = extent;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////////////

		void VulkanDriver::CreateImageViews()
		{
			m_swapChainImageViews.resize(m_swapChainImages.size(), VulkanDeleter<VkImageView>{m_device, vkDestroyImageView});
		
			for (uint32_t i = 0; i < m_swapChainImages.size(); i++) {
				createImageView(m_swapChainImages[i], m_swapChainImageFormat, VK_IMAGE_ASPECT_COLOR_BIT, m_swapChainImageViews[i]);
			}
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////////////

		void VulkanDriver::CreateFramebuffers()
		{
			m_swapChainFramebuffers.resize(m_swapChainImageViews.size(), VulkanDeleter<VkFramebuffer>{m_device, vkDestroyFramebuffer});

			for (size_t i = 0; i < m_swapChainImageViews.size(); i++) 
			{
				std::array<VkImageView, 2> attachments = 
				{
					m_swapChainImageViews[i],
					m_depthImageView
				};

				VkFramebufferCreateInfo framebufferInfo = {};
				framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
				framebufferInfo.renderPass = m_renderPass;
				framebufferInfo.attachmentCount = attachments.size();
				framebufferInfo.pAttachments = attachments.data();
				framebufferInfo.width = m_swapChainExtent.width;
				framebufferInfo.height = m_swapChainExtent.height;
				framebufferInfo.layers = 1;

				SH_ASSERT(vkCreateFramebuffer(m_device, &framebufferInfo, nullptr, m_swapChainFramebuffers[i].Replace()) == VK_SUCCESS,
					"failed to create framebuffer!");
			}
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////////////

		void VulkanDriver::CreateCommandPool()
		{
			QueueFamilyIndices queueFamilyIndices = findQueueFamilies(m_physicalDevice);

			VkCommandPoolCreateInfo poolInfo = {};
			poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
			poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily;
			//poolInfo.flags = 0; // Optional

			SH_ASSERT(vkCreateCommandPool(m_device, &poolInfo, nullptr, m_commandPool.Replace()) == VK_SUCCESS,
				"failed to create command pool!");
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////////////

		void VulkanDriver::CreateDepthResources()
		{
			VkFormat depthFormat = findDepthFormat();

			createImage(m_swapChainExtent.width, m_swapChainExtent.height, depthFormat, 
				VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, 
				m_depthImage, m_depthImageMemory);
			createImageView(m_depthImage, depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT, m_depthImageView);

			transitionImageLayout(m_depthImage, depthFormat, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////////////

		void VulkanDriver::CreateTextureImage()
		{
			int texWidth, texHeight, texChannels;
			stbi_uc* pixels = stbi_load("", &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
			SH_ASSERT(pixels, "failed to load texture image!");

			VkDeviceSize imageSize = texWidth * texHeight * 4;

			VulkanDeleter<VkImage> stagingImage{ m_device, vkDestroyImage };
			VulkanDeleter<VkDeviceMemory> stagingImageMemory{ m_device, vkFreeMemory };

			createImage(texWidth, texHeight, VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_TILING_LINEAR, VK_IMAGE_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingImage, stagingImageMemory);

			void* data;
			vkMapMemory(m_device, stagingImageMemory, 0, imageSize, 0, &data);
			memcpy(data, pixels, (size_t)imageSize);
			vkUnmapMemory(m_device, stagingImageMemory);

			stbi_image_free(pixels);

			createImage(texWidth, texHeight, VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_textureImage, m_textureImageMemory);

			transitionImageLayout(stagingImage, VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_LAYOUT_PREINITIALIZED, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL);
			transitionImageLayout(m_textureImage, VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_LAYOUT_PREINITIALIZED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
			copyImage(stagingImage, m_textureImage, texWidth, texHeight);

			transitionImageLayout(m_textureImage, VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////////////

		void VulkanDriver::CreateTextureImageView()
		{
			createImageView(m_textureImage, VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_ASPECT_COLOR_BIT, m_textureImageView);
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////////////

		void VulkanDriver::CreateTextureSampler()
		{
			VkSamplerCreateInfo samplerInfo = {};
			samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
			samplerInfo.magFilter = VK_FILTER_LINEAR;
			samplerInfo.minFilter = VK_FILTER_LINEAR;
			samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
			samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
			samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
			samplerInfo.anisotropyEnable = VK_TRUE;
			samplerInfo.maxAnisotropy = 16;
			samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
			samplerInfo.unnormalizedCoordinates = VK_FALSE;
			samplerInfo.compareEnable = VK_FALSE;
			samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
			samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;

			SH_ASSERT(vkCreateSampler(m_device, &samplerInfo, nullptr, m_textureSampler.Replace()) == VK_SUCCESS,
				"failed to create texture sampler!");
		
		}
		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////

		void VulkanDriver::CreateSemaphores()
		{
			VkSemaphoreCreateInfo semaphoreInfo = {};
			semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

			SH_ASSERT(vkCreateSemaphore(m_device, &semaphoreInfo, nullptr, m_imageAvailableSemaphore.Replace()) == VK_SUCCESS,
				"failed to create semaphores!");
			SH_ASSERT(vkCreateSemaphore(m_device, &semaphoreInfo, nullptr, m_renderFinishedSemaphore.Replace()) == VK_SUCCESS,
				"failed to create semaphores!");			
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////////////

		void VulkanDriver::CreateDescriptorPool()
		{
			std::array<VkDescriptorPoolSize, 2> poolSizes = {};
			poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			poolSizes[0].descriptorCount = 1;
			poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
			poolSizes[1].descriptorCount = 1;

			VkDescriptorPoolCreateInfo poolInfo = {};
			poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
			poolInfo.poolSizeCount = poolSizes.size();
			poolInfo.pPoolSizes = poolSizes.data();
			poolInfo.maxSets = 1;

			SH_ASSERT(vkCreateDescriptorPool(m_device, &poolInfo, nullptr, m_descriptorPool.Replace()) == VK_SUCCESS,
				"failed to create descriptor pool!");			
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////////////

		
		void VulkanDriver::CreateRenderPass()
		{
			VkAttachmentDescription colorAttachment = {};
			colorAttachment.format = m_swapChainImageFormat;
			colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
			colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
			colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
			colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
			colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
			colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
			colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

			VkAttachmentDescription depthAttachment = {};
			depthAttachment.format = findDepthFormat();
			depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
			depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
			depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
			depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
			depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
			depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
			depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

			VkAttachmentReference colorAttachmentRef = {};
			colorAttachmentRef.attachment = 0;
			colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

			VkAttachmentReference depthAttachmentRef = {};
			depthAttachmentRef.attachment = 1;
			depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

			VkSubpassDescription subPass = {};
			subPass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
			subPass.colorAttachmentCount = 1;
			subPass.pColorAttachments = &colorAttachmentRef;
			subPass.pDepthStencilAttachment = &depthAttachmentRef;

			VkSubpassDependency dependency = {};
			dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
			dependency.dstSubpass = 0;
			dependency.srcStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
			dependency.srcAccessMask = VK_ACCESS_MEMORY_READ_BIT;
			dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
			dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

			std::array<VkAttachmentDescription, 2> attachments = { colorAttachment, depthAttachment };
			VkRenderPassCreateInfo renderPassInfo = {};
			renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
			renderPassInfo.attachmentCount = attachments.size();
			renderPassInfo.pAttachments = attachments.data();
			renderPassInfo.subpassCount = 1;
			renderPassInfo.pSubpasses = &subPass;
			renderPassInfo.dependencyCount = 1;
			renderPassInfo.pDependencies = &dependency;

			SH_ASSERT(vkCreateRenderPass(m_device, &renderPassInfo, nullptr, m_renderPass.Replace()) == VK_SUCCESS,
				"failed to create render pass!");
			
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool VulkanDriver::isDeviceSuitable(VkPhysicalDevice device)
		{
			QueueFamilyIndices indices = findQueueFamilies(device);

			bool extensionsSupported = checkDeviceExtensionSupport(device);

			bool swapChainAdequate = false;
			if (extensionsSupported) 
			{
				SwapChainSupportDetails swapChainSupport = querySwapChainSupport(device);
				swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
			}

			return indices.isComplete() && extensionsSupported && swapChainAdequate;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////////////

		VulkanDriver::QueueFamilyIndices VulkanDriver::findQueueFamilies(VkPhysicalDevice device)
		{
			QueueFamilyIndices indices;

			uint32_t queueFamilyCount = 0;
			vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

			std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
			vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

			int i = 0;
			for (const auto& queueFamily : queueFamilies) 
			{
				if (queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) 
				{
					indices.graphicsFamily = i;
				}

				VkBool32 presentSupport = false;
				vkGetPhysicalDeviceSurfaceSupportKHR(device, i, m_surface, &presentSupport);

				if (queueFamily.queueCount > 0 && presentSupport) 
				{
					indices.presentFamily = i;
				}

				if (indices.isComplete()) 
				{
					break;
				}

				i++;
			}

			return indices;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////////////
		
		bool VulkanDriver::checkDeviceExtensionSupport(VkPhysicalDevice device)
		{
			uint32_t extensionCount;
			vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

			std::vector<VkExtensionProperties> availableExtensions(extensionCount);
			vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

			std::set<std::string> requiredExtensions(m_deviceExtensionsList.begin(), m_deviceExtensionsList.end());

			for (const auto& extension : availableExtensions) 
			{
				requiredExtensions.erase(extension.extensionName);
			}

			return requiredExtensions.empty();
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////////////

		VulkanDriver::SwapChainSupportDetails VulkanDriver::querySwapChainSupport(VkPhysicalDevice device)
		{
			SwapChainSupportDetails details;

			vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, m_surface, &details.capabilities);

			uint32_t formatCount;
			vkGetPhysicalDeviceSurfaceFormatsKHR(device, m_surface, &formatCount, nullptr);
			if (formatCount != 0) 
			{
				details.formats.resize(formatCount);
				vkGetPhysicalDeviceSurfaceFormatsKHR(device, m_surface, &formatCount, details.formats.data());
			}

			uint32_t presentModeCount;
			vkGetPhysicalDeviceSurfacePresentModesKHR(device, m_surface, &presentModeCount, nullptr);
			if (presentModeCount != 0) 
			{
				details.presentModes.resize(presentModeCount);
				vkGetPhysicalDeviceSurfacePresentModesKHR(device, m_surface, &presentModeCount, details.presentModes.data());
			}

			return details;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////////////

		VkSurfaceFormatKHR VulkanDriver::chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats)
		{
			if (availableFormats.size() == 1 && availableFormats[0].format == VK_FORMAT_UNDEFINED) 
			{
				return{ VK_FORMAT_B8G8R8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR };
			}

			for (const auto& availableFormat : availableFormats) 
			{
				if (availableFormat.format == VK_FORMAT_B8G8R8A8_UNORM && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) 
				{
					return availableFormat;
				}
			}

			return availableFormats[0];
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////////////

		VkPresentModeKHR VulkanDriver::chooseSwapPresentMode(const std::vector<VkPresentModeKHR> availablePresentModes)
		{
			for (const auto& availablePresentMode : availablePresentModes) 
			{
				if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) 
				{
					return availablePresentMode;
				}
			}
			return VK_PRESENT_MODE_FIFO_KHR;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////////////

		VkExtent2D VulkanDriver::chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities)
		{
			if (capabilities.currentExtent.width != 5000/*std::numeric_limits<uint32_t>::max()*/) 
			{
				return capabilities.currentExtent;
			}
			else 
			{
				VkExtent2D actualExtent = { 800, 600 };
				
				//actualExtent.width = std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent.width, actualExtent.width));
				//actualExtent.height = std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent.height, actualExtent.height));

				return actualExtent;
			}
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////////////

		uint32_t VulkanDriver::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties)
		{
			VkPhysicalDeviceMemoryProperties memProperties;
			vkGetPhysicalDeviceMemoryProperties(m_physicalDevice, &memProperties);

			for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) 
			{
				if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) 
				{
					return i;
				}
			}

			SH_ASSERT(0, "failed to find suitable memory type!");
			return 0;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////////////

		void VulkanDriver::createImage(
			uint32_t width, uint32_t height,
			VkFormat format,
			VkImageTiling tiling,
			VkImageUsageFlags usage,
			VkMemoryPropertyFlags properties,
			VulkanDeleter<VkImage>& image,
			VulkanDeleter<VkDeviceMemory>& imageMemory)
		{
			VkImageCreateInfo imageInfo = {};
			imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
			imageInfo.imageType = VK_IMAGE_TYPE_2D;
			imageInfo.extent.width = width;
			imageInfo.extent.height = height;
			imageInfo.extent.depth = 1;
			imageInfo.mipLevels = 1;
			imageInfo.arrayLayers = 1;
			imageInfo.format = format;
			imageInfo.tiling = tiling;
			imageInfo.initialLayout = VK_IMAGE_LAYOUT_PREINITIALIZED;
			imageInfo.usage = usage;
			imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
			imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;


			SH_ASSERT(vkCreateImage(m_device, &imageInfo, nullptr, image.Replace()) == VK_SUCCESS,
				"failed to create image!");

			VkMemoryRequirements memRequirements;
			vkGetImageMemoryRequirements(m_device, image, &memRequirements);

			VkMemoryAllocateInfo allocInfo = {};
			allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
			allocInfo.allocationSize = memRequirements.size;
			allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties);


			SH_ASSERT(vkAllocateMemory(m_device, &allocInfo, nullptr, imageMemory.Replace()) == VK_SUCCESS,
				"failed to allocate image memory!");

			vkBindImageMemory(m_device, image, imageMemory, 0);
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////////////

		VkCommandBuffer VulkanDriver::beginSingleTimeCommands() 
		{
			VkCommandBufferAllocateInfo allocInfo = {};
			allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
			allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
			allocInfo.commandPool = m_commandPool;
			allocInfo.commandBufferCount = 1;

			VkCommandBuffer commandBuffer;
			vkAllocateCommandBuffers(m_device, &allocInfo, &commandBuffer);

			VkCommandBufferBeginInfo beginInfo = {};
			beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
			beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

			vkBeginCommandBuffer(commandBuffer, &beginInfo);

			return commandBuffer;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////////////

		void VulkanDriver::endSingleTimeCommands(VkCommandBuffer commandBuffer) {
			vkEndCommandBuffer(commandBuffer);

			VkSubmitInfo submitInfo = {};
			submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
			submitInfo.commandBufferCount = 1;
			submitInfo.pCommandBuffers = &commandBuffer;

			vkQueueSubmit(m_graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
			vkQueueWaitIdle(m_graphicsQueue);

			vkFreeCommandBuffers(m_device, m_commandPool, 1, &commandBuffer);
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////////////

		void VulkanDriver::transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout) 
		{
			VkCommandBuffer commandBuffer = beginSingleTimeCommands();

			VkImageMemoryBarrier barrier = {};
			barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
			barrier.oldLayout = oldLayout;
			barrier.newLayout = newLayout;
			barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
			barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
			barrier.image = image;

			if (newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL) {
				barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;

				if (hasStencilComponent(format)) {
					barrier.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
				}
			}
			else {
				barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			}

			barrier.subresourceRange.baseMipLevel = 0;
			barrier.subresourceRange.levelCount = 1;
			barrier.subresourceRange.baseArrayLayer = 0;
			barrier.subresourceRange.layerCount = 1;

			if (oldLayout == VK_IMAGE_LAYOUT_PREINITIALIZED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL) {
				barrier.srcAccessMask = VK_ACCESS_HOST_WRITE_BIT;
				barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
			}
			else if (oldLayout == VK_IMAGE_LAYOUT_PREINITIALIZED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
				barrier.srcAccessMask = VK_ACCESS_HOST_WRITE_BIT;
				barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
			}
			else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
				barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
				barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
			}
			else if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL) {
				barrier.srcAccessMask = 0;
				barrier.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
			}
			else {
				throw std::invalid_argument("unsupported layout transition!");
			}

			vkCmdPipelineBarrier(
				commandBuffer,
				VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
				0,
				0, nullptr,
				0, nullptr,
				1, &barrier
			);

			endSingleTimeCommands(commandBuffer);
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////////////

		void VulkanDriver::copyImage(VkImage srcImage, VkImage dstImage, uint32_t width, uint32_t height)
		{
			VkCommandBuffer commandBuffer = beginSingleTimeCommands();

			VkImageSubresourceLayers subResource = {};
			subResource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			subResource.baseArrayLayer = 0;
			subResource.mipLevel = 0;
			subResource.layerCount = 1;

			VkImageCopy region = {};
			region.srcSubresource = subResource;
			region.dstSubresource = subResource;
			region.srcOffset = { 0, 0, 0 };
			region.dstOffset = { 0, 0, 0 };
			region.extent.width = width;
			region.extent.height = height;
			region.extent.depth = 1;

			vkCmdCopyImage(
				commandBuffer,
				srcImage, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
				dstImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
				1, &region
			);


			endSingleTimeCommands(commandBuffer);
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////////////

		void VulkanDriver::createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, VulkanDeleter<VkImageView>& imageView)
		{
			VkImageViewCreateInfo viewInfo = {};
			viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			viewInfo.image = image;
			viewInfo.subresourceRange.aspectMask = aspectFlags;
			viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
			viewInfo.format = format;
			viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			viewInfo.subresourceRange.baseMipLevel = 0;
			viewInfo.subresourceRange.levelCount = 1;
			viewInfo.subresourceRange.baseArrayLayer = 0;
			viewInfo.subresourceRange.layerCount = 1;

			SH_ASSERT(vkCreateImageView(m_device, &viewInfo, nullptr, imageView.Replace()) == VK_SUCCESS,
				"failed to create texture image view!");			
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////////////

		VkFormat VulkanDriver::findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features)
		{
			for (VkFormat format : candidates) {
				VkFormatProperties props;
				vkGetPhysicalDeviceFormatProperties(m_physicalDevice, format, &props);

				if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features) 
				{
					return format;
				}
				else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features) 
				{
					return format;
				}
			}

			SH_ASSERT(0, "failed to find supported format!");	
			VkFormat format = VK_FORMAT_UNDEFINED;
			return format;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////////////

		VkFormat VulkanDriver::findDepthFormat()
		{
			return findSupportedFormat(
			{ VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT },
				VK_IMAGE_TILING_OPTIMAL,
				VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
			);
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////////////
	}
}