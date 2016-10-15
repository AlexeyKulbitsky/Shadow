#ifndef SHADOW_HARDWARE_BUFFER_INCLUDE
#define SHADOW_HARDWARE_BUFFER_INCLUDE

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

			UsageType verticesUsageType = UsageType::NONE;
			UsageType indicesUsageType = UsageType::NONE;	
		};
	}
}
#endif