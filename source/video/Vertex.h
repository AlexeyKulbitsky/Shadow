#ifndef SHADOW_VERTEX_INCLUDE
#define SHADOW_VERTEX_INCLUDE

#include "../math/vec3.h"
#include <vector>

namespace sh
{
	namespace video
	{
		enum class AttributeSemantic
		{
			POSITION = 0,
			COLOR,
			UV,
			NORMAL,
			BINORMAL,
			TANGENT,
			NONE = 0
		};

		enum class AttributeType
		{
			FLOAT = sizeof(f32),
			INT = sizeof(s32),
			UINT = sizeof(u32),
			NONE = 0
		};

		struct Attribute
		{
			Attribute(AttributeSemantic _semantic, AttributeType _type, u32 _componentsCount)
				: semantic(_semantic)
				, type(_type)
				, componentsCount(_componentsCount)
			{}
			
			AttributeSemantic semantic;
			AttributeType type;
			u32 offset;
			u32 componentsCount;
			u32 index;
		};

		class VertexDecalaration
		{
		public:
			VertexDecalaration() {}
			VertexDecalaration(const VertexDecalaration& other)
				: m_attributes(other.m_attributes)
				, m_stride(other.m_stride)
			{
			}

			void AddAttribute(Attribute& attribute)
			{
				attribute.offset = CalculateAttributeOffset(attribute);
				m_attributes.push_back(attribute);
				m_stride += static_cast<u32>(attribute.type) * attribute.componentsCount;
			}
			Attribute* GetAttribute(AttributeSemantic sematic)
			{
				for (auto attr : m_attributes)
				{
					if (attr.semantic == sematic)
						return &attr;
				}
				return nullptr;
			}

			u32 GetStride() const { return m_stride; }
		private:

			u32 CalculateAttributeOffset(const Attribute& attribute)
			{
				u32 offset = 0U;
				for (auto attr : m_attributes)
				{
					offset += static_cast<u32>(attr.type) * attr.componentsCount;
				}
				return offset;
			}

		private:
			std::vector<Attribute> m_attributes;
			u32 m_stride = 0U;
		};

		struct Vertex
		{
			Vertex(){}
			Vertex(f32 x, f32 y, f32 z, f32 r, f32 g, f32 b)
				: pos(x, y, z)
				, color(r, g, b)
			{}
			Vertex(const math::vec3f& p, const math::vec3f& c)
				: pos(p)
				, color(c)
			{}
			math::vec3f pos;
			math::vec3f color;
		};
	}
}
#endif