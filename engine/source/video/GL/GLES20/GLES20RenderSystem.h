#ifndef SHADOW_GLES20_RENDER_SYSTEM_INCLUDE
#define SHADOW_GLES20_RENDER_SYSTEM_INCLUDE

#include "video/GL/GLRenderSystemCommon.h"

namespace sh
{
	namespace video
	{
		class GLES20RenderSystem : public GLRenderSystemCommon
		{
		public:
            GLES20RenderSystem();
			virtual ~GLES20RenderSystem();

			virtual bool Init() override;

			virtual void SetWindow(void* winId, uint32_t width, uint32_t height) override;


			// State management 
			//virtual void SetDepthStencilState(const DepthStencilStatePtr& depthStencilState) override;
			//virtual void SetRasterizationState(const RasterizationStatePtr& rasterizationState) override;
			//virtual void SetBlendingState(const BlendingStatePtr& blendingState) override;
			//virtual void SetRenderPipeline(const RenderPipelinePtr& pipeline, const CommandBufferPtr& commandBuffer = nullptr) override;
			//virtual void SetComputePipeline() override;
			
			// Rendering
			//virtual void SetGpuParams(const GpuParamsPtr& params, const CommandBufferPtr& commandBuffer = nullptr) override;
			//virtual void SetTopology(Topology topology, const CommandBufferPtr& commandBuffer = nullptr) override;
			//virtual void SetVertexDeclaration(const VertexInputDeclarationPtr& declaration, const CommandBufferPtr& commandBuffer = nullptr) override;
			//virtual void SetVertexBuffer(const VertexBufferPtr& buffer, const CommandBufferPtr& commandBuffer = nullptr)override;
			//virtual void SetIndexBuffer(const IndexBufferPtr& buffer, const CommandBufferPtr& commandBuffer = nullptr) override;
			//virtual void Draw(u32 offset, u32 verticesCount, u32 instancesCount = 1U, const CommandBufferPtr& commandBuffer = nullptr) override;
			//virtual void DrawIndexed(u32 offset, u32 indicesCount, u32 instancesCount = 1U, const CommandBufferPtr& commandBuffer = nullptr) override;
			//virtual void SetScissorRect(const math::Rect& scissor, const CommandBufferPtr& commandBuffer = nullptr) override;

			//virtual void GetPixelData(u32 x, u32 y, u8* data) override;

			//virtual RenderTargetPtr CreateRenderTarget() const override;

			//void OnWindowResized(int width, int height);
		};
	}
}
#endif
