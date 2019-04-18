#ifndef SHADOW_VERTEX_DECLARATION_INCLUDE
#define SHADOW_VERTEX_DECLARATION_INCLUDE

#include "prerequisities/Prerequisities.h"
#include "video/Common.h"

namespace sh
{

namespace video
{
		
    struct SHADOW_API Attribute
    {
        Attribute();
        Attribute(AttributeSemantic _semantic, AttributeType _type, size_t _componentsCount);

        AttributeSemantic semantic;
        AttributeType type;
        // Attribute offset (in bytes)
        size_t offset;
        size_t componentsCount;
    };

    class SHADOW_API VertexDeclaration
    {
    public:
        VertexDeclaration();
        VertexDeclaration(const VertexDeclaration& other);

        void AddAttribute(Attribute& attribute);
        Attribute* GetAttribute(AttributeSemantic sematic);

        size_t GetStride() const;
        size_t GetAttributesCount() const;
        Attribute* GetAttribute(size_t attributeIndex);

        VertexDeclaration& operator=(const VertexDeclaration& other);

    private:
        size_t CalculateAttributeOffset(const Attribute& attribute);

    private:
        std::vector<Attribute> m_attributes;
        Attribute m_attribs[(size_t)AttributeSemantic::COUNT];
        size_t m_stride = 0U;
    };

} // video

} // sh

#endif // !SHADOW_VERTEX_DECLARATION_INCLUDE
