#ifndef SHADOW_GLES20_COMMAND_CONTEXT_INCLUDE
#define SHADOW_GLES20_COMMAND_CONTEXT_INCLUDE

#include "../CommandContext.h"

namespace sh
{
	namespace video
	{
		class GLES20CommandContext : public CommandContext
		{
		public:
			virtual ~GLES20CommandContext();

			// Set viewport bounds and depth range
			virtual void SetViewPort(u32 minX, u32 minY, f32 minZ, u32 maxX, u32 maxY, f32 maxZ) override;

			// Some variants for sending texture to proper shader (framebuffer binding)
			virtual void SetShaderTexture() override;

			// Send texture to shader
			virtual void SetShaderSampler() override;

			// Send uniform to shader
			virtual void SetShaderParameter(u32 bufferIndex, u32 baseIndex, u32 bytesCount, const void* data) override;

			// Draw primitives without indices
			virtual void DrawPrimitives(Topology topology, u32 baseVertexIndex, u32 primitivesCount, u32 instancesCount) override;

			// Draw primitives indexed
			virtual void DrawIndexedPrimitives(/*IndexBuffer* indexbuffer,*/ Topology topology, u32 primitivesCount) override;

			// Enable/disable depth testing
			virtual void EnableDepthTest(bool enable, f32 minDepth, f32 maxDepth) override;
		};
	}
}

#endif