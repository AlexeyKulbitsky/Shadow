#include "VulkanFunctions.h"

#if defined(SHADOW_WINDOWS)
#include <Windows.h>
#elif defined(SHADOW_ANDROID)
#include <dlfcn.h>
#endif

#if defined(SHADOW_WINDOWS)

HMODULE vulkanLibrary = nullptr;
#define LOAD_VULKAN_FUNCTION(fun)														\
	if (! (fun = reinterpret_cast<PFN_##fun>(GetProcAddress(vulkanLibrary, #fun) ) ) )	\
		SH_ASSERT(0, "ERROR LOADING VULKAN FUNCTION!");


#elif defined(SHADOW_ANDROID)

void* vulkanLibrary = nullptr;
#define LOAD_VULKAN_FUNCTION(fun)														\
	if (! (fun = reinterpret_cast<PFN_##fun>(dlsym(vulkanLibrary, #fun) ) )				\
		SH_ASSERT(0, "ERROR LOADING VULKAN FUNCTION!");


#endif


bool InitVulkanFunctions()
{
#if defined(SHADOW_WINDOWS)
	vulkanLibrary = LoadLibrary("vulkan-1.dll");
#elif defined(SHADOW_ANDROID)
	vulkanLibrary = dlopen("libvulkan.so", RTLD_NOW | RTLD_LOCAL);
#endif
	if (!vulkanLibrary)
		return false;

	// Vulkan supported, set function addresses
	LOAD_VULKAN_FUNCTION(vkCreateInstance);
	LOAD_VULKAN_FUNCTION(vkDestroyInstance);
	LOAD_VULKAN_FUNCTION(vkEnumeratePhysicalDevices);
	LOAD_VULKAN_FUNCTION(vkGetPhysicalDeviceFeatures);
	LOAD_VULKAN_FUNCTION(vkGetPhysicalDeviceFormatProperties);
	LOAD_VULKAN_FUNCTION(vkGetPhysicalDeviceImageFormatProperties);
	LOAD_VULKAN_FUNCTION(vkGetPhysicalDeviceProperties);
	LOAD_VULKAN_FUNCTION(vkGetPhysicalDeviceQueueFamilyProperties);
	LOAD_VULKAN_FUNCTION(vkGetPhysicalDeviceMemoryProperties);
	LOAD_VULKAN_FUNCTION(vkGetInstanceProcAddr);
	LOAD_VULKAN_FUNCTION(vkGetDeviceProcAddr);
	LOAD_VULKAN_FUNCTION(vkCreateDevice);
	LOAD_VULKAN_FUNCTION(vkDestroyDevice);
	LOAD_VULKAN_FUNCTION(vkEnumerateInstanceExtensionProperties);
	LOAD_VULKAN_FUNCTION(vkEnumerateDeviceExtensionProperties);
	LOAD_VULKAN_FUNCTION(vkEnumerateInstanceLayerProperties);
	LOAD_VULKAN_FUNCTION(vkEnumerateDeviceLayerProperties);
	LOAD_VULKAN_FUNCTION(vkGetDeviceQueue);
	LOAD_VULKAN_FUNCTION(vkQueueSubmit);
	LOAD_VULKAN_FUNCTION(vkQueueWaitIdle);
	LOAD_VULKAN_FUNCTION(vkDeviceWaitIdle);
	LOAD_VULKAN_FUNCTION(vkAllocateMemory);
	LOAD_VULKAN_FUNCTION(vkFreeMemory);
	LOAD_VULKAN_FUNCTION(vkMapMemory);
	LOAD_VULKAN_FUNCTION(vkUnmapMemory);
	LOAD_VULKAN_FUNCTION(vkFlushMappedMemoryRanges);
	LOAD_VULKAN_FUNCTION(vkInvalidateMappedMemoryRanges);
	LOAD_VULKAN_FUNCTION(vkGetDeviceMemoryCommitment);
	LOAD_VULKAN_FUNCTION(vkBindBufferMemory);
	LOAD_VULKAN_FUNCTION(vkBindImageMemory);
	LOAD_VULKAN_FUNCTION(vkGetBufferMemoryRequirements);
	LOAD_VULKAN_FUNCTION(vkGetImageMemoryRequirements);
	LOAD_VULKAN_FUNCTION(vkGetImageSparseMemoryRequirements);
	LOAD_VULKAN_FUNCTION(vkGetPhysicalDeviceSparseImageFormatProperties);
	LOAD_VULKAN_FUNCTION(vkQueueBindSparse);
	LOAD_VULKAN_FUNCTION(vkCreateFence);
	LOAD_VULKAN_FUNCTION(vkDestroyFence);
	LOAD_VULKAN_FUNCTION(vkResetFences);
	LOAD_VULKAN_FUNCTION(vkGetFenceStatus);
	LOAD_VULKAN_FUNCTION(vkWaitForFences);
	LOAD_VULKAN_FUNCTION(vkCreateSemaphore);
	LOAD_VULKAN_FUNCTION(vkDestroySemaphore);
	LOAD_VULKAN_FUNCTION(vkCreateEvent);
	LOAD_VULKAN_FUNCTION(vkDestroyEvent);
	LOAD_VULKAN_FUNCTION(vkGetEventStatus);
	LOAD_VULKAN_FUNCTION(vkSetEvent);
	LOAD_VULKAN_FUNCTION(vkResetEvent);
	LOAD_VULKAN_FUNCTION(vkCreateQueryPool);
	LOAD_VULKAN_FUNCTION(vkDestroyQueryPool);
	LOAD_VULKAN_FUNCTION(vkGetQueryPoolResults);
	LOAD_VULKAN_FUNCTION(vkCreateBuffer);
	LOAD_VULKAN_FUNCTION(vkDestroyBuffer);
	LOAD_VULKAN_FUNCTION(vkCreateBufferView);
	LOAD_VULKAN_FUNCTION(vkDestroyBufferView);
	LOAD_VULKAN_FUNCTION(vkCreateImage);
	LOAD_VULKAN_FUNCTION(vkDestroyImage);
	LOAD_VULKAN_FUNCTION(vkGetImageSubresourceLayout);
	LOAD_VULKAN_FUNCTION(vkCreateImageView);
	LOAD_VULKAN_FUNCTION(vkDestroyImageView);
	LOAD_VULKAN_FUNCTION(vkCreateShaderModule);
	LOAD_VULKAN_FUNCTION(vkDestroyShaderModule);
	LOAD_VULKAN_FUNCTION(vkCreatePipelineCache);
	LOAD_VULKAN_FUNCTION(vkDestroyPipelineCache);
	LOAD_VULKAN_FUNCTION(vkGetPipelineCacheData);
	LOAD_VULKAN_FUNCTION(vkMergePipelineCaches);
	LOAD_VULKAN_FUNCTION(vkCreateGraphicsPipelines);
	LOAD_VULKAN_FUNCTION(vkCreateComputePipelines);
	LOAD_VULKAN_FUNCTION(vkDestroyPipeline);
	LOAD_VULKAN_FUNCTION(vkCreatePipelineLayout);
	LOAD_VULKAN_FUNCTION(vkDestroyPipelineLayout);
	LOAD_VULKAN_FUNCTION(vkCreateSampler);
	LOAD_VULKAN_FUNCTION(vkDestroySampler);
	LOAD_VULKAN_FUNCTION(vkCreateDescriptorSetLayout);
	LOAD_VULKAN_FUNCTION(vkDestroyDescriptorSetLayout);
	LOAD_VULKAN_FUNCTION(vkCreateDescriptorPool);
	LOAD_VULKAN_FUNCTION(vkDestroyDescriptorPool);
	LOAD_VULKAN_FUNCTION(vkResetDescriptorPool);
	LOAD_VULKAN_FUNCTION(vkAllocateDescriptorSets);
	LOAD_VULKAN_FUNCTION(vkFreeDescriptorSets);
	LOAD_VULKAN_FUNCTION(vkUpdateDescriptorSets);
	LOAD_VULKAN_FUNCTION(vkCreateFramebuffer);
	LOAD_VULKAN_FUNCTION(vkDestroyFramebuffer);
	LOAD_VULKAN_FUNCTION(vkCreateRenderPass);
	LOAD_VULKAN_FUNCTION(vkDestroyRenderPass);
	LOAD_VULKAN_FUNCTION(vkGetRenderAreaGranularity);
	LOAD_VULKAN_FUNCTION(vkCreateCommandPool);
	LOAD_VULKAN_FUNCTION(vkDestroyCommandPool);
	LOAD_VULKAN_FUNCTION(vkResetCommandPool);
	LOAD_VULKAN_FUNCTION(vkAllocateCommandBuffers);
	LOAD_VULKAN_FUNCTION(vkFreeCommandBuffers);
	LOAD_VULKAN_FUNCTION(vkBeginCommandBuffer);
	LOAD_VULKAN_FUNCTION(vkEndCommandBuffer);
	LOAD_VULKAN_FUNCTION(vkResetCommandBuffer);
	LOAD_VULKAN_FUNCTION(vkCmdBindPipeline);
	LOAD_VULKAN_FUNCTION(vkCmdSetViewport);
	LOAD_VULKAN_FUNCTION(vkCmdSetScissor);
	LOAD_VULKAN_FUNCTION(vkCmdSetLineWidth);
	LOAD_VULKAN_FUNCTION(vkCmdSetDepthBias);
	LOAD_VULKAN_FUNCTION(vkCmdSetBlendConstants);
	LOAD_VULKAN_FUNCTION(vkCmdSetDepthBounds);
	LOAD_VULKAN_FUNCTION(vkCmdSetStencilCompareMask);
	LOAD_VULKAN_FUNCTION(vkCmdSetStencilWriteMask);
	LOAD_VULKAN_FUNCTION(vkCmdSetStencilReference);
	LOAD_VULKAN_FUNCTION(vkCmdBindDescriptorSets);
	LOAD_VULKAN_FUNCTION(vkCmdBindIndexBuffer);
	LOAD_VULKAN_FUNCTION(vkCmdBindVertexBuffers);
	LOAD_VULKAN_FUNCTION(vkCmdDraw);
	LOAD_VULKAN_FUNCTION(vkCmdDrawIndexed);
	LOAD_VULKAN_FUNCTION(vkCmdDrawIndirect);
	LOAD_VULKAN_FUNCTION(vkCmdDrawIndexedIndirect);
	LOAD_VULKAN_FUNCTION(vkCmdDispatch);
	LOAD_VULKAN_FUNCTION(vkCmdDispatchIndirect);
	LOAD_VULKAN_FUNCTION(vkCmdCopyBuffer);
	LOAD_VULKAN_FUNCTION(vkCmdCopyImage);
	LOAD_VULKAN_FUNCTION(vkCmdBlitImage);
	LOAD_VULKAN_FUNCTION(vkCmdCopyBufferToImage);
	LOAD_VULKAN_FUNCTION(vkCmdCopyImageToBuffer);
	LOAD_VULKAN_FUNCTION(vkCmdUpdateBuffer);
	LOAD_VULKAN_FUNCTION(vkCmdFillBuffer);
	LOAD_VULKAN_FUNCTION(vkCmdClearColorImage);
	LOAD_VULKAN_FUNCTION(vkCmdClearDepthStencilImage);
	LOAD_VULKAN_FUNCTION(vkCmdClearAttachments);
	LOAD_VULKAN_FUNCTION(vkCmdResolveImage);
	LOAD_VULKAN_FUNCTION(vkCmdSetEvent);
	LOAD_VULKAN_FUNCTION(vkCmdResetEvent);
	LOAD_VULKAN_FUNCTION(vkCmdWaitEvents);
	LOAD_VULKAN_FUNCTION(vkCmdPipelineBarrier);
	LOAD_VULKAN_FUNCTION(vkCmdBeginQuery);
	LOAD_VULKAN_FUNCTION(vkCmdEndQuery);
	LOAD_VULKAN_FUNCTION(vkCmdResetQueryPool);
	LOAD_VULKAN_FUNCTION(vkCmdWriteTimestamp);
	LOAD_VULKAN_FUNCTION(vkCmdCopyQueryPoolResults);
	LOAD_VULKAN_FUNCTION(vkCmdPushConstants);
	LOAD_VULKAN_FUNCTION(vkCmdBeginRenderPass);
	LOAD_VULKAN_FUNCTION(vkCmdNextSubpass);
	LOAD_VULKAN_FUNCTION(vkCmdEndRenderPass);
	LOAD_VULKAN_FUNCTION(vkCmdExecuteCommands);
	LOAD_VULKAN_FUNCTION(vkDestroySurfaceKHR);
	LOAD_VULKAN_FUNCTION(vkGetPhysicalDeviceSurfaceSupportKHR);
	LOAD_VULKAN_FUNCTION(vkGetPhysicalDeviceSurfaceCapabilitiesKHR);
	LOAD_VULKAN_FUNCTION(vkGetPhysicalDeviceSurfaceFormatsKHR);
	LOAD_VULKAN_FUNCTION(vkGetPhysicalDeviceSurfacePresentModesKHR);
	LOAD_VULKAN_FUNCTION(vkCreateSwapchainKHR);
	LOAD_VULKAN_FUNCTION(vkDestroySwapchainKHR);
	LOAD_VULKAN_FUNCTION(vkGetSwapchainImagesKHR);
	LOAD_VULKAN_FUNCTION(vkAcquireNextImageKHR);
	LOAD_VULKAN_FUNCTION(vkQueuePresentKHR);
	LOAD_VULKAN_FUNCTION(vkGetPhysicalDeviceDisplayPropertiesKHR);
	LOAD_VULKAN_FUNCTION(vkGetPhysicalDeviceDisplayPlanePropertiesKHR);
	LOAD_VULKAN_FUNCTION(vkGetDisplayPlaneSupportedDisplaysKHR);
	LOAD_VULKAN_FUNCTION(vkGetDisplayModePropertiesKHR);
	LOAD_VULKAN_FUNCTION(vkCreateDisplayModeKHR);
	LOAD_VULKAN_FUNCTION(vkGetDisplayPlaneCapabilitiesKHR);
	LOAD_VULKAN_FUNCTION(vkCreateDisplayPlaneSurfaceKHR);
	LOAD_VULKAN_FUNCTION(vkCreateSharedSwapchainsKHR);

#ifdef VK_USE_PLATFORM_XLIB_KHR
	LOAD_VULKAN_FUNCTION(vkCreateXlibSurfaceKHR);
	LOAD_VULKAN_FUNCTION(vkGetPhysicalDeviceXlibPresentationSupportKHR);
#endif

#ifdef VK_USE_PLATFORM_XCB_KHR
	LOAD_VULKAN_FUNCTION(vkCreateXcbSurfaceKHR);
		LOAD_VULKAN_FUNCTION(vkGetPhysicalDeviceXcbPresentationSupportKHR);
#endif

#ifdef VK_USE_PLATFORM_WAYLAND_KHR
		LOAD_VULKAN_FUNCTION(vkCreateWaylandSurfaceKHR);
		LOAD_VULKAN_FUNCTION(vkGetPhysicalDeviceWaylandPresentationSupportKHR);
#endif

#ifdef VK_USE_PLATFORM_MIR_KHR
		LOAD_VULKAN_FUNCTION(vkCreateMirSurfaceKHR);
		LOAD_VULKAN_FUNCTION(vkGetPhysicalDeviceMirPresentationSupportKHR);
#endif

#ifdef VK_USE_PLATFORM_ANDROID_KHR
		LOAD_VULKAN_FUNCTION(vkCreateAndroidSurfaceKHR);
#endif

#ifdef VK_USE_PLATFORM_WIN32_KHR
		LOAD_VULKAN_FUNCTION(vkCreateWin32SurfaceKHR);
		LOAD_VULKAN_FUNCTION(vkGetPhysicalDeviceWin32PresentationSupportKHR);
#endif
#ifdef USE_DEBUG_EXTENTIONS
		LOAD_VULKAN_FUNCTION(vkCreateDebugReportCallbackEXT);
		LOAD_VULKAN_FUNCTION(vkDestroyDebugReportCallbackEXT);
		LOAD_VULKAN_FUNCTION(vkDebugReportMessageEXT);
#endif

	return true;
}


// No Vulkan support, do not set function addresses
PFN_vkCreateInstance vkCreateInstance;
PFN_vkDestroyInstance vkDestroyInstance;
PFN_vkEnumeratePhysicalDevices vkEnumeratePhysicalDevices;
PFN_vkGetPhysicalDeviceFeatures vkGetPhysicalDeviceFeatures;
PFN_vkGetPhysicalDeviceFormatProperties vkGetPhysicalDeviceFormatProperties;
PFN_vkGetPhysicalDeviceImageFormatProperties vkGetPhysicalDeviceImageFormatProperties;
PFN_vkGetPhysicalDeviceProperties vkGetPhysicalDeviceProperties;
PFN_vkGetPhysicalDeviceQueueFamilyProperties vkGetPhysicalDeviceQueueFamilyProperties;
PFN_vkGetPhysicalDeviceMemoryProperties vkGetPhysicalDeviceMemoryProperties;
PFN_vkGetInstanceProcAddr vkGetInstanceProcAddr;
PFN_vkGetDeviceProcAddr vkGetDeviceProcAddr;
PFN_vkCreateDevice vkCreateDevice;
PFN_vkDestroyDevice vkDestroyDevice;
PFN_vkEnumerateInstanceExtensionProperties vkEnumerateInstanceExtensionProperties;
PFN_vkEnumerateDeviceExtensionProperties vkEnumerateDeviceExtensionProperties;
PFN_vkEnumerateInstanceLayerProperties vkEnumerateInstanceLayerProperties;
PFN_vkEnumerateDeviceLayerProperties vkEnumerateDeviceLayerProperties;
PFN_vkGetDeviceQueue vkGetDeviceQueue;
PFN_vkQueueSubmit vkQueueSubmit;
PFN_vkQueueWaitIdle vkQueueWaitIdle;
PFN_vkDeviceWaitIdle vkDeviceWaitIdle;
PFN_vkAllocateMemory vkAllocateMemory;
PFN_vkFreeMemory vkFreeMemory;
PFN_vkMapMemory vkMapMemory;
PFN_vkUnmapMemory vkUnmapMemory;
PFN_vkFlushMappedMemoryRanges vkFlushMappedMemoryRanges;
PFN_vkInvalidateMappedMemoryRanges vkInvalidateMappedMemoryRanges;
PFN_vkGetDeviceMemoryCommitment vkGetDeviceMemoryCommitment;
PFN_vkBindBufferMemory vkBindBufferMemory;
PFN_vkBindImageMemory vkBindImageMemory;
PFN_vkGetBufferMemoryRequirements vkGetBufferMemoryRequirements;
PFN_vkGetImageMemoryRequirements vkGetImageMemoryRequirements;
PFN_vkGetImageSparseMemoryRequirements vkGetImageSparseMemoryRequirements;
PFN_vkGetPhysicalDeviceSparseImageFormatProperties vkGetPhysicalDeviceSparseImageFormatProperties;
PFN_vkQueueBindSparse vkQueueBindSparse;
PFN_vkCreateFence vkCreateFence;
PFN_vkDestroyFence vkDestroyFence;
PFN_vkResetFences vkResetFences;
PFN_vkGetFenceStatus vkGetFenceStatus;
PFN_vkWaitForFences vkWaitForFences;
PFN_vkCreateSemaphore vkCreateSemaphore;
PFN_vkDestroySemaphore vkDestroySemaphore;
PFN_vkCreateEvent vkCreateEvent;
PFN_vkDestroyEvent vkDestroyEvent;
PFN_vkGetEventStatus vkGetEventStatus;
PFN_vkSetEvent vkSetEvent;
PFN_vkResetEvent vkResetEvent;
PFN_vkCreateQueryPool vkCreateQueryPool;
PFN_vkDestroyQueryPool vkDestroyQueryPool;
PFN_vkGetQueryPoolResults vkGetQueryPoolResults;
PFN_vkCreateBuffer vkCreateBuffer;
PFN_vkDestroyBuffer vkDestroyBuffer;
PFN_vkCreateBufferView vkCreateBufferView;
PFN_vkDestroyBufferView vkDestroyBufferView;
PFN_vkCreateImage vkCreateImage;
PFN_vkDestroyImage vkDestroyImage;
PFN_vkGetImageSubresourceLayout vkGetImageSubresourceLayout;
PFN_vkCreateImageView vkCreateImageView;
PFN_vkDestroyImageView vkDestroyImageView;
PFN_vkCreateShaderModule vkCreateShaderModule;
PFN_vkDestroyShaderModule vkDestroyShaderModule;
PFN_vkCreatePipelineCache vkCreatePipelineCache;
PFN_vkDestroyPipelineCache vkDestroyPipelineCache;
PFN_vkGetPipelineCacheData vkGetPipelineCacheData;
PFN_vkMergePipelineCaches vkMergePipelineCaches;
PFN_vkCreateGraphicsPipelines vkCreateGraphicsPipelines;
PFN_vkCreateComputePipelines vkCreateComputePipelines;
PFN_vkDestroyPipeline vkDestroyPipeline;
PFN_vkCreatePipelineLayout vkCreatePipelineLayout;
PFN_vkDestroyPipelineLayout vkDestroyPipelineLayout;
PFN_vkCreateSampler vkCreateSampler;
PFN_vkDestroySampler vkDestroySampler;
PFN_vkCreateDescriptorSetLayout vkCreateDescriptorSetLayout;
PFN_vkDestroyDescriptorSetLayout vkDestroyDescriptorSetLayout;
PFN_vkCreateDescriptorPool vkCreateDescriptorPool;
PFN_vkDestroyDescriptorPool vkDestroyDescriptorPool;
PFN_vkResetDescriptorPool vkResetDescriptorPool;
PFN_vkAllocateDescriptorSets vkAllocateDescriptorSets;
PFN_vkFreeDescriptorSets vkFreeDescriptorSets;
PFN_vkUpdateDescriptorSets vkUpdateDescriptorSets;
PFN_vkCreateFramebuffer vkCreateFramebuffer;
PFN_vkDestroyFramebuffer vkDestroyFramebuffer;
PFN_vkCreateRenderPass vkCreateRenderPass;
PFN_vkDestroyRenderPass vkDestroyRenderPass;
PFN_vkGetRenderAreaGranularity vkGetRenderAreaGranularity;
PFN_vkCreateCommandPool vkCreateCommandPool;
PFN_vkDestroyCommandPool vkDestroyCommandPool;
PFN_vkResetCommandPool vkResetCommandPool;
PFN_vkAllocateCommandBuffers vkAllocateCommandBuffers;
PFN_vkFreeCommandBuffers vkFreeCommandBuffers;
PFN_vkBeginCommandBuffer vkBeginCommandBuffer;
PFN_vkEndCommandBuffer vkEndCommandBuffer;
PFN_vkResetCommandBuffer vkResetCommandBuffer;
PFN_vkCmdBindPipeline vkCmdBindPipeline;
PFN_vkCmdSetViewport vkCmdSetViewport;
PFN_vkCmdSetScissor vkCmdSetScissor;
PFN_vkCmdSetLineWidth vkCmdSetLineWidth;
PFN_vkCmdSetDepthBias vkCmdSetDepthBias;
PFN_vkCmdSetBlendConstants vkCmdSetBlendConstants;
PFN_vkCmdSetDepthBounds vkCmdSetDepthBounds;
PFN_vkCmdSetStencilCompareMask vkCmdSetStencilCompareMask;
PFN_vkCmdSetStencilWriteMask vkCmdSetStencilWriteMask;
PFN_vkCmdSetStencilReference vkCmdSetStencilReference;
PFN_vkCmdBindDescriptorSets vkCmdBindDescriptorSets;
PFN_vkCmdBindIndexBuffer vkCmdBindIndexBuffer;
PFN_vkCmdBindVertexBuffers vkCmdBindVertexBuffers;
PFN_vkCmdDraw vkCmdDraw;
PFN_vkCmdDrawIndexed vkCmdDrawIndexed;
PFN_vkCmdDrawIndirect vkCmdDrawIndirect;
PFN_vkCmdDrawIndexedIndirect vkCmdDrawIndexedIndirect;
PFN_vkCmdDispatch vkCmdDispatch;
PFN_vkCmdDispatchIndirect vkCmdDispatchIndirect;
PFN_vkCmdCopyBuffer vkCmdCopyBuffer;
PFN_vkCmdCopyImage vkCmdCopyImage;
PFN_vkCmdBlitImage vkCmdBlitImage;
PFN_vkCmdCopyBufferToImage vkCmdCopyBufferToImage;
PFN_vkCmdCopyImageToBuffer vkCmdCopyImageToBuffer;
PFN_vkCmdUpdateBuffer vkCmdUpdateBuffer;
PFN_vkCmdFillBuffer vkCmdFillBuffer;
PFN_vkCmdClearColorImage vkCmdClearColorImage;
PFN_vkCmdClearDepthStencilImage vkCmdClearDepthStencilImage;
PFN_vkCmdClearAttachments vkCmdClearAttachments;
PFN_vkCmdResolveImage vkCmdResolveImage;
PFN_vkCmdSetEvent vkCmdSetEvent;
PFN_vkCmdResetEvent vkCmdResetEvent;
PFN_vkCmdWaitEvents vkCmdWaitEvents;
PFN_vkCmdPipelineBarrier vkCmdPipelineBarrier;
PFN_vkCmdBeginQuery vkCmdBeginQuery;
PFN_vkCmdEndQuery vkCmdEndQuery;
PFN_vkCmdResetQueryPool vkCmdResetQueryPool;
PFN_vkCmdWriteTimestamp vkCmdWriteTimestamp;
PFN_vkCmdCopyQueryPoolResults vkCmdCopyQueryPoolResults;
PFN_vkCmdPushConstants vkCmdPushConstants;
PFN_vkCmdBeginRenderPass vkCmdBeginRenderPass;
PFN_vkCmdNextSubpass vkCmdNextSubpass;
PFN_vkCmdEndRenderPass vkCmdEndRenderPass;
PFN_vkCmdExecuteCommands vkCmdExecuteCommands;
PFN_vkDestroySurfaceKHR vkDestroySurfaceKHR;
PFN_vkGetPhysicalDeviceSurfaceSupportKHR vkGetPhysicalDeviceSurfaceSupportKHR;
PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR vkGetPhysicalDeviceSurfaceCapabilitiesKHR;
PFN_vkGetPhysicalDeviceSurfaceFormatsKHR vkGetPhysicalDeviceSurfaceFormatsKHR;
PFN_vkGetPhysicalDeviceSurfacePresentModesKHR vkGetPhysicalDeviceSurfacePresentModesKHR;
PFN_vkCreateSwapchainKHR vkCreateSwapchainKHR;
PFN_vkDestroySwapchainKHR vkDestroySwapchainKHR;
PFN_vkGetSwapchainImagesKHR vkGetSwapchainImagesKHR;
PFN_vkAcquireNextImageKHR vkAcquireNextImageKHR;
PFN_vkQueuePresentKHR vkQueuePresentKHR;
PFN_vkGetPhysicalDeviceDisplayPropertiesKHR vkGetPhysicalDeviceDisplayPropertiesKHR;
PFN_vkGetPhysicalDeviceDisplayPlanePropertiesKHR vkGetPhysicalDeviceDisplayPlanePropertiesKHR;
PFN_vkGetDisplayPlaneSupportedDisplaysKHR vkGetDisplayPlaneSupportedDisplaysKHR;
PFN_vkGetDisplayModePropertiesKHR vkGetDisplayModePropertiesKHR;
PFN_vkCreateDisplayModeKHR vkCreateDisplayModeKHR;
PFN_vkGetDisplayPlaneCapabilitiesKHR vkGetDisplayPlaneCapabilitiesKHR;
PFN_vkCreateDisplayPlaneSurfaceKHR vkCreateDisplayPlaneSurfaceKHR;
PFN_vkCreateSharedSwapchainsKHR vkCreateSharedSwapchainsKHR;

#ifdef VK_USE_PLATFORM_XLIB_KHR
PFN_vkCreateXlibSurfaceKHR vkCreateXlibSurfaceKHR;
PFN_vkGetPhysicalDeviceXlibPresentationSupportKHR vkGetPhysicalDeviceXlibPresentationSupportKHR;
#endif

#ifdef VK_USE_PLATFORM_XCB_KHR
PFN_vkCreateXcbSurfaceKHR vkCreateXcbSurfaceKHR;
PFN_vkGetPhysicalDeviceXcbPresentationSupportKHR vkGetPhysicalDeviceXcbPresentationSupportKHR;
#endif

#ifdef VK_USE_PLATFORM_WAYLAND_KHR
PFN_vkCreateWaylandSurfaceKHR vkCreateWaylandSurfaceKHR;
PFN_vkGetPhysicalDeviceWaylandPresentationSupportKHR vkGetPhysicalDeviceWaylandPresentationSupportKHR;
#endif

#ifdef VK_USE_PLATFORM_MIR_KHR
PFN_vkCreateMirSurfaceKHR vkCreateMirSurfaceKHR;
PFN_vkGetPhysicalDeviceMirPresentationSupportKHR vkGetPhysicalDeviceMirPresentationSupportKHR;
#endif

#ifdef VK_USE_PLATFORM_ANDROID_KHR
PFN_vkCreateAndroidSurfaceKHR vkCreateAndroidSurfaceKHR;
#endif

#ifdef VK_USE_PLATFORM_WIN32_KHR
PFN_vkCreateWin32SurfaceKHR vkCreateWin32SurfaceKHR;
PFN_vkGetPhysicalDeviceWin32PresentationSupportKHR vkGetPhysicalDeviceWin32PresentationSupportKHR;
#endif
PFN_vkCreateDebugReportCallbackEXT vkCreateDebugReportCallbackEXT;
PFN_vkDestroyDebugReportCallbackEXT vkDestroyDebugReportCallbackEXT;
PFN_vkDebugReportMessageEXT vkDebugReportMessageEXT;