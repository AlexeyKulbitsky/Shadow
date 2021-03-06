#include "VulkanVertexDeclaration.h"

namespace sh
{
	namespace video
	{

		VulkanVertexDeclaration& VulkanVertexDeclaration::operator=(const VulkanVertexDeclaration& other)
		{
			m_descriptor = other.m_descriptor;
			m_vulkanAttributes = other.m_vulkanAttributes;
			m_attributes = other.m_attributes;
			return *this;
		}

		/////////////////////////////////////////////////////////////////////////////////

		void VulkanVertexDeclaration::Load(const pugi::xml_node &node)
		{
			if (node.empty())
				return;

			pugi::xml_node attributeNode = node.first_child();
			while (!attributeNode.empty())
			{
				VulkanVertexAttribute attribute;

				std::string name = attributeNode.name();
				if (name == "position")
				{
					attribute.semantic = AttributeSemantic::POSITION;
					printf("Postion attribute\n");
				}
				else if (name == "normal")
				{
					attribute.semantic = AttributeSemantic::NORMAL;
					printf("Normal attribute\n");
				}
				else if (name == "color")
				{
					attribute.semantic = AttributeSemantic::COLOR;
					printf("Color attribute\n");
				}
				else if (name == "uv")
				{
					attribute.semantic = AttributeSemantic::UV;
					printf("UV attribute\n");
				}
				else
				{
					printf("Unknown attribute\n");
				}

				attribute.name = attributeNode.attribute("name").as_string();

				pugi::xml_attribute typeAttr = attributeNode.attribute("type");
				name = typeAttr.as_string();

				pugi::xml_attribute locationAttr = attributeNode.attribute("location");
				u32 location = locationAttr.as_uint();

				if (name == "vec3")
				{
					attribute.vulkanAttribute.location = location;
					attribute.vulkanAttribute.format = VK_FORMAT_R32G32B32_SFLOAT;				
				}
				else if (name == "vec2")
				{
					attribute.vulkanAttribute.location = location;
					attribute.vulkanAttribute.format = VK_FORMAT_R32G32_SFLOAT;
				}

				AddAttribute(attribute);
				attributeNode = attributeNode.next_sibling();
			}		
		}

		/////////////////////////////////////////////////////////////////////////////////

		void VulkanVertexDeclaration::SetShaderProgram(ShaderProgram*)
		{
		}

		/////////////////////////////////////////////////////////////////////////////////

		void VulkanVertexDeclaration::Init()
		{
		}

		/////////////////////////////////////////////////////////////////////////////////

		VertexInputDeclarationPtr VulkanVertexDeclaration::Clone()
		{
			VulkanVertexDeclaration* internalDeclaration = new VulkanVertexDeclaration();
			(*internalDeclaration) = (*this);
			VertexInputDeclarationPtr result(internalDeclaration);
			return result;
		}

		/////////////////////////////////////////////////////////////////////////////////

		void VulkanVertexDeclaration::Assemble(VertexDeclaration& declaration)
		{
			// Init descriptor telling Vulkan what package of attributes to use
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

		/////////////////////////////////////////////////////////////////////////////////

		bool VulkanVertexDeclaration::operator == (const VulkanVertexDeclaration& other)
		{
			// Compare descriptors
			bool descriptorsEqual = 
				m_descriptor.binding == other.m_descriptor.binding &&
				m_descriptor.stride == other.m_descriptor.stride &&
				m_descriptor.inputRate == other.m_descriptor.inputRate;
			if (!descriptorsEqual)
				return false;

			// Compare Vulkan attributes
			if(m_vulkanAttributes.size() != other.m_vulkanAttributes.size())
				return false;

			for( size_t i = 0U, sz = m_vulkanAttributes.size(); i < sz; ++i )
			{
				bool attributesEqual = 
					m_vulkanAttributes[i].binding == other.m_vulkanAttributes[i].binding &&
					m_vulkanAttributes[i].format == other.m_vulkanAttributes[i].format &&
					m_vulkanAttributes[i].location == other.m_vulkanAttributes[i].location &&
					m_vulkanAttributes[i].offset == other.m_vulkanAttributes[i].offset;
				if(!attributesEqual)
					return false;
			}

			return true;
		}

		/////////////////////////////////////////////////////////////////////////////////
	}
}