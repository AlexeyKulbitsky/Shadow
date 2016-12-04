#include "ModelBase.h"

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

		void ModelBase::AddMesh(MeshBase* mesh)
		{
			m_meshes.push_back(mesh);
		}

		//////////////////////////////////////////////////////////////////////////////

		size_t ModelBase::GetMeshesCount() const
		{
			return m_meshes.size();
		}

		//////////////////////////////////////////////////////////////////////////////

		MeshBase* ModelBase::GetMesh(size_t index)
		{
			return m_meshes[index];
		}

		//////////////////////////////////////////////////////////////////////////////
	}
}