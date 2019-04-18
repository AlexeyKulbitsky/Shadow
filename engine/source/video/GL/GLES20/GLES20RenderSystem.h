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

			bool Init() override final;

			// State management 
			void SetDepthStencilState(const DepthStencilStatePtr& depthStencilState) override final;
			void SetRasterizationState(const RasterizationStatePtr& rasterizationState) override final;
			void SetBlendingState(const BlendingStatePtr& blendingState) override final;
			void SetRenderPipeline(const RenderPipelinePtr& pipeline, const CommandBufferPtr& commandBuffer = nullptr) override final;
			void SetComputePipeline() override final;
			
			// Rendering
			//virtual void SetGpuParams(const GpuParamsPtr& params, const CommandBufferPtr& commandBuffer = nullptr) override;
			void SetTopology(Topology topology, const CommandBufferPtr& commandBuffer = nullptr) override final;
			void SetVertexDeclaration(const VertexInputDeclarationPtr& declaration, const CommandBufferPtr& commandBuffer = nullptr) override final;
			void SetVertexBuffer(const VertexBufferPtr& buffer, const CommandBufferPtr& commandBuffer = nullptr) override final;
			void SetIndexBuffer(const IndexBufferPtr& buffer, const CommandBufferPtr& commandBuffer = nullptr) override final;
			void Draw(uint32_t offset, uint32_t verticesCount, uint32_t instancesCount = 1U, const CommandBufferPtr& commandBuffer = nullptr) override final;
			void DrawIndexed(uint32_t offset, uint32_t indicesCount, uint32_t instancesCount = 1U, const CommandBufferPtr& commandBuffer = nullptr) override final;
			//virtual void SetScissorRect(const math::Rect& scissor, const CommandBufferPtr& commandBuffer = nullptr) override;

			//virtual void GetPixelData(u32 x, u32 y, u8* data) override;

			//virtual RenderTargetPtr CreateRenderTarget() const override;

			//void OnWindowResized(int width, int height);
		};
	}
}
#endif
