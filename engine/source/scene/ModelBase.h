#ifndef SHADOW_MODEL_BASE_INCLUDE
#define SHADOW_MODEL_BASE_INCLUDE

#include "../Globals.h"
#include "Model.h"

namespace sh
{
	namespace scene
	{
		class MeshBase;

		class ModelBase : public Resource
		{
		public:
			ModelBase();
			~ModelBase();
			
			void AddMesh(MeshBase* mesh);
			size_t GetMeshesCount() const;
			MeshBase* GetMesh(size_t index);

		private:
			std::vector<MeshBase*> m_meshes;
		};
	}
}

#endif