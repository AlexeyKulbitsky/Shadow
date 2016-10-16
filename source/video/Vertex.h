#ifndef SHADOW_VERTEX_INCLUDE
#define SHADOW_VERTEX_INCLUDE

#include "../math/vec3.h"
#include <vector>

namespace sh
{
	namespace video
	{
		enum class AttributeName
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
			FLOAT1 = sizeof(f32),
			FLOAT2 = sizeof(f32) * 2,
			FLOAT3 = sizeof(f32) * 3,
			FLOAT4 = sizeof(f32) * 4,
			NONE = 0
		};

		struct Attribute
		{
			Attribute()
				: type(AttributeType::NONE)
				, name(AttributeName::NONE)
			{}
			Attribute(AttributeType _type, AttributeName _name)
				: type(_type)
				, name(_name)
			{}
			AttributeType type;
			AttributeName name;
			u32 offset;
		};

		class VertexDeclaration
		{
		public:
			VertexDeclaration()
				: m_size(0U)
			{}
			void AddAttribute(Attribute& attribute)
			{
				attribute.offset = CalculateAttributeOffset(attribute);
				m_attributes.push_back(attribute);
				m_size += static_cast<u32>(attribute.type);
			}

		private:
			u32 CalculateAttributeOffset(const Attribute& attribute)
			{
				u32 offset = 0U;
				for (auto attr : m_attributes)
				{
					offset += static_cast<u32>(attr.type);
				}
				return offset;
			}

		private:
			std::vector<Attribute> m_attributes;
			u32 m_size;
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