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

		class VulkanVertexDeclaration : public VertexInputDeclaration
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

			VulkanVertexDeclaration& operator=(const VulkanVertexDeclaration& other);

			virtual void Load(const pugi::xml_node &node) override;
			virtual void SetShaderProgram(ShaderProgram* shaderProgram) override;
			virtual void Init() override;
			virtual VertexInputDeclarationPtr Clone() override;
			virtual void Assemble(VertexDeclaration& declaration) override;

			bool operator == (const VulkanVertexDeclaration& other);

		private:
			VkVertexInputBindingDescription m_descriptor;
			std::vector<VkVertexInputAttributeDescription> m_vulkanAttributes;
			std::vector<VulkanVertexAttribute> m_attributes;
			u32 m_id = 0U;

			static std::vector<VulkanVertexDeclaration*> s_declarations;
		};
	}
}

#endif // !SHADOW_VULKAN_VERTEX_DECLARATION_INCLUDE
