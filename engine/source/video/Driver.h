#ifndef SHADOW_DRIVER_INCLUDE
#define SHADOW_DRIVER_INCLUDE

#include "../Globals.h"
#include "Uniform.h"
#include "HardwareBuffer.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "Shader.h"
#include "Texture.h"
#include "RenderPipeline.h"
#include "Sampler.h"
#include "CommandBuffer.h"

namespace sh
{
	namespace video
	{
		enum class DriverType
		{
			OPENGL_ES_2_0,
			OPENGL_ES_3_0,
			DIRECTX_11,
			VULKAN
		};

		struct RenderPipelineDescription;

		class Driver
		{
		public:
			Driver(){}
			~Driver(){}

			virtual const String& GetApiName() const = 0;

			virtual bool Init() = 0;
			virtual void BeginRendering() = 0;
			virtual void EndRendering() = 0;
			
			virtual void Render(scene::Model* model);
			virtual void Render(scene::Mesh* mesh);
			//virtual void Render(const RenderCommandPtr& command) {};

			// Viewport management
			virtual void SetViewport(u32 x, u32 y, u32 width, u32 height);
			virtual const math::Vector4u& GetViewPort() const { return m_viewPort; }
			virtual void SetDepthRange(f32 zMin, f32 zMax);
			virtual const math::Vector2f& GetDepthRange() const { return m_depthRange; }
			
			// Buffers clearing
			virtual void SetClearColor(const math::Vector4f& clearColor) { m_clearColor = clearColor; }
			virtual void SetClearDepth(f32 clearDepth) { m_clearDepth = clearDepth; }
			virtual void SetClearStencil(u32 clearStencil) { m_clearStencil = clearStencil; }
			const math::Vector4f& GetClearColor() const { return m_clearColor; }
			f32 GetClearDepth() const { return m_clearDepth; }
			u32 GetClearStencil() const { return m_clearStencil; }

			virtual void ClearColorBuffer() { }
			virtual void ClearDepthBuffer() { }
			virtual void ClearStencilBuffer() { }
			virtual void ClearBuffers() { }

			// State management 
			virtual void SetDepthStencilState(const DepthStencilStatePtr& depthStencilState) { }
			virtual void SetRasterizationState(const RasterizationStatePtr& rasterizationState) { }
			virtual void SetBlendingState(const BlendingStatePtr& blendingState) { }
			virtual void SetRenderPipeline(const RenderPipelinePtr& pipeline, const CommandBufferPtr& commandBuffer = nullptr) { }
			virtual void SetComputePipeline() { }

			// Rendering
			virtual void SetGpuParams(const GpuParamsPtr& params, const CommandBufferPtr& commandBuffer = nullptr) { }
			virtual void SetTopology(Topology topology, const CommandBufferPtr& commandBuffer = nullptr) { }
			virtual void SetAutoUniformsBatch(const UniformsBatchPtr& batch, const CommandBufferPtr& commandBuffer = nullptr) { }
			virtual void SetVertexDeclaration(const VertexInputDeclarationPtr& declaration, const CommandBufferPtr& commandBuffer = nullptr) { }
			virtual void SetVertexBuffer(const VertexBufferPtr& buffer, const CommandBufferPtr& commandBuffer = nullptr) { }
			virtual void SetIndexBuffer(const IndexBufferPtr& buffer, const CommandBufferPtr& commandBuffer = nullptr) { }
			virtual void Draw(u32 offset, u32 verticesCount, u32 instancesCount = 1U, const CommandBufferPtr& commandBuffer = nullptr) { }
			virtual void DrawIndexed(u32 offset, u32 indicesCount, u32 instancesCount = 1U, const CommandBufferPtr& commandBuffer = nullptr) { }

			virtual void GetPixelData(u32 x, u32 y, u32 width, u32 height, u8* data) {}

			u32 GetMaxTextureUnits() const { return m_maxTextureUnits; }
			u32 GetMaxTextureSize() const { return m_maxTextureSize; }
			u32 GetMaxCubeTextureSize() const { return m_maxCubeTextureSize; }

			// Resources creation interface
			virtual UniformBufferPtr CreateUniformBuffer() const {return nullptr;}
			virtual RenderTargetPtr CreateRenderTarget() const { return nullptr; }

			template<typename T>
			void SetGlobalUniform(GlobalUniformName globalName, const T& value);
			Uniform* GetGlobalUniform(GlobalUniformName globalName) { return m_globalUniforms[(size_t)globalName]; }

		protected:
			void InitGlobalUniforms();

		protected:
			Uniform* m_globalUniforms[(size_t)GlobalUniformName::COUNT];
			math::Vector4u m_viewPort;
			math::Vector2f m_depthRange;
			
			// Clear values
			math::Vector4f m_clearColor;
			f32 m_clearDepth;
			u32 m_clearStencil;

			// Global info
			u32 m_maxTextureUnits;
			u32 m_maxTextureSize;
			u32 m_maxCubeTextureSize;
		};
	}
}

#endif
