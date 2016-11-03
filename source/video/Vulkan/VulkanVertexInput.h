#ifndef SHADOW_VULKAN_VERTEX_INPUT
#define SHADOW_VULKAN_VERTEX_INPUT

#include "VulkanDeleter.h"
#include <array>
#include "../../math/smath.h"
namespace sh
{
	namespace video
	{
		struct VulkanVertexDeclaration
		{
			struct Vertex
			{
				math::vec3f pos;
				math::vec3f color;
				math::vec2f texCoord;
			};

			static VkVertexInputBindingDescription getBindingDescription() 
			{
				VkVertexInputBindingDescription bindingDescription = {};
				bindingDescription.binding = 0;
				bindingDescription.stride = sizeof(Vertex);
				bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
				return bindingDescription;
			}

			static std::array<VkVertexInputAttributeDescription, 3> getAttributeDescriptions() 
			{
				std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions = {};

				attributeDescriptions[0].binding = 0;
				attributeDescriptions[0].location = 0;
				attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
				attributeDescriptions[0].offset = offsetof(Vertex, pos);

				attributeDescriptions[1].binding = 0;
				attributeDescriptions[1].location = 1;
				attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
				attributeDescriptions[1].offset = offsetof(Vertex, color);

				attributeDescriptions[2].binding = 0;
				attributeDescriptions[2].location = 2;
				attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
				attributeDescriptions[2].offset = offsetof(Vertex, texCoord);

				return attributeDescriptions;
			}
		};
	}
}

#endif
