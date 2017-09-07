#include "GLES20VertexDeclaration.h"

namespace sh
{
	namespace video
	{

		void GLES20VertexDeclaration::AddAttribute(const GLES20VertexAttribute& attribute)
		{
			m_attributes.push_back(attribute);
			m_attributes[m_attributes.size() - 1].pointer = reinterpret_cast<const void*>(m_stride);
			m_stride += attribute.size * sizeof(float);
		}

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

		GLES20VertexDeclaration& GLES20VertexDeclaration::operator=(const GLES20VertexDeclaration& other)
		{
			m_attributes = other.m_attributes;
			m_stride = other.m_stride;
			return *this;
		}
	}
}