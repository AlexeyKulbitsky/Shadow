#ifndef SHADOW_VULKAN_VERTEX_DECLARATION_INCLUDE
#define SHADOW_VULKAN_VERTEX_DECLARATION_INCLUDE

#include <vector>
#include "../VertexDeclaration.h"
#include "VulkanDeleter.h"

namespace sh
{
	namespace video
	{
		struct VulkanVertexAttribute
		{
			VkVertexInputAttributeDescription vulkanAttribute;
			AttributeSemantic semantic;
			std::string name;
		};

		class VKVertexDeclaration
		{			
		public:
			// Add attribute to engines structure
			void AddAttribute(const VulkanVertexAttribute& attribute)
			{
				m_attributes.push_back(attribute);
			}

			// Get Vulkan Internal attributes
			const std::vector<VkVertexInputAttributeDescription>& GetVulkanAttributes() const { return m_vulkanAttributes; }
			const VkVertexInputAttributeDescription* GetVulkanAttributesPointer() const { return m_vulkanAttributes.data(); }
			size_t GetVulkanAttributesCount() const { return m_vulkanAttributes.size(); }

			// Connect mesh attributes with vulkan attributes presentation
			void Assemble(VertexDeclaration declaration)
			{
				m_descriptor.binding = 0U;
				m_descriptor.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
				m_descriptor.stride = declaration.GetStride();

				m_vulkanAttributes.clear();
				size_t attrCount = m_attributes.size();
				for (size_t i = 0; i < attrCount; ++i)
				{					
					Attribute* attribute = declaration.GetAttribute(m_attributes[i].semantic);
					m_attributes[i].vulkanAttribute.offset = attribute->offset;
					m_attributes[i].vulkanAttribute.binding = m_descriptor.binding;

					m_vulkanAttributes.push_back(m_attributes[i].vulkanAttribute);
				}			
			}

		private:
			VkVertexInputBindingDescription m_descriptor;
			std::vector<VkVertexInputAttributeDescription> m_vulkanAttributes;
			std::vector<VulkanVertexAttribute> m_attributes;
		};
	}
}

#endif // !SHADOW_VULKAN_VERTEX_DECLARATION_INCLUDE
