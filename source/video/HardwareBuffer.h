#ifndef SHADOW_HARDWARE_BUFFER_INCLUDE
#define SHADOW_HARDWARE_BUFFER_INCLUDE

#include "../Types.h"
#include <vector>

namespace sh
{
	namespace video
	{
		struct HardwareBuffer
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
				FLOAT1 = sizeof(f32),
				FLOAT2 = sizeof(f32) * 2,
				FLOAT3 = sizeof(f32) * 3,
				FLOAT4 = sizeof(f32) * 4,
				NONE = 0
			};

			enum class UsageType
			{
				NONE = 0,
				STATIC,
				DYNAMIC,
				STREAM
			};

			enum class BufferType
			{
				NONE = 0,
				VERTEX,
				INDEX,
				VERTEX_AND_INDEX
			};

			enum class Topology
			{
				LINE_LIST = 0,
				LINE_LOOP,
				LINE_STRIP,
				TRIANGLE_LIST,
				TRIANGLE_STRIP,
				TRIANGLE_FAN
			};

			struct Attribute
			{
				Attribute(AttributeType _type, AttributeSemantic _semantic)
					: type(_type)
					, semantic(_semantic)
				{}
				AttributeType type;
				AttributeSemantic semantic;
				u32 offset;
			};

			void AddAttribute(Attribute& attribute)
			{
				attribute.offset = CalculateAttributeOffset(attribute);
				attributes.push_back(attribute);
				stride += static_cast<u32>(attribute.type);
			}

			u32 CalculateAttributeOffset(const Attribute& attribute)
			{
				u32 offset = 0U;
				for (auto attr : attributes)
				{
					offset += static_cast<u32>(attr.type);
				}
				return offset;
			}

			UsageType verticesUsageType = UsageType::NONE;
			UsageType indicesUsageType = UsageType::NONE;	
			BufferType type = BufferType::NONE;
			Topology topology;

			std::vector<Attribute> attributes;
			u32 stride = 0U;
		};
	}
}
#endif