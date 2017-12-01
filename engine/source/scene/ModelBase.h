#ifndef SHADOW_MODEL_BASE_INCLUDE
#define SHADOW_MODEL_BASE_INCLUDE

#include "../Globals.h"
#include "Model.h"

namespace sh
{
	namespace scene
	{
		class SHADOW_API ModelBase : public Resource
		{
		public:
			ModelBase();
			~ModelBase();
			virtual void Unload() override;

			void AddMesh(MeshBasePtr mesh);
			size_t GetMeshesCount() const;
			MeshBasePtr GetMesh(size_t index);

			const math::AABBf& GetBoundingBox() const { return m_boundingBox; }
		private:
			std::vector<MeshBasePtr> m_meshes;
			math::AABBf m_boundingBox;
		};
	}
}

#endif