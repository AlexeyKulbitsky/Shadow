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

		class VKVertexDeclaration : public VertexInputDeclaration
		{			
		public:
			// Add attribute to engines structure
			void AddAttribute(const VulkanVertexAttribute& attribute)
			{
				m_attributes.push_back(attribute);
			}

			// Get Vulkan Internal attributes
			const VkVertexInputBindingDescription& GetBindingDescription() const { return m_descriptor; }
			const std::vector<VkVertexInputAttributeDescription>& GetVulkanAttributes() const { return m_vulkanAttributes; }
			const VkVertexInputAttributeDescription* GetVulkanAttributesPointer() const { return m_vulkanAttributes.data(); }
			size_t GetVulkanAttributesCount() const { return m_vulkanAttributes.size(); }

			VKVertexDeclaration& operator=(const VKVertexDeclaration& other);

			virtual void Load(const pugi::xml_node &node) override;
			virtual void SetShaderProgram(ShaderProgram* shaderProgram) override;
			virtual void Init() override;
			virtual VertexInputDeclaration* Clone() override;
			virtual void Assemble(VertexDeclaration& declaration) override;

		private:
			VkVertexInputBindingDescription m_descriptor;
			std::vector<VkVertexInputAttributeDescription> m_vulkanAttributes;
			std::vector<VulkanVertexAttribute> m_attributes;
		};
	}
}

#endif // !SHADOW_VULKAN_VERTEX_DECLARATION_INCLUDE
