#ifndef SHADOW_DRIVER_INCLUDE
#define SHADOW_DRIVER_INCLUDE

#include "HardwareBuffer.h"

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
			OPENGL_ES_3_0
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
		};
	}
}

#endif
