#ifndef SHADOW_COMMAND_CONTEXT_INCLUDE
#define SHADOW_COMMAND_CONTEXT_INCLUDE

#include "../Globals.h"

namespace sh
{
	namespace video
	{
		class CommandContext
		{
		public:
			virtual ~CommandContext() {}

			// Set viewport bounds and depth range
			virtual void SetViewPort(u32 minX,u32 minY, f32 minZ, u32 maxX, u32 maxY,f32 maxZ) = 0;

			// Some variants for sending texture to proper shader (framebuffer binding)
			virtual void SetShaderTexture() = 0;

			// Send texture to shader
			virtual void SetShaderSampler() = 0;

			// Send uniform to shader
			virtual void SetShaderParameter(u32 bufferIndex, u32 baseIndex, u32 bytesCount, const void* data) = 0;

			// Draw primitives without indices
			virtual void DrawPrimitives(u32 primitiveType, u32 baseVertexIndex, u32 primitivesCount, u32 instancesCount) = 0;

			// Draw primitives indexed
			virtual void DrawIndexedPrimitives(/*IndexBuffer* indexbuffer,*/ u32 primitiveType, u32 primitivesCount) = 0;

			// Enable/disable depth testing
			virtual void EnableDepthTest(bool enable, f32 minDepth, f32 maxDepth) = 0;
		};
	}
}

#endif