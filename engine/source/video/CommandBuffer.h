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

	struct SHADOW_API CommandBufferDescription
	{
		CommandBufferType type;
	};

	class SHADOW_API CommandBuffer
	{
	public:
		CommandBuffer() {}
		~CommandBuffer() {}

		virtual void Begin() { }
		virtual void End() { }

		static CommandBufferPtr Create(const CommandBufferDescription& description);
	};

} // video

} // sh

#endif // !SHADOW_COMMAND_BUFFER_INCLUDE
