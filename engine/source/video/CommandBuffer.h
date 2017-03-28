#ifndef SHADOW_COMMAND_BUFFER_INCLUDE
#define SHADOW_COMMAND_BUFFER_INCLUDE

#include "../Globals.h"

namespace sh
{
	namespace video
	{
		class Command
		{
		public:
			virtual void Execute() = 0;
		};

		class CommandBuffer
		{
		public:
			CommandBuffer() {}
			~CommandBuffer() {}
		};

	}
}
#endif // !SHADOW_COMMAND_BUFFER_INCLUDE
