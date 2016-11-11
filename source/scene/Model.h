#ifndef SHADOW_MODEL_INCLUDE
#define SHADOW_MODEL_INCLUDE

#include <vector>

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
			void Render();
			void AddMesh(Mesh* mesh);
			size_t GetMeshesCount() const;
			Mesh* GetMesh(size_t index);

		private:
			std::vector<Mesh*> m_meshes;
		};
	}
}

#endif
