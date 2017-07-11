#include "VulkanDriver.h"
#include "../../scene/Mesh.h"
#include "../../Device.h"

#include "VulkanVertexBuffer.h"
#include "VulkanIndexBuffer.h"
#include "VulkanRenderPipeline.h"
#include "Batching/VulkanRenderBatchManager.h"
#include "VulkanShader.h"
#include "VulkanSampler.h"
#include "VulkanCommandBuffer.h"
#include "VulkanGpuPipelineParamsInfo.h"
#include "VulkanGpuParams.h"
#include "VulkanShaderCompiler.h"

#include "Managers/VulkanRenderStateManager.h"
#include "Managers/VulkanHardwareBufferManager.h"
#include "Managers/VulkanCommandBufferManager.h"
#include "Managers/VulkanTextureManager.h"

#include "../../gui/SpriteManager.h"

#include "../../scene/SceneManager.h"
#include "../../scene/Camera.h"



#include <iostream>
#include <stdexcept>
#include <functional>
#include <fstream>
#include <algorithm>
#include <vector>
#include <cstring>
#include <set>
#include <limits>

namespace sh
{
	namespace video
	{

		const std::vector<const char*> validationLayers = 
		{
			"VK_LAYER_LUNARG_standard_validation"
		};


		//std::vector<uint32_t> indices;
		
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
		
		static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
			VkDebugReportFlagsEXT flags, 
			VkDebugReportObjectTypeEXT objType, 
			uint64_t obj,
			size_t location,
			int32_t code, 
			const char* layerPrefix, 
			const char* msg, 
			void* userData) 
		{					
			if (flags & VK_DEBUG_REPORT_ERROR_BIT_EXT)
			{
				std::cout << "ERROR: " << "[" << layerPrefix << "] Code " << code << " : " << msg << "\n";
			}
			else if (flags & VK_DEBUG_REPORT_WARNING_BIT_EXT)
			{				
				std::cout << "WARNING: " << "[" << layerPrefix << "] Code " << code << " : " << msg << "\n";
			}
			else
			{
				return false;
			}
			
			return VK_FALSE;
		}

		


		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		VulkanDriver::VulkanDriver()
		{
		}

		VulkanDriver::VulkanDriver(const CreationParameters& parameters)
		{
			InitVulkanFunctions();
			m_parameters = parameters;
			VulkanShaderCompiler::CreateInstance();
		}

		VulkanDriver::~VulkanDriver()
		{
			vkDeviceWaitIdle(m_device);
			VulkanRenderStateManager::DestroyInstance();
			VulkanHardwareBufferManager::DestroyInstance();
			CommandBufferManager::DestroyInstance();
			RenderBatchManager::DestroyInstance();
			TextureManager::DestroyInstance();
		}

		const String& VulkanDriver::GetApiName() const
		{
			static const String s_apiName = "Vulkan";
			return s_apiName;
		}

		DriverType VulkanDriver::GetType() const
		{
			return DriverType::VULKAN;
		}

        void VulkanDriver::SetWindow(void* winId, u32 width, u32 height)
        {
            m_parameters.WinId = winId;
            m_parameters.width = width;
            m_parameters.height = height;
            SetViewport(0U, 0U, width, height);

			static bool first = true;
			if( !first )
			{
				recreateSwapChain(true);
			}
			first = false;
        }

		bool VulkanDriver::Init()
		{
			sh::Device::GetInstance()->windowResizeEvent.Connect(
				std::bind(&VulkanDriver::OnWindowResized, this, std::placeholders::_1, std::placeholders::_2));

			SetupLayersAndExtensions();			
			CreateInstance();
			setupDebugCallback();
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

			CreateDefaultCommadBuffers();

			m_executableCommandBuffers.reserve(100);

			
			RenderStateManager::CreateInstance<VulkanRenderStateManager>();
			HardwareBufferManager::CreateInstance<VulkanHardwareBufferManager>();
			CommandBufferManager::CreateInstance<VulkanCommandBufferManager>();
			RenderBatchManager::CreateInstance<VulkanRenderBatchManager>();
			TextureManager::CreateInstance<VulkanTextureManager>();
			
			return true;
		}

		void VulkanDriver::BeginRendering()
		{
			vkAcquireNextImageKHR(m_device, m_swapChain, std::numeric_limits<u64>::max(), m_imageAvailableSemaphore, VK_NULL_HANDLE, &m_currentImageIndex);
			
			VkFramebuffer framebuffer = m_swapChainFramebuffers[m_currentImageIndex];

			VkCommandBuffer primaryCommandBuffer = m_primaryCommandBuffer->GetVulkanId();//m_commandBuffers[0];
			VkCommandBufferBeginInfo beginInfo = {};
			beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
			beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
			

			std::array<VkClearValue, 2> clearValues = {};
			clearValues[0].color.float32[0] = 0.7f;
			clearValues[0].color.float32[1] = 0.7f;
			clearValues[0].color.float32[2] = 0.7f;
			clearValues[0].color.float32[3] = 1.0f;
			clearValues[1].depthStencil.depth = 1.0f;
			clearValues[1].depthStencil.stencil = 0U;

			VkRenderPassBeginInfo renderPassInfo = {};
			renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
			renderPassInfo.renderPass = m_renderPass;
			renderPassInfo.framebuffer = framebuffer;
			renderPassInfo.renderArea.offset = { 0, 0 };
			renderPassInfo.renderArea.extent.width = m_viewPort.z;
			renderPassInfo.renderArea.extent.height = m_viewPort.w;
			renderPassInfo.clearValueCount = clearValues.size();
			renderPassInfo.pClearValues = clearValues.data();

			vkBeginCommandBuffer(primaryCommandBuffer, &beginInfo);

			// The primary command buffer does not contain any rendering commands
			// These are stored (and retrieved) from the secondary command buffers
			vkCmdBeginRenderPass(primaryCommandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_SECONDARY_COMMAND_BUFFERS);

			m_inheritanceInfo = VkCommandBufferInheritanceInfo();
			m_inheritanceInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_INHERITANCE_INFO;
			m_inheritanceInfo.renderPass = m_renderPass;
			m_inheritanceInfo.framebuffer = framebuffer;
			m_inheritanceInfo.occlusionQueryEnable = VK_FALSE;
			m_inheritanceInfo.pipelineStatistics = 0;
			m_inheritanceInfo.pNext = nullptr;
			m_inheritanceInfo.queryFlags = 0;
			m_inheritanceInfo.subpass = 0;
		}
		void VulkanDriver::EndRendering()
		{
			VkCommandBuffer primaryCommandBuffer = m_primaryCommandBuffer->GetVulkanId();

			m_primaryCommandBuffer->Execute();

			vkCmdEndRenderPass(primaryCommandBuffer);

			VkResult res = vkEndCommandBuffer(primaryCommandBuffer);
			SH_ASSERT(res == VK_SUCCESS, "failed to record command buffer!");

			// Submitting
			VkSubmitInfo submitInfo = {};
			submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
			VkSemaphore waitSemaphores[] = { m_imageAvailableSemaphore };
			VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
			submitInfo.waitSemaphoreCount = 1;
			submitInfo.pWaitSemaphores = waitSemaphores;
			submitInfo.pWaitDstStageMask = waitStages;
			submitInfo.commandBufferCount = 1;
			submitInfo.pCommandBuffers = &primaryCommandBuffer;
			VkSemaphore signalSemaphores[] = { m_renderFinishedSemaphore };
			submitInfo.signalSemaphoreCount = 0;//1;
			submitInfo.pSignalSemaphores = nullptr;//signalSemaphores;

			VkFence renderFence = {};
			VkFenceCreateInfo fenceCreateInfo = {};
			fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
			fenceCreateInfo.flags = 0;
			vkCreateFence(m_device, &fenceCreateInfo, nullptr, &renderFence);
			res = vkQueueSubmit(m_graphicsQueue, 1, &submitInfo, renderFence);
			SH_ASSERT(res == VK_SUCCESS, "Failed to submit draw command buffer!");

			// Wait for fence to signal that all command buffers are ready
			do
			{
				res = vkWaitForFences(m_device, 1, &renderFence, VK_TRUE, 100000000);
			} while (res == VK_TIMEOUT);

			VkPresentInfoKHR presentInfo = {};
			presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
			presentInfo.waitSemaphoreCount = 0;//1;

			presentInfo.pWaitSemaphores = nullptr;//signalSemaphores;
			VkSwapchainKHR swapChains[] = { m_swapChain };
			presentInfo.swapchainCount = 1;
			presentInfo.pSwapchains = swapChains;
			presentInfo.pImageIndices = &m_currentImageIndex;
			presentInfo.pNext = nullptr;
			presentInfo.pResults = nullptr;

			res = vkQueuePresentKHR(m_presentQueue, &presentInfo);
			SH_ASSERT(res == VK_SUCCESS, "Failed to present render result!");
			res = vkQueueWaitIdle(m_presentQueue);
			SH_ASSERT(res == VK_SUCCESS, "Failed waiting!");

			vkDestroyFence(m_device, renderFence, nullptr);

			m_executableCommandBuffers.clear();
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////////////

		void VulkanDriver::Render(scene::Mesh* mesh)
		{
		}


			/*
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
			SH_ASSERT(res == VK_SUCCESS, "Failed to submit draw command buffer!");


			VkPresentInfoKHR presentInfo = {};
			presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
			presentInfo.waitSemaphoreCount = 1;
			presentInfo.pWaitSemaphores = signalSemaphores;
			VkSwapchainKHR swapChains[] = { m_swapChain };
			presentInfo.swapchainCount = 1;
			presentInfo.pSwapchains = swapChains;
			presentInfo.pImageIndices = &m_currentImageIndex;
			res = vkQueuePresentKHR(m_presentQueue, &presentInfo);
			SH_ASSERT(res == VK_SUCCESS, "Failed to present render result!");
			*/

		////////////////////////////////////////////////////////////////////////////////////////////////////////////

		void VulkanDriver::SetRenderPipeline(const RenderPipelinePtr& pipeline, const CommandBufferPtr& commandBuffer)
		{ 
			VulkanCommandBuffer* cmdBuffer = static_cast<VulkanCommandBuffer*>(commandBuffer.get());
			VulkanRenderPipeline* vkPipeline = static_cast<VulkanRenderPipeline*>(pipeline.get());
			m_pipelineTemp = vkPipeline;
			m_layoutTemp = vkPipeline->GetVulkanPipelineLayout();

			//VkPipeline vulkanPipeline = vkPipeline->GetVulkanPipeline(m_vulkanDeclarationTemp);
			//vkCmdBindPipeline(cmdBuffer->GetVulkanId(), VK_PIPELINE_BIND_POINT_GRAPHICS, vulkanPipeline);
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////////////

		void VulkanDriver::SetGpuParams(const GpuParamsPtr& params, const CommandBufferPtr& commandBuffer) 
		{ 
			VulkanCommandBuffer* cmdBuffer = static_cast<VulkanCommandBuffer*>(commandBuffer.get());
			VulkanGpuParams* vkParams = static_cast<VulkanGpuParams*>(params.get());
			

			const u8* data = params->GetData();
			const auto& samplers = params->GetSamplers();

			for( size_t i = 0; i < 6U; ++i )
			{
				ShaderType shaderType = static_cast<ShaderType>(i);

				const SPtr<GpuParamsDescription>& desc = params->GetDescription(shaderType);
				if(!desc)
					continue;

				for (const auto& param : desc->params)
				{
					const u8* dataPtr = data + param.second.offset;
					
					vkCmdPushConstants(
						cmdBuffer->GetVulkanId(),  
						m_layoutTemp, 
						s_vkShaderType[i], 
						param.second.offset, 
						param.second.size, 
						dataPtr);
				}

				
				for (const auto& samplerDesc : desc->samplers)
				{
					const auto& sampler = samplers.at(samplerDesc.first);
					
					VulkanSampler* vkSampler = static_cast<VulkanSampler*>(sampler.get());
					VkDescriptorImageInfo imageInfo = vkSampler->GetDescriptor();

					VkWriteDescriptorSet descriptorWrite = {};
					descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
					descriptorWrite.dstSet = vkParams->GetDescriptorSet();
					descriptorWrite.dstBinding = 0;
					descriptorWrite.dstArrayElement = 0;
					descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
					descriptorWrite.descriptorCount = 1;
					descriptorWrite.pImageInfo = &imageInfo; 
					descriptorWrite.pBufferInfo = nullptr;
					descriptorWrite.pNext = nullptr;
					descriptorWrite.pTexelBufferView = nullptr;

					vkUpdateDescriptorSets(m_device, 1, &descriptorWrite, 0, nullptr);

					VkDescriptorSet descriptorSet = vkParams->GetDescriptorSet();

					vkCmdBindDescriptorSets(cmdBuffer->GetVulkanId(), VK_PIPELINE_BIND_POINT_GRAPHICS, m_layoutTemp, 0, 1, &descriptorSet, 0, nullptr);
				}

			}
			
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////////////

		void VulkanDriver::SetTopology(Topology topology, const CommandBufferPtr& commandBuffer) 
		{ 
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////////////

		void VulkanDriver::SetAutoUniformsBatch(const UniformsBatchPtr& batch, const CommandBufferPtr& commandBuffer) 
		{ 
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////////////

		void VulkanDriver::SetVertexDeclaration(const VertexInputDeclarationPtr& declaration, const CommandBufferPtr& commandBuffer) 
		{ 
			VulkanCommandBuffer* cmdBuffer = static_cast<VulkanCommandBuffer*>(commandBuffer.get());
			m_vulkanDeclarationTemp = declaration;

			VkPipeline vulkanPipeline = m_pipelineTemp->GetVulkanPipeline(m_vulkanDeclarationTemp);
			vkCmdBindPipeline(cmdBuffer->GetVulkanId(), VK_PIPELINE_BIND_POINT_GRAPHICS, vulkanPipeline);
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////////////

		void VulkanDriver::SetVertexBuffer(const VertexBufferPtr& buffer, const CommandBufferPtr& commandBuffer) 
		{ 
			VulkanCommandBuffer* cmdBuffer = static_cast<VulkanCommandBuffer*>(commandBuffer.get());
			VulkanVertexBuffer* vtxBuffer = static_cast<VulkanVertexBuffer*>(buffer.get());

			VkDeviceSize offsets[1] = { 0 };
			VkBuffer vertexBuffer = vtxBuffer->GetVulkanId();
			vkCmdBindVertexBuffers(cmdBuffer->GetVulkanId(), 0, 1, &vertexBuffer, offsets);
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////////////

		void VulkanDriver::SetIndexBuffer(const IndexBufferPtr& buffer, const CommandBufferPtr& commandBuffer) 
		{ 
			VulkanCommandBuffer* cmdBuffer = static_cast<VulkanCommandBuffer*>(commandBuffer.get());
			VulkanIndexBuffer* indBuffer = static_cast<VulkanIndexBuffer*>(buffer.get());

			VkBuffer indexBuffer = indBuffer->GetVulkanId();
			vkCmdBindIndexBuffer(cmdBuffer->GetVulkanId(), indexBuffer, 0, VK_INDEX_TYPE_UINT32);
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////////////

		void VulkanDriver::Draw(u32 offset, u32 verticesCount, u32 instancesCount, const CommandBufferPtr& commandBuffer) 
		{ 
			VulkanCommandBuffer* cmdBuffer = static_cast<VulkanCommandBuffer*>(commandBuffer.get());
			vkCmdDraw(cmdBuffer->GetVulkanId(), verticesCount, instancesCount, offset, 0U);
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////////////

		void VulkanDriver::DrawIndexed(u32 offset, u32 indicesCount, u32 instancesCount, const CommandBufferPtr& commandBuffer) 
		{ 
			VulkanCommandBuffer* cmdBuffer = static_cast<VulkanCommandBuffer*>(commandBuffer.get());
			vkCmdDrawIndexed(cmdBuffer->GetVulkanId(), indicesCount, instancesCount, 0, 0, 0);
		}
		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////

		void VulkanDriver::SubmitCommandBuffer( const CommandBufferPtr& commandBuffer )
		{
			VulkanCommandBuffer* cmdBuf = static_cast<VulkanCommandBuffer*>(commandBuffer.get());
			m_primaryCommandBuffer->Append(cmdBuf);
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////////////

		void VulkanDriver::GetPixelData(u32 x, u32 y, u32 width, u32 height, u8* data)
		{
			// Create temporary buffer
			VkDeviceSize bufferSize = width * height * 4 * 4 * 2;
			VkBuffer stagingBuffer;
			VkDeviceMemory stagingBufferMemory;
			createBuffer(
				bufferSize,
				VK_BUFFER_USAGE_TRANSFER_DST_BIT,
				VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
				stagingBuffer,
				stagingBufferMemory);


			VkCommandBuffer commandBuffer = beginSingleTimeCommands();

			VkImage srcImage = m_swapChainImages[m_currentImageIndex];

			VkBufferImageCopy imageCopyRegion = {};
			imageCopyRegion.bufferOffset = 0;
			imageCopyRegion.bufferRowLength = 0;
			imageCopyRegion.bufferImageHeight = 0;
			imageCopyRegion.imageExtent.width = width;
			imageCopyRegion.imageExtent.height = height;
			imageCopyRegion.imageExtent.depth = 1;

			imageCopyRegion.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			imageCopyRegion.imageSubresource.baseArrayLayer = 0;
			imageCopyRegion.imageSubresource.layerCount = 1;
			imageCopyRegion.imageSubresource.mipLevel = 0;
			VkImageLayout srcImageLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;


			


			// Fill temporary buffer with data
			//void* bufferData;
			//vkMapMemory(device, stagingBufferMemory, 0, bufferSize, 0, &bufferData);
			//memcpy(bufferData, data, (size_t)bufferSize);
			//vkUnmapMemory(device, stagingBufferMemory);

			// Create fina buffer
			//createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_buffer, m_bufferMemory);
			// Copy data from temporary buffer to final buffer
			//copyBuffer(stagingBuffer, m_buffer, bufferSize);

			// Free temporary buffer
			

			vkCmdCopyImageToBuffer(commandBuffer, srcImage, srcImageLayout, stagingBuffer, 1, &imageCopyRegion);

			void* bufferData;
			vkMapMemory(m_device, stagingBufferMemory, 0, bufferSize, 0, &bufferData);
			memcpy(data, bufferData, 4);
			vkUnmapMemory(m_device, stagingBufferMemory);

			

			endSingleTimeCommands(commandBuffer);


			vkFreeMemory(m_device, stagingBufferMemory, nullptr);
			vkDestroyBuffer(m_device, stagingBuffer, nullptr);

			int r = data[0];
			int g = data[1];
			int b = data[2];
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////////////

		void VulkanDriver::OnWindowResized(int width, int height)
		{
			m_parameters.width = static_cast<u32>(width);
			m_parameters.height = static_cast<u32>(height);
			SetViewport(0U, 0U, width, height);

			recreateSwapChain(false);
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
			// Needs for debug message handling
			m_instanceExtensionsList.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
			// Needed for surface to draw on
			m_instanceExtensionsList.push_back(VK_KHR_SURFACE_EXTENSION_NAME);
			// Needed for platform-specific surface creation
#if defined SHADOW_WINDOWS
			m_instanceExtensionsList.push_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);
#elif defined SHADOW_ANDROID
			m_instanceExtensionsList.push_back(VK_KHR_ANDROID_SURFACE_EXTENSION_NAME);
#endif
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
			//SH_ASSERT(chechValidationLayers(), "Can not create Vulkan instance! Not all the validation layers are supported!");


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

			createInfo.enabledLayerCount = validationLayers.size();
			createInfo.ppEnabledLayerNames = validationLayers.data();

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

			createInfo.enabledLayerCount = validationLayers.size();
			createInfo.ppEnabledLayerNames = validationLayers.data();

			SH_ASSERT(vkCreateDevice(m_physicalDevice, &createInfo, nullptr, m_device.Replace()) == VK_SUCCESS,
				"failed to create logical device!");

			vkGetDeviceQueue(m_device, indices.graphicsFamily, 0, &m_graphicsQueue);
			vkGetDeviceQueue(m_device, indices.presentFamily, 0, &m_presentQueue);
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////////////

		void VulkanDriver::CreateSurface()
		{
            VkResult res;
#if defined SHADOW_WINDOWS
			VkWin32SurfaceCreateInfoKHR createInfo{};
			createInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
			createInfo.hinstance = GetModuleHandle(nullptr);
			createInfo.hwnd = static_cast<HWND>(m_parameters.WinId);
			res = vkCreateWin32SurfaceKHR(m_instance, &createInfo, nullptr, &m_surface);
			SH_ASSERT(res == VK_SUCCESS, "failed to create window surface!");
#elif defined SHADOW_ANDROID
			VkAndroidSurfaceCreateInfoKHR createInfo{};
			createInfo.sType = VK_STRUCTURE_TYPE_ANDROID_SURFACE_CREATE_INFO_KHR;
			createInfo.pNext = nullptr;
			createInfo.flags = 0;
			createInfo.window = static_cast<ANativeWindow*>(m_parameters.WinId);
            res = vkCreateAndroidSurfaceKHR(m_instance, &createInfo, nullptr, &m_surface);
			SH_ASSERT(res == VK_SUCCESS, "failed to create window surface!");
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

			SH_ASSERT(vkCreateSwapchainKHR(m_device, &createInfo, nullptr, &m_swapChain) == VK_SUCCESS,
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

				SH_ASSERT(vkCreateFramebuffer(m_device, &framebufferInfo, nullptr, &m_swapChainFramebuffers[i]) == VK_SUCCESS,
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
			poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT; // Optional

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

		void VulkanDriver::CreateDefaultCommadBuffers()
		{
			m_commandBuffers.resize(1);
			VkCommandBufferAllocateInfo allocInfo = {};
			allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
			allocInfo.commandPool = m_commandPool;
			allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
			allocInfo.commandBufferCount = 1;

			VkResult res = vkAllocateCommandBuffers(m_device, &allocInfo, m_commandBuffers.data());
			SH_ASSERT(res == VK_SUCCESS, "Failed to allocate command buffers!");


			//CommandBufferDescription desc;
			//desc.type = COMMAND_BUFFER_TYPE_PRIMARY;
			//m_primaryCommandBuffer = new VulkanCommandBuffer(desc);
			m_primaryCommandBuffer = new VulkanCommandBuffer();
			m_primaryCommandBuffer->m_commandBuffer = m_commandBuffers[0];

			/*
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
				renderPassInfo.renderArea.extent.width = m_viewPort.z;
				renderPassInfo.renderArea.extent.height = m_viewPort.w;

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
				vkCmdEndRenderPass(m_commandBuffers[i]);

				VkResult res = vkEndCommandBuffer(m_commandBuffers[i]);
				SH_ASSERT(res == VK_SUCCESS, "failed to record command buffer!");
			}
			*/
		}
		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////

		void VulkanDriver::CreateSemaphores()
		{
			VkSemaphoreCreateInfo semaphoreInfo = {};
			semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
			semaphoreInfo.pNext = nullptr;
			semaphoreInfo.flags = 0;

			VkResult res = vkCreateSemaphore(m_device, &semaphoreInfo, nullptr, &m_imageAvailableSemaphore);
			SH_ASSERT(res == VK_SUCCESS, "Failed to create semaphores!");

			res = vkCreateSemaphore(m_device, &semaphoreInfo, nullptr, &m_renderFinishedSemaphore);
			SH_ASSERT(res == VK_SUCCESS, "Failed to create semaphores!");			

			res = vkCreateSemaphore(m_device, &semaphoreInfo, nullptr, &m_showImageSemaphore);
			SH_ASSERT(res == VK_SUCCESS, "Failed to create semaphores!");
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

			VkResult res = vkCreateRenderPass(m_device, &renderPassInfo, nullptr, &m_renderPass);
			SH_ASSERT(res == VK_SUCCESS, "Failed to create render pass!");
			
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
			if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) 
			{
				return capabilities.currentExtent;
			}
			else 
			{
				VkExtent2D actualExtent = { 800, 600 };
				
				actualExtent.width = std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent.width, actualExtent.width));
				actualExtent.height = std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent.height, actualExtent.height));

				return actualExtent;
			}
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool VulkanDriver::chechValidationLayers()
		{
			uint32_t layerCount;
			vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

			std::vector<VkLayerProperties> availableLayers(layerCount);
			vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

			for (const char* layerName : validationLayers) 
			{
				bool layerFound = false;

				for (const auto& layerProperties : availableLayers) 
				{
					if (strcmp(layerName, layerProperties.layerName) == 0) 
					{
						layerFound = true;
						break;
					}
				}

				if (!layerFound) 
				{
					return false;
				}
			}

			return true;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////////////

		void VulkanDriver::setupDebugCallback()
		{

			VkDebugReportCallbackCreateInfoEXT createInfo = {};
			createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
			createInfo.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT;
			createInfo.pfnCallback = debugCallback;

			VkResult res = CreateDebugReportCallbackEXT(m_instance, &createInfo, nullptr, m_callback.Replace());
			SH_ASSERT(res == VK_SUCCESS, "Failed to set up debug callback!");
			
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////////////

		void VulkanDriver::recreateSwapChain(bool recreateSurface)
		{
			vkDeviceWaitIdle(m_device);

			cleanupSwapChain(recreateSurface);

			if (recreateSurface)
				CreateSurface();

			CreateSwapChain();
			CreateImageViews();
			//CreateRenderPass();
			CreateDepthResources();
			CreateFramebuffers();
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////////////

		void VulkanDriver::cleanupSwapChain(bool destroySurface)
		{
			for (size_t i = 0; i < m_swapChainFramebuffers.size(); i++) 
			{
				vkDestroyFramebuffer(m_device, m_swapChainFramebuffers[i], nullptr);
			}


			//vkDestroyRenderPass(m_device, m_renderPass, nullptr);

			for (size_t i = 0; i < m_swapChainImageViews.size(); i++) 
			{
				vkDestroyImageView(m_device, m_swapChainImageViews[i], nullptr);
			}

			vkDestroyImageView(m_device, m_depthImageView, nullptr);
			vkDestroyImage(m_device, m_depthImage, nullptr);
			vkFreeMemory(m_device, m_depthImageMemory, nullptr);

			vkDestroySwapchainKHR(m_device, m_swapChain, nullptr);

			if (destroySurface)
				vkDestroySurfaceKHR(m_instance, m_surface, nullptr);
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
			VkImage& image,
			VkDeviceMemory& imageMemory)
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


			SH_ASSERT(vkCreateImage(m_device, &imageInfo, nullptr, &image) == VK_SUCCESS,
				"failed to create image!");

			VkMemoryRequirements memRequirements;
			vkGetImageMemoryRequirements(m_device, image, &memRequirements);

			VkMemoryAllocateInfo allocInfo = {};
			allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
			allocInfo.allocationSize = memRequirements.size;
			allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties);


			SH_ASSERT(vkAllocateMemory(m_device, &allocInfo, nullptr, &imageMemory) == VK_SUCCESS,
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

		void VulkanDriver::createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory)
		{
			VkBufferCreateInfo bufferInfo = {};
			bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
			bufferInfo.size = size;
			bufferInfo.usage = usage;
			bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

			SH_ASSERT(vkCreateBuffer(m_device, &bufferInfo, nullptr, &buffer) == VK_SUCCESS, "failed to create buffer!");

			VkMemoryRequirements memRequirements;
			vkGetBufferMemoryRequirements(m_device, buffer, &memRequirements);

			VkMemoryAllocateInfo allocInfo = {};
			allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
			allocInfo.allocationSize = memRequirements.size;
			allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties);

			SH_ASSERT(vkAllocateMemory(m_device, &allocInfo, nullptr, &bufferMemory) == VK_SUCCESS, "failed to allocate vertex buffer memory!");

			vkBindBufferMemory(m_device, buffer, bufferMemory, 0);
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

		void VulkanDriver::createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, VkImageView& imageView)
		{
			VkImageViewCreateInfo viewInfo = {};
			viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			viewInfo.image = image;
			viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
			viewInfo.format = format;
			viewInfo.subresourceRange.aspectMask = aspectFlags;
			viewInfo.subresourceRange.baseMipLevel = 0;
			viewInfo.subresourceRange.levelCount = 1;
			viewInfo.subresourceRange.baseArrayLayer = 0;
			viewInfo.subresourceRange.layerCount = 1;
			viewInfo.flags = 0;

			VkResult res = vkCreateImageView(m_device, &viewInfo, nullptr, &imageView);
			SH_ASSERT(res == VK_SUCCESS, "Failed to create texture image view!");			
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////////////

		VkFormat VulkanDriver::findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features)
		{
			for (VkFormat format : candidates)
			{
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