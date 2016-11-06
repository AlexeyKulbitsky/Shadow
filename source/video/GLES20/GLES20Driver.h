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
			virtual void DrawMesh(scene::Mesh* mesh) override;
			virtual void DrawHardwareBuffer(HardwareBuffer *buffer) const override;
			virtual HardwareBuffer* CreateHardwareBuffer(const HardwareBuffer::CreateInfo& info) const override;
			virtual void InitHardwareBuffer(HardwareBuffer *buffer, const void* verticesPointer, u32 verticesSize, const void* indicesPointer, u32 indicesSize) const override;

			virtual VertexBuffer* CreateVertexBuffer() override;
			virtual VertexBuffer* CreateVertexBuffer(const void* data, size_t size) override;
			virtual IndexBuffer* CreateIndexBuffer() override;
			virtual IndexBuffer* CreateIndexBuffer(const void* data, size_t size) override;

		private:
			EGLContextManager* m_contextManager = nullptr;
		};
	}
}
#endif
