#ifndef SHADOW_DRIVER_INCLUDE
#define SHADOW_DRIVER_INCLUDE

#include "../Globals.h"
#include "HardwareBuffer.h"
#include "RenderCommand.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Uniform.h"

namespace sh
{
	namespace scene
	{
		class Mesh;
	}
	namespace video
	{
		struct HardwareBuffer;
		class RenderCommand;
		class ShaderProgram;
		class Texture;

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
			virtual void SetViewport(u32 x, u32 y, u32 width, u32 height) {}

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
		};
	}
}

#endif
