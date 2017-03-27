#include "ModelBase.h"
#include "MeshBase.h"

namespace sh
{
	namespace scene
	{
		ModelBase::ModelBase()
		{
		}

		//////////////////////////////////////////////////////////////////////////////

		ModelBase::~ModelBase()
		{
		}

		//////////////////////////////////////////////////////////////////////////////

		void ModelBase::Unload()
		{
			for (size_t i = 0; i < m_meshes.size(); ++i)
			{
				m_meshes[i]->Unload();
			}

			m_meshes.clear();
		}

		//////////////////////////////////////////////////////////////////////////////

		void ModelBase::AddMesh(MeshBasePtr mesh)
		{
			m_meshes.push_back(mesh);
		}

		//////////////////////////////////////////////////////////////////////////////

		size_t ModelBase::GetMeshesCount() const
		{
			return m_meshes.size();
		}

		//////////////////////////////////////////////////////////////////////////////

		MeshBasePtr ModelBase::GetMesh(size_t index)
		{
			return m_meshes[index];
		}

		//////////////////////////////////////////////////////////////////////////////
	}
}