#ifndef SHADOW_VULKAN_COMMON_INCLUDE
#define SHADOW_VULKAN_COMMON_INCLUDE

#include "VulkanDeleter.h"

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

	static VkShaderStageFlagBits const s_vkShaderType[] =
	{
		VK_SHADER_STAGE_VERTEX_BIT,
		VK_SHADER_STAGE_FRAGMENT_BIT,
		VK_SHADER_STAGE_GEOMETRY_BIT,
		VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT,
		VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT,
		VK_SHADER_STAGE_COMPUTE_BIT
	};

	static VkCommandBufferLevel const s_vkCommandBufferType[] =
	{
		VK_COMMAND_BUFFER_LEVEL_PRIMARY,
		VK_COMMAND_BUFFER_LEVEL_SECONDARY
	};

	static VkFilter const s_vkTextureFiltering[] =
	{
		VK_FILTER_NEAREST,
		VK_FILTER_LINEAR
	};

	static VkSamplerMipmapMode const s_vkMipFileting[] =
	{
		VK_SAMPLER_MIPMAP_MODE_NEAREST,
		VK_SAMPLER_MIPMAP_MODE_LINEAR
	};
	
	static VkSamplerAddressMode const s_vkTiling[] =
	{
		VK_SAMPLER_ADDRESS_MODE_REPEAT,
		VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT,
		VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE
	};

} // video

} // sh

#endif
