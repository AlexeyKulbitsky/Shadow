#ifndef SHADOW_GLES20_DRIVER_INCLUDE
#define SHADOW_GLES20_DRIVER_INCLUDE

#include "../../Driver.h"
#include "../GLContext/GLContextManager.h"

namespace sh
{
	namespace video
	{
		class EGLContextManager;

		class GLES20Driver : public Driver 
		{
		public:
			GLES20Driver(){}
			GLES20Driver(GLContextManager* contextManager);
			virtual ~GLES20Driver();
			virtual const String& GetApiName() const override;
			virtual DriverType GetType() const override;

			virtual bool Init() override;
			virtual void BeginRendering() override;
			virtual void EndRendering() override;		

			virtual void SetWindow(void* winId, u32 width, u32 height) override;

			// Viewport management
			virtual void SetViewport(u32 x, u32 y, u32 width, u32 height) override;
			virtual void SetDepthRange(f32 zMin, f32 zMax) override;
			
			// Buffers clearing
			virtual void SetClearColor(const math::Vector4& clearColor) override;
			virtual void SetClearDepth(f32 clearDepth) override;
			virtual void SetClearStencil(u32 clearStencil) override;

			virtual void ClearColorBuffer() override;
			virtual void ClearDepthBuffer() override;
			virtual void ClearStencilBuffer() override;
			virtual void ClearBuffers() override;

			// State management 
			virtual void SetDepthStencilState(const DepthStencilStatePtr& depthStencilState) override;
			virtual void SetRasterizationState(const RasterizationStatePtr& rasterizationState) override;
			virtual void SetBlendingState(const BlendingStatePtr& blendingState) override;
			virtual void SetRenderPipeline(const RenderPipelinePtr& pipeline, const CommandBufferPtr& commandBuffer = nullptr) override;
			virtual void SetComputePipeline() override;
			
			// Rendering
			virtual void SetGpuParams(const GpuParamsPtr& params, const CommandBufferPtr& commandBuffer = nullptr) override;
			virtual void SetTopology(Topology topology, const CommandBufferPtr& commandBuffer = nullptr) override;
			virtual void SetVertexDeclaration(const VertexInputDeclarationPtr& declaration, const CommandBufferPtr& commandBuffer = nullptr) override;
			virtual void SetVertexBuffer(const VertexBufferPtr& buffer, const CommandBufferPtr& commandBuffer = nullptr)override;
			virtual void SetIndexBuffer(const IndexBufferPtr& buffer, const CommandBufferPtr& commandBuffer = nullptr) override;
			virtual void Draw(u32 offset, u32 verticesCount, u32 instancesCount = 1U, const CommandBufferPtr& commandBuffer = nullptr) override;
			virtual void DrawIndexed(u32 offset, u32 indicesCount, u32 instancesCount = 1U, const CommandBufferPtr& commandBuffer = nullptr) override;
			virtual void SetScissorRect(const math::Rect& scissor, const CommandBufferPtr& commandBuffer = nullptr) override;

			virtual void GetPixelData(u32 x, u32 y, u8* data) override;

			virtual RenderTargetPtr CreateRenderTarget() const override;

			void OnWindowResized(int width, int height);

		private:
			GLContextManager* m_contextManager = nullptr;
			GLuint m_currentVertexBuffer;
			GLuint m_currentIndexBuffer;
			GLuint m_currentShaderProgram;
			GLenum m_currentTopology;
		};
	}
}
#endif
