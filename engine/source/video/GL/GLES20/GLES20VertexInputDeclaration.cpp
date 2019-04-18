#include "video/GL/GLES20/GLES20VertexInputDeclaration.h"

namespace sh
{

namespace video
{

    void GLES20VertexInputDeclaration::AddAttribute(const GLES20VertexInputAttribute& attribute)
    {
        m_attributes.push_back(attribute);
        m_attributes[m_attributes.size() - 1].pointer = reinterpret_cast<const void*>(m_stride);
        m_stride += attribute.size * sizeof(float);
    }

    const std::vector<GLES20VertexInputAttribute>& GLES20VertexInputDeclaration::GetAttributes() const
    {
        return m_attributes;
    }

    std::vector<GLES20VertexInputAttribute>& GLES20VertexInputDeclaration::GetAttributes()
    {
        return m_attributes;
    }

    GLsizei GLES20VertexInputDeclaration::GetStride() const
    {
        return m_stride;
    }

    GLES20VertexInputDeclaration& GLES20VertexInputDeclaration::operator=(const GLES20VertexInputDeclaration& other)
    {
        m_attributes = other.m_attributes;
        m_stride = other.m_stride;
        return *this;
    }

    void GLES20VertexInputDeclaration::Init()
    {
    }

    VertexInputDeclarationPtr GLES20VertexInputDeclaration::Clone()
    {
        GLES20VertexInputDeclaration* internalDeclaration = new GLES20VertexInputDeclaration();
        (*internalDeclaration) = (*this);

        VertexInputDeclarationPtr result(internalDeclaration);
        return result;
    }

    void GLES20VertexInputDeclaration::Assemble(VertexDeclaration& declatarion)
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

    uint32_t GLES20VertexInputDeclaration::GetAttributesCount() const 
    { 
        return m_attributes.size(); 
    }

    const InputAttribute& GLES20VertexInputDeclaration::GetAttribute(uint32_t index) const 
    { 
        return m_attributes[index]; 
    }

} // video

} // sh