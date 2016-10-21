#ifndef SHADOW_MESH_INCLUDE
#define SHADOW_MESH_INCLUDE

#include <vector>
#include "../video/HardwareBuffer.h"
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
			void SetVerticesData(const void* data, u32 size);
			void SetIndicesData(const void* data, u32 size);

			const void* GetVerticesPointer() const { return m_vertices.data(); }
			const u32 GetVerticesCount() const { return m_vertices.size(); }

			const void* GetIndicesPointer() const { return m_indices.data(); }
			const u32 GetIndicesCount() const { return m_indices.size(); }

			void SetHardwareBuffer(video::HardwareBuffer* buffer) { m_buffer = buffer; }
			video::HardwareBuffer* GetHardwareBuffer() { return m_buffer; }

			void SetMaterial(video::Material* material) { m_material = material; }
			video::Material* GetMaterial() { return m_material; }

		private:
			video::HardwareBuffer* m_buffer = nullptr;
			std::vector<f32> m_vertices;
			std::vector<u16> m_indices;

			video::Material* m_material = nullptr;
		};
	}
}
#endif