#ifndef SHADOW_DRIVER_INCLUDE
#define SHADOW_DRIVER_INCLUDE

#include "../Globals.h"
#include "RenderCommand.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Uniform.h"

namespace sh
{
	namespace scene
	{
		class Mesh;
		class Model;
	}
	namespace video
	{
		class RenderCommand;
		class ShaderProgram;
		class Texture;
		struct DepthStencilState;
		struct RasterizationState;
		struct BlendingState;

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
			virtual void DrawMesh(scene::Mesh* mesh) = 0;
			virtual void Render(RenderCommand* command){}
			virtual void Render(scene::Model* model);

			// Viewport management
			virtual void SetViewport(u32 x, u32 y, u32 width, u32 height);
			virtual const math::Vector4u& GetViewPort() const { return m_viewPort; }
			virtual void SetDepthRange(f32 zMin, f32 zMax);
			virtual const math::Vector2f& GetDepthRange() const { return m_depthRange; }
			
			// Buffers clearing
			void SetClearColor(const math::Vector4f& clearColor) { m_clearColor = clearColor; }
			void SetClearDepth(f32 clearDepth) { m_clearDepth = clearDepth; }
			void SetClearStencil(u32 clearStencil) { m_clearStencil = clearStencil; }
			const math::Vector4f& GetClearColor() const { return m_clearColor; }
			f32 GetClearDepth() const { return m_clearDepth; }
			u32 GetClearStencil() const { return m_clearStencil; }

			virtual void ClearColorBuffer() { }
			virtual void ClearDepthBuffer() { }
			virtual void ClearStencilBuffer() { }
			virtual void ClearBuffers() { }

			// State management 
			virtual void SetDepthStencilState(DepthStencilState* depthStencilState) { }
			virtual void SetRasterizationState(RasterizationState* rasterizationState) { }
			virtual void SetBlendingState(BlendingState* blendingState) { }


			virtual void GetPixelData(u32 x, u32 y, u32 width, u32 height, u8* data) {}

			u32 GetMaxTextureUnits() const { return m_maxTextureUnits; }
			u32 GetMaxTextureSize() const { return m_maxTextureSize; }
			u32 GetMaxCubeTextureSize() const { return m_maxCubeTextureSize; }

			// Resources creation interface
			virtual VertexBuffer* CreateVertexBuffer() = 0;
			virtual VertexBuffer* CreateVertexBuffer(const void* data, size_t size) = 0;		
			virtual IndexBuffer* CreateIndexBuffer() = 0;
			virtual IndexBuffer* CreateIndexBuffer(const void* data, size_t size) = 0;
			virtual UniformBuffer* CreateUniformBuffer() { return nullptr; }
			virtual RenderCommand* CreateRenderCommand() = 0;
			virtual ShaderProgram* CreateShaderProgram() = 0;
			virtual Texture* CreateTexture() { return nullptr; }
			virtual VertexInputDeclaration* CreateVertexInputDeclaration() { return nullptr; }
			virtual RenderState* CreateRenderState() { return nullptr; }

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
