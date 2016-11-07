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
			virtual void DrawHardwareBuffer(HardwareBuffer *buffer) const = 0;
			virtual HardwareBuffer* CreateHardwareBuffer(const HardwareBuffer::CreateInfo& info) const = 0;
			virtual void InitHardwareBuffer(HardwareBuffer *buffer, const void* verticesPointer, u32 verticesSize, const void* indicesPointer, u32 indicesSize) const = 0;

			virtual void Render(RenderCommand* command){}

			// Creates empty vertex buffer
			virtual VertexBuffer* CreateVertexBuffer() = 0;
			// Creates and fills with data vertex buffer
			virtual VertexBuffer* CreateVertexBuffer(const void* data, size_t size) = 0;

			// Creates empty index buffer
			virtual IndexBuffer* CreateIndexBuffer() = 0;
			// Creates and fills with data index buffer
			virtual IndexBuffer* CreateIndexBuffer(const void* data, size_t size) = 0;
		};
	}
}

#endif
