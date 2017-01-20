#include "GLES20CommandContext.h"
#include "../GLContext/EGLContextManager.h"

namespace sh
{
	namespace video
	{
		GLES20CommandContext::~GLES20CommandContext()
		{

		}

		// Set viewport bounds and depth range
		void GLES20CommandContext::SetViewPort(u32 minX, u32 minY, f32 minZ, u32 maxX, u32 maxY, f32 maxZ)
		{
			glViewport(minX, minY, maxX - minX, maxY - minY);
		}

		// Some variants for sending texture to proper shader (framebuffer binding)
		void GLES20CommandContext::SetShaderTexture()
		{

		}

		// Send texture to shader
		void GLES20CommandContext::SetShaderSampler()
		{

		}

		// Send uniform to shader
		void GLES20CommandContext::SetShaderParameter(u32 bufferIndex, u32 baseIndex, u32 bytesCount, const void* data)
		{

		}

		// Draw primitives without indices
		void GLES20CommandContext::DrawPrimitives(Topology topology, u32 baseVertexIndex, u32 primitivesCount, u32 instancesCount)
		{
			//glDrawArrays(primitiveType, baseVertexIndex, primitivesCount);
		}

		// Draw primitives indexed
		void GLES20CommandContext::DrawIndexedPrimitives(/*IndexBuffer* indexbuffer,*/ Topology topology, u32 primitivesCount)
		{

		}

		// Enable/disable depth testing
		void GLES20CommandContext::EnableDepthTest(bool enable, f32 minDepth, f32 maxDepth)
		{

		}
	}
}