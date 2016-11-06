#ifndef SHADOW_MESH_INCLUDE
#define SHADOW_MESH_INCLUDE

#include <vector>
#include "../video/HardwareBuffer.h"
#include "../video/VertexBuffer.h"
#include "../video/IndexBuffer.h"
#include "../Types.h"

namespace sh
{
	namespace video
	{
		class Material;
	}
	namespace scene
	{		
		class Mesh
		{
		public:
			Mesh() {}
			void Init();	

			void SetHardwareBuffer(video::HardwareBuffer* buffer) { m_buffer = buffer; }
			video::HardwareBuffer* GetHardwareBuffer() { return m_buffer; }

			void SetMaterial(video::Material* material) { m_material = material; }
			video::Material* GetMaterial() { return m_material; }

		private:
			video::HardwareBuffer* m_buffer = nullptr;			
			video::Material* m_material = nullptr;
		};
	}
}
#endif