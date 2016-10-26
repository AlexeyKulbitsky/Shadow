#include "VulkanDriver.h"
#include "../HardwareBuffer.h"
#include "../../scene/Mesh.h"
#include "../../pempek_assert.h"
#include "VulkanVertexInput.h"

#include <iostream>
#include <stdexcept>
#include <functional>
#include <fstream>
#include <algorithm>
#include <vector>
#include <cstring>
#include <set>



namespace sh
{
	namespace video
	{
		const std::vector<VulkanVertexDeclaration::Vertex> vertices = 
		{
			{ { 0.0f, -0.5f },{ 1.0f, 1.0f, 0.0f } },
			{ { 0.5f, 0.5f },{ 1.0f, 1.0f, 0.0f } },
			{ { -0.5f, 0.5f },{ 0.0f, 0.0f, 1.0f } }
		};

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
			SetupLayersAndExtensions();
			CreateInstance();
			CreateSurface();
			PickPhysicalDevice();
			CreateLogicalDevice();
			CreateSwapChain();
			CreateImageViews();
			CreateRenderPass();
			CreateGraphicsPipeline();
			CreateFramebuffers();
			CreateCommandPool();
			CreateVertexBuffer();
			CreateCommandBuffers();
			CreateSemaphores();

			return true;
		}

		void VulkanDriver::BeginRendering()
		{

		}
		void VulkanDriver::EndRendering()
		{

		}

		////////////////////////////////////////////////////////////////////////////////////////////////////////////

		void VulkanDriver::DrawMesh(scene::Mesh* mesh)
		{

		}

		////////////////////////////////////////////////////////////////////////////////////////////////////////////

		void VulkanDriver::DrawHardwareBuffer(HardwareBuffer *buffer) const
		{
			uint32_t imageIndex;
			vkAcquireNextImageKHR(m_device, m_swapChain, 500000/*std::numeric_limits<uint64_t>::max()*/, m_imageAvailableSemaphore, VK_NULL_HANDLE, &imageIndex);

			VkSubmitInfo submitInfo = {};
			submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

			VkSemaphore waitSemaphores[] = { m_imageAvailableSemaphore };
			VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
			submitInfo.waitSemaphoreCount = 1;
			submitInfo.pWaitSemaphores = waitSemaphores;
			submitInfo.pWaitDstStageMask = waitStages;

			submitInfo.commandBufferCount = 1;
			submitInfo.pCommandBuffers = &m_commandBuffers[imageIndex];

			VkSemaphore signalSemaphores[] = { m_renderFinishedSemaphore };
			submitInfo.signalSemaphoreCount = 1;
			submitInfo.pSignalSemaphores = signalSemaphores;


			SH_ASSERT(vkQueueSubmit(m_graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE) == VK_SUCCESS,
				"failed to submit draw command buffer!");
			

			VkPresentInfoKHR presentInfo = {};
			presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

			presentInfo.waitSemaphoreCount = 1;
			presentInfo.pWaitSemaphores = signalSemaphores;

			VkSwapchainKHR swapChains[] = { m_swapChain };
			presentInfo.swapchainCount = 1;
			presentInfo.pSwapchains = swapChains;

			presentInfo.pImageIndices = &imageIndex;

			vkQueuePresentKHR(m_presentQueue, &presentInfo);
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////////////

		HardwareBuffer* VulkanDriver::CreateHardwareBuffer(const HardwareBuffer::CreateInfo& info) const
		{
			return nullptr;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////////////

		void VulkanDriver::InitHardwareBuffer(HardwareBuffer *buffer, const void* verticesPointer, u32 verticesSize, const void* indicesPointer, u32 indicesSize) const
		{

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

			m_instanceExtensionsList.push_back(VK_KHR_SURFACE_EXTENSION_NAME);

			// Needed for platform-specific surface creation
			m_instanceExtensionsList.push_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);

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
			VkWin32SurfaceCreateInfoKHR createInfo{};
			createInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
			createInfo.hinstance = GetModuleHandle(nullptr);
			createInfo.hwnd = static_cast<HWND>(m_parameters.WinId);
			SH_ASSERT(vkCreateWin32SurfaceKHR(m_instance, &createInfo, nullptr, m_surface.Replace()) == VK_SUCCESS,
				"failed to create window surface!");
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
		
			for (uint32_t i = 0; i < m_swapChainImages.size(); i++) 
			{
				VkImageViewCreateInfo createInfo = {};
				createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
				createInfo.image = m_swapChainImages[i];
				createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
				createInfo.format = m_swapChainImageFormat;
				createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
				createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
				createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
				createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
				createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
				createInfo.subresourceRange.baseMipLevel = 0;
				createInfo.subresourceRange.levelCount = 1;
				createInfo.subresourceRange.baseArrayLayer = 0;
				createInfo.subresourceRange.layerCount = 1;

				SH_ASSERT(vkCreateImageView(m_device, &createInfo, nullptr, m_swapChainImageViews[i].Replace()) == VK_SUCCESS,
					"failed to create image views!");
				
			}
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////////////

		void VulkanDriver::CreateGraphicsPipeline()
		{
			auto vertShaderCode = readFile("../data/shaders/vulkan/vert.spv");
			auto fragShaderCode = readFile("../data/shaders/vulkan/frag.spv");

			VulkanDeleter<VkShaderModule> vertShaderModule{ m_device, vkDestroyShaderModule };
			VulkanDeleter<VkShaderModule> fragShaderModule{ m_device, vkDestroyShaderModule };
			createShaderModule(vertShaderCode, vertShaderModule);
			createShaderModule(fragShaderCode, fragShaderModule);

			VkPipelineShaderStageCreateInfo vertShaderStageInfo = {};
			vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
			vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
			vertShaderStageInfo.module = vertShaderModule;
			vertShaderStageInfo.pName = "main";

			VkPipelineShaderStageCreateInfo fragShaderStageInfo = {};
			fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
			fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
			fragShaderStageInfo.module = fragShaderModule;
			fragShaderStageInfo.pName = "main";

			VkPipelineShaderStageCreateInfo shaderStages[] = { vertShaderStageInfo, fragShaderStageInfo };

			VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};
			vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;

			auto bindingDescription = VulkanVertexDeclaration::getBindingDescription();
			auto attributeDescriptions = VulkanVertexDeclaration::getAttributeDescriptions();

			vertexInputInfo.vertexBindingDescriptionCount = 1;
			vertexInputInfo.vertexAttributeDescriptionCount = attributeDescriptions.size();
			vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
			vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

			VkPipelineInputAssemblyStateCreateInfo inputAssembly = {};
			inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
			inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
			inputAssembly.primitiveRestartEnable = VK_FALSE;

			VkViewport viewport = {};
			viewport.x = 0.0f;
			viewport.y = 0.0f;
			viewport.width = (float)m_swapChainExtent.width;
			viewport.height = (float)m_swapChainExtent.height;
			viewport.minDepth = 0.0f;
			viewport.maxDepth = 1.0f;

			VkRect2D scissor = {};
			scissor.offset = { 0, 0 };
			scissor.extent = m_swapChainExtent;

			VkPipelineViewportStateCreateInfo viewportState = {};
			viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
			viewportState.viewportCount = 1;
			viewportState.pViewports = &viewport;
			viewportState.scissorCount = 1;
			viewportState.pScissors = &scissor;

			VkPipelineRasterizationStateCreateInfo rasterizer = {};
			rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
			rasterizer.depthClampEnable = VK_FALSE;
			rasterizer.rasterizerDiscardEnable = VK_FALSE;
			rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
			rasterizer.lineWidth = 1.0f;
			rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
			rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
			rasterizer.depthBiasEnable = VK_FALSE;

			VkPipelineMultisampleStateCreateInfo multisampling = {};
			multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
			multisampling.sampleShadingEnable = VK_FALSE;
			multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

			VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
			colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
			colorBlendAttachment.blendEnable = VK_FALSE;

			VkPipelineColorBlendStateCreateInfo colorBlending = {};
			colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
			colorBlending.logicOpEnable = VK_FALSE;
			colorBlending.logicOp = VK_LOGIC_OP_COPY;
			colorBlending.attachmentCount = 1;
			colorBlending.pAttachments = &colorBlendAttachment;
			colorBlending.blendConstants[0] = 0.0f;
			colorBlending.blendConstants[1] = 0.0f;
			colorBlending.blendConstants[2] = 0.0f;
			colorBlending.blendConstants[3] = 0.0f;

			VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
			pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
			pipelineLayoutInfo.setLayoutCount = 0;
			pipelineLayoutInfo.pushConstantRangeCount = 0;

			SH_ASSERT(vkCreatePipelineLayout(m_device, &pipelineLayoutInfo, nullptr, m_pipelineLayout.Replace()) == VK_SUCCESS,
				"failed to create pipeline layout!");
			
			VkGraphicsPipelineCreateInfo pipelineInfo = {};
			pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
			pipelineInfo.stageCount = 2;
			pipelineInfo.pStages = shaderStages;
			pipelineInfo.pVertexInputState = &vertexInputInfo;
			pipelineInfo.pInputAssemblyState = &inputAssembly;
			pipelineInfo.pViewportState = &viewportState;
			pipelineInfo.pRasterizationState = &rasterizer;
			pipelineInfo.pMultisampleState = &multisampling;
			pipelineInfo.pColorBlendState = &colorBlending;
			pipelineInfo.layout = m_pipelineLayout;
			pipelineInfo.renderPass = m_renderPass;
			pipelineInfo.subpass = 0;
			pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

			SH_ASSERT(vkCreateGraphicsPipelines(m_device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, m_graphicsPipeline.Replace()) == VK_SUCCESS,
				"failed to create graphics pipeline!");
			
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////////////

		void VulkanDriver::CreateFramebuffers()
		{
			m_swapChainFramebuffers.resize(m_swapChainImageViews.size(), VulkanDeleter<VkFramebuffer>{m_device, vkDestroyFramebuffer});

			for (size_t i = 0; i < m_swapChainImageViews.size(); i++) 
			{
				VkImageView attachments[] = 
				{
					m_swapChainImageViews[i]
				};

				VkFramebufferCreateInfo framebufferInfo = {};
				framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
				framebufferInfo.renderPass = m_renderPass;
				framebufferInfo.attachmentCount = 1;
				framebufferInfo.pAttachments = attachments;
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

		void VulkanDriver::CreateCommandBuffers()
		{
			if (m_commandBuffers.size() > 0) 
			{
				vkFreeCommandBuffers(m_device, m_commandPool, m_commandBuffers.size(), m_commandBuffers.data());
			}
			m_commandBuffers.resize(m_swapChainFramebuffers.size());

			VkCommandBufferAllocateInfo allocInfo = {};
			allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
			allocInfo.commandPool = m_commandPool;
			allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
			allocInfo.commandBufferCount = (uint32_t)m_commandBuffers.size();

			SH_ASSERT(vkAllocateCommandBuffers(m_device, &allocInfo, m_commandBuffers.data()) == VK_SUCCESS,
				"failed to allocate command buffers!");

			for (size_t i = 0; i < m_commandBuffers.size(); i++) 
			{
				VkCommandBufferBeginInfo beginInfo = {};
				beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
				beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;

				vkBeginCommandBuffer(m_commandBuffers[i], &beginInfo);

				VkRenderPassBeginInfo renderPassInfo = {};
				renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
				renderPassInfo.renderPass = m_renderPass;
				renderPassInfo.framebuffer = m_swapChainFramebuffers[i];
				renderPassInfo.renderArea.offset = { 0, 0 };
				renderPassInfo.renderArea.extent = m_swapChainExtent;

				VkClearValue clearColor = { 0.0f, 0.0f, 0.0f, 1.0f };
				renderPassInfo.clearValueCount = 1;
				renderPassInfo.pClearValues = &clearColor;

				vkCmdBeginRenderPass(m_commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

				vkCmdBindPipeline(m_commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, m_graphicsPipeline);

				VkBuffer vertexBuffers[] = { m_vertexBuffer };
				VkDeviceSize offsets[] = { 0 };
				vkCmdBindVertexBuffers(m_commandBuffers[i], 0, 1, vertexBuffers, offsets);

				vkCmdDraw(m_commandBuffers[i], vertices.size(), 1, 0, 0);

				vkCmdEndRenderPass(m_commandBuffers[i]);

				SH_ASSERT(vkEndCommandBuffer(m_commandBuffers[i]) == VK_SUCCESS,
					"failed to record command buffer!");			
			}

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

		void VulkanDriver::CreateVertexBuffer()
		{
			VkBufferCreateInfo bufferInfo = {};
			bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
			bufferInfo.size = sizeof(vertices[0]) * vertices.size();
			bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
			bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

			SH_ASSERT(vkCreateBuffer(m_device, &bufferInfo, nullptr, m_vertexBuffer.Replace()) == VK_SUCCESS,
				"failed to create vertex buffer!");
			

			VkMemoryRequirements memRequirements;
			vkGetBufferMemoryRequirements(m_device, m_vertexBuffer, &memRequirements);

			VkMemoryAllocateInfo allocInfo = {};
			allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
			allocInfo.allocationSize = memRequirements.size;
			allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

			SH_ASSERT(vkAllocateMemory(m_device, &allocInfo, nullptr, m_vertexBufferMemory.Replace()) == VK_SUCCESS,
				"failed to allocate vertex buffer memory!");
			
			vkBindBufferMemory(m_device, m_vertexBuffer, m_vertexBufferMemory, 0);

			void* data;
			vkMapMemory(m_device, m_vertexBufferMemory, 0, bufferInfo.size, 0, &data);
			memcpy(data, vertices.data(), (size_t)bufferInfo.size);
			vkUnmapMemory(m_device, m_vertexBufferMemory);

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

			VkAttachmentReference colorAttachmentRef = {};
			colorAttachmentRef.attachment = 0;
			colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

			VkSubpassDescription subPass = {};
			subPass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
			subPass.colorAttachmentCount = 1;
			subPass.pColorAttachments = &colorAttachmentRef;

			VkSubpassDependency dependency = {};
			dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
			dependency.dstSubpass = 0;
			dependency.srcStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
			dependency.srcAccessMask = VK_ACCESS_MEMORY_READ_BIT;
			dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
			dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

			VkRenderPassCreateInfo renderPassInfo = {};
			renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
			renderPassInfo.attachmentCount = 1;
			renderPassInfo.pAttachments = &colorAttachment;
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

		std::vector<char> VulkanDriver::readFile(const std::string& filename)
		{
			std::ifstream file(filename, std::ios::ate | std::ios::binary);

			SH_ASSERT(file.is_open(), "failed to open file!");
			
			size_t fileSize = (size_t)file.tellg();
			std::vector<char> buffer(fileSize);

			file.seekg(0);
			file.read(buffer.data(), fileSize);
			file.close();

			return buffer;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////////////
		
		void VulkanDriver::createShaderModule(const std::vector<char>& code, VulkanDeleter<VkShaderModule>& shaderModule)
		{
			VkShaderModuleCreateInfo createInfo = {};
			createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
			createInfo.codeSize = code.size();
			createInfo.pCode = (uint32_t*)code.data();

			SH_ASSERT(vkCreateShaderModule(m_device, &createInfo, nullptr, shaderModule.Replace()) == VK_SUCCESS,
				"failed to create shader module!");			
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
		}
	}
}