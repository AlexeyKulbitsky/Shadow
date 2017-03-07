#ifndef SHADOW_DRIVER_INCLUDE
#define SHADOW_DRIVER_INCLUDE

#include "../Globals.h"
#include "Uniform.h"
#include "HardwareBuffer.h"

namespace sh
{
	namespace video
	{
		enum class DriverType
		{
			OPENGL_ES_2_0,
			OPENGL_ES_3_0,
			VULKAN
		};

		class Driver
		{
		public:
			Driver(){}
			~Driver(){}

			virtual bool Init() = 0;
			virtual void BeginRendering() = 0;
			virtual void EndRendering() = 0;
			
			virtual void Render(scene::Model* model);
			virtual void Render(scene::Mesh* mesh);
			virtual void Render(const RenderCommandPtr& command) {}

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


			virtual void GetPixelData(u32 x, u32 y, u32 width, u32 height, u8* data) {}

			u32 GetMaxTextureUnits() const { return m_maxTextureUnits; }
			u32 GetMaxTextureSize() const { return m_maxTextureSize; }
			u32 GetMaxCubeTextureSize() const { return m_maxCubeTextureSize; }

			// Resources creation interface
			virtual VertexBufferPtr CreateVertexBuffer(HardwareBuffer::Usage usage) const = 0;
			virtual IndexBufferPtr CreateIndexBuffer(HardwareBuffer::Usage usage) const = 0;
			virtual UniformBufferPtr CreateUniformBuffer() const = 0;
			virtual RenderCommandPtr CreateRenderCommand() const = 0;
			virtual ShaderProgramPtr CreateShaderProgram() const = 0;
			virtual TexturePtr CreateTexture() const { return nullptr; }
			virtual VertexInputDeclaration* CreateVertexInputDeclaration() const = 0;
			virtual RenderTargetPtr CreateRenderTarget() const { return nullptr; }
			virtual RenderPipelinePtr CreateRenderPipeline() const = 0;
			

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
