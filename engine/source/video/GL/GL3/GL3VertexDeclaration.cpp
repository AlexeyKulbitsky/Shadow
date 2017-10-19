#include "GL3VertexDeclaration.h"

namespace sh
{
	namespace video
	{

		void GL3VertexDeclaration::AddAttribute(const GL3VertexAttribute& attribute)
		{
			m_attributes.push_back(attribute);
			m_attributes[m_attributes.size() - 1].pointer = reinterpret_cast<const void*>(m_stride);
			m_stride += attribute.size * sizeof(float);
		}

		void GL3VertexDeclaration::Init()
		{
		}

		/////////////////////////////////////////////////////////////////////////////////

		VertexInputDeclarationPtr GL3VertexDeclaration::Clone()
		{
			GL3VertexDeclaration* internalDeclaration = new GL3VertexDeclaration();
			(*internalDeclaration) = (*this);

			VertexInputDeclarationPtr result(internalDeclaration);
			return result;
		}

		/////////////////////////////////////////////////////////////////////////////////

		void GL3VertexDeclaration::Assemble(VertexDeclaration& declatarion)
		{
			for (size_t i = 0; i < m_attributes.size(); ++i)
			{
				Attribute* attr = declatarion.GetAttribute(m_attributes[i].semantic);
				if (!attr)
					continue;
				const void* tempPointer = (const void*)(attr->offset);
				m_attributes[i].pointer = tempPointer;
			}
			m_stride = declatarion.GetStride();
		}

		/////////////////////////////////////////////////////////////////////////////////

		GL3VertexDeclaration& GL3VertexDeclaration::operator=(const GL3VertexDeclaration& other)
		{
			m_attributes = other.m_attributes;
			m_stride = other.m_stride;
			return *this;
		}
	}
}
