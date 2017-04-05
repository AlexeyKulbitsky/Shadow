#ifndef SHADOW_VULKAN_COMMON_INCLUDE
#define SHADOW_VULKAN_COMMON_INCLUDE

#include "VulkanDeleter.h"

namespace sh
{

namespace video
{

	static VkShaderStageFlagBits const s_vkShaderType[] =
	{
		VK_SHADER_STAGE_VERTEX_BIT,
		VK_SHADER_STAGE_FRAGMENT_BIT,
		VK_SHADER_STAGE_GEOMETRY_BIT,
		VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT,
		VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT,
		VK_SHADER_STAGE_COMPUTE_BIT
	};


} // video

} // sh

#endif
