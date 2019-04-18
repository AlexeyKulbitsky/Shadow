#include "video/VertexDeclaration.h"


namespace sh
{

namespace video
{

    Attribute::Attribute()
        : semantic(AttributeSemantic::POSITION)
        , type(AttributeType::FLOAT)
        , componentsCount(3)
    {
    }

    Attribute::Attribute(AttributeSemantic _semantic, AttributeType _type, size_t _componentsCount)
        : semantic(_semantic)
        , type(_type)
        , componentsCount(_componentsCount)
    {
    }

    VertexDeclaration::VertexDeclaration() 
    {
    }

    VertexDeclaration::VertexDeclaration(const VertexDeclaration& other)
        : m_attributes(other.m_attributes)
        , m_stride(other.m_stride)
    {
    }

    void VertexDeclaration::AddAttribute(Attribute& attribute)
    {
        attribute.offset = CalculateAttributeOffset(attribute);
        m_attributes.push_back(attribute);
        m_stride += static_cast<size_t>(attribute.type) * attribute.componentsCount;
    }

    Attribute* VertexDeclaration::GetAttribute(AttributeSemantic sematic)
    {
        for (size_t i = 0, sz = m_attributes.size(); i < sz; ++i)
        {
            if (m_attributes[i].semantic == sematic)
                return &m_attributes[i];
        }
        return nullptr;
    }

    size_t VertexDeclaration::GetStride() const 
    { 
        return m_stride; 
    }

    size_t VertexDeclaration::GetAttributesCount() const 
    { 
        return m_attributes.size(); 
    }

    Attribute* VertexDeclaration::GetAttribute(size_t attributeIndex) 
    { 
        return &m_attributes[attributeIndex]; 
    }

    VertexDeclaration& VertexDeclaration::operator=(const VertexDeclaration& other)
    {
        m_attributes = other.m_attributes;
        m_stride = other.m_stride;
        return *this;
    }

    size_t VertexDeclaration::CalculateAttributeOffset(const Attribute& attribute)
    {
        size_t offset = 0U;
        for (auto attr : m_attributes)
        {
            offset += static_cast<size_t>(attr.type) * attr.componentsCount;
        }
        return offset;
    }

} // video

} // sh