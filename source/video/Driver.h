#ifndef SHADOW_DRIVER_INCLUDE
#define SHADOW_DRIVER_INCLUDE

#include "HardwareBuffer.h"
#include "RenderCommand.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

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

			
			virtual VertexBuffer* CreateVertexBuffer() = 0;
			virtual VertexBuffer* CreateVertexBuffer(const void* data, size_t size) = 0;		
			virtual IndexBuffer* CreateIndexBuffer() = 0;
			virtual IndexBuffer* CreateIndexBuffer(const void* data, size_t size) = 0;
			virtual RenderCommand* CreateRenderCommand() = 0;
			virtual ShaderProgram* CreateShaderProgram() = 0;
			virtual Texture* CreateTexture() { return nullptr; }
		};
	}
}

#endif
