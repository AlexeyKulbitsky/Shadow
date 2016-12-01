#ifndef SHADOW_MODEL_INCLUDE
#define SHADOW_MODEL_INCLUDE

#include <vector>
#include "../Globals.h"

namespace sh
{
	namespace scene
	{
		class Mesh;

		class Model
		{
		public:
			Model();
			~Model();

			void SetPosition(const math::Vector3f& pos) { m_position = pos; }
			const math::Vector3f& GetPosition() const { return m_position; }
			void Render();
			void AddMesh(Mesh* mesh);
			size_t GetMeshesCount() const;
			Mesh* GetMesh(size_t index);

		private:
			std::vector<Mesh*> m_meshes;
			math::Vector3f m_position;
		};
	}
}

#endif
