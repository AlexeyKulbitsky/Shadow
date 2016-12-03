#include "Entity.h"

namespace sh
{
	namespace scene
	{
		Entity::Entity(Model* model)
		{
			size_t meshesCount = model->GetMeshesCount();

			m_subEntities.resize(meshesCount);

			for (size_t i = 0; i < meshesCount; ++i)
			{
				Mesh* mesh = model->GetMesh(i);

				m_subEntities[i] = new SubEntity(mesh);
			}
		}

		////////////////////////////////////////////////////////////////////////////////////

		void Entity::Render()
		{

		}
	}
}