#include "GLES20VertexDeclaration.h"

namespace sh
{
	namespace video
	{
		void GLES20VertexDeclaration::Load(const pugi::xml_node &node)
		{
			// All attributes will be loaded as they are declared in technique
			// Othewise they can be overriden while assigning material to loaded mesh
			// In that case this declaration will take offsets and stride from meshe's vetex declaration
			if (node.empty())
				return;

			stride = 0U;
			pugi::xml_node attributeNode = node.first_child();
			while (!attributeNode.empty())
			{
				GLES20VertexAttribute attribute;

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

				if (name == "vec4")
				{
					attribute.type = GL_FLOAT;
					attribute.size = 4;
				}
				else if (name == "vec3")
				{
					attribute.type = GL_FLOAT;
					attribute.size = 3;
				}
				else if (name == "vec2")
				{
					attribute.type = GL_FLOAT;
					attribute.size = 2;
				}
				attribute.pointer = reinterpret_cast<const void*>(stride);
				stride += attribute.size * sizeof(float);

				AddAttribute(attribute);

				attributeNode = attributeNode.next_sibling();
			}
		}

		/////////////////////////////////////////////////////////////////////////////////

		void GLES20VertexDeclaration::Init()
		{
		}

		/////////////////////////////////////////////////////////////////////////////////

		VertexInputDeclarationPtr GLES20VertexDeclaration::Clone()
		{
			GLES20VertexDeclaration* internalDeclaration = new GLES20VertexDeclaration();
			(*internalDeclaration) = (*this);

			VertexInputDeclarationPtr result(internalDeclaration);
			return result;
		}

		/////////////////////////////////////////////////////////////////////////////////

		void GLES20VertexDeclaration::Assemble(VertexDeclaration& declatarion)
		{
			for (size_t i = 0; i < attributes.size(); ++i)
			{
				Attribute* attr = declatarion.GetAttribute(attributes[i].semantic);
				if (!attr)
					continue;
				const void* tempPointer = (const void*)(attr->offset);
				attributes[i].pointer = tempPointer;
			}
			stride = declatarion.GetStride();
		}

		/////////////////////////////////////////////////////////////////////////////////

		GLES20VertexDeclaration& GLES20VertexDeclaration::operator=(const GLES20VertexDeclaration& other)
		{
			attributes = other.attributes;
			stride = other.stride;
			return *this;
		}
	}
}