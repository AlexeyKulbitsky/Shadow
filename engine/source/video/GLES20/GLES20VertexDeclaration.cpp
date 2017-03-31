#include "GLES20VertexDeclaration.h"
#include "GLES20ShaderProgram.h"

namespace sh
{
	namespace video
	{
		void GLES20VertexDeclaration::Load(const pugi::xml_node &node)
		{
			if (node.empty())
				return;

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

				if (name == "vec3")
				{
					attribute.type = GL_FLOAT;
					attribute.size = 3;
				}
				else if (name == "vec2")
				{
					attribute.type = GL_FLOAT;
					attribute.size = 2;
				}

				AddAttribute(attribute);

				attributeNode = attributeNode.next_sibling();
			}
		}

		/////////////////////////////////////////////////////////////////////////////////

		void GLES20VertexDeclaration::SetShaderProgram(ShaderProgram* shaderProgram)
		{
			m_shaderProgram = static_cast<GLES20ShaderProgram*>(shaderProgram);
		}

		/////////////////////////////////////////////////////////////////////////////////

		void GLES20VertexDeclaration::Init()
		{
			m_shaderProgram->BindProgram();
			for (u32 i = 0; i < attributes.size(); ++i)
			{
				attributes[i].index = glGetAttribLocation(m_shaderProgram->GetGLId(), attributes[i].name.c_str());
			}
			m_shaderProgram->UnbindProgram();
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