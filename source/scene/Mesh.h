#ifndef SHADOW_MESH_INCLUDE
#define SHADOW_MESH_INCLUDE

#include <vector>
#include "../video/HardwareBuffer.h"
#include "../Types.h"

namespace sh
{
	namespace scene
	{		
		class Mesh
		{
		public:
			Mesh() {}
			void Init(const void* data, u32 size);
			const void* GetVerticesPointer() const { return m_vertices.data(); }
			const u32 GetVerticesCount() const { return m_vertices.size(); }

		private:
			video::HardwareBuffer m_buffer;
			std::vector<float> m_vertices;
			std::vector<u16> m_indices;
		};
	}
}
#endif