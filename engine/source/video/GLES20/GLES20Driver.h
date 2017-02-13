#ifndef SHADOW_GLES20_DRIVER_INCLUDE
#define SHADOW_GLES20_DRIVER_INCLUDE

#include "../Driver.h"

namespace sh
{
	namespace video
	{
		class EGLContextManager;

		class GLES20Driver : public Driver 
		{
		public:
			GLES20Driver(){}
			GLES20Driver(EGLContextManager* contextManager);
			virtual ~GLES20Driver(){}

			virtual bool Init() override;
			virtual void BeginRendering() override;
			virtual void EndRendering() override;			
			virtual void Render(RenderCommand* command) override;

			// Viewport management
			virtual void SetViewport(u32 x, u32 y, u32 width, u32 height) override;
			virtual void SetDepthRange(f32 zMin, f32 zMax) override;
			
			// Buffers clearing
			virtual void SetClearColor(const math::Vector4f& clearColor) override;
			virtual void SetClearDepth(f32 clearDepth) override;
			virtual void SetClearStencil(u32 clearStencil) override;

			virtual void ClearColorBuffer() override;
			virtual void ClearDepthBuffer() override;
			virtual void ClearStencilBuffer() override;
			virtual void ClearBuffers() override;

			// State management 
			virtual void SetDepthStencilState(DepthStencilState* depthStencilState) override;
			virtual void SetRasterizationState(RasterizationState* rasterizationState) override;
			virtual void SetBlendingState(BlendingState* blendingState) override;
			
			virtual void GetPixelData(u32 x, u32 y, u32 width, u32 height, u8* data) override;

			virtual VertexBufferPtr CreateVertexBuffer(HardwareBuffer::Usage usage) override;
			virtual IndexBufferPtr CreateIndexBuffer(HardwareBuffer::Usage usage) override;
			virtual UniformBufferPtr CreateUniformBuffer() override;
			virtual RenderCommand* CreateRenderCommand() override;
			virtual ShaderProgramPtr CreateShaderProgram() override;
			virtual TexturePtr CreateTexture() override;
			virtual VertexInputDeclaration* CreateVertexInputDeclaration() override;

		private:
			EGLContextManager* m_contextManager = nullptr;
		};
	}
}
#endif
