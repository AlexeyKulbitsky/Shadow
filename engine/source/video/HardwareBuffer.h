#ifndef SHADOW_HARDWARE_BUFFER_INCLUDE
#define SHADOW_HARDWARE_BUFFER_INCLUDE

#include "../Types.h"
#include "../video/VertexDeclaration.h"
#include <vector>

namespace sh
{
	namespace video
	{
		struct HardwareBuffer
		{
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

			struct CreateInfo
			{
				BufferType bufferType;
				UsageType verticesUsageType;
				UsageType indicesUsageType;			
				Topology topology;
			};
			
			void SetVertexDeclaration(const VertexDeclaration& declaration) { m_vertexDeclaration = declaration; }

			CreateInfo info;
			VertexDeclaration m_vertexDeclaration;
		};
	}
}
#endif