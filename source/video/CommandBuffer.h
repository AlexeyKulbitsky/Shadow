#ifndef SHADOW_COMMAND_BUFFER_INCLUDE
#define SHADOW_COMMAND_BUFFER_INCLUDE

namespace sh
{
	namespace video
	{
		class RenderCommand;

		class CommandBuffer
		{
		public:
			CommandBuffer() {}
			~CommandBuffer() {}
			virtual void Submit(RenderCommand* renderCommand){}
		};

	}
}
#endif // !SHADOW_COMMAND_BUFFER_INCLUDE
