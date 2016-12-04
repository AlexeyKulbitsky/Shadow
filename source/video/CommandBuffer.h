#ifndef SHADOW_COMMAND_BUFFER_INCLUDE
#define SHADOW_COMMAND_BUFFER_INCLUDE

#include "../Globals.h"

namespace sh
{
	namespace video
	{
		class RenderCommand;
		class RenderState;
		class ShaderProgram;

		class CommandBuffer
		{
		public:
			/*
			This class is used to hold render commands with the same material
			*/
			CommandBuffer() {}
			~CommandBuffer() {}
			virtual void SetRenderState(RenderState* renderState){}
			virtual void SetShaderProgram(ShaderProgram* shaderProgram){}
			virtual void AddCommand(RenderCommand* renderCommand){}
			virtual void RemoveCommand(RenderCommand* renderCommand){}
		};

	}
}
#endif // !SHADOW_COMMAND_BUFFER_INCLUDE
