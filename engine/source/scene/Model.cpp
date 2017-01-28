#include "Model.h"
#include "ModelBase.h"
#include "Mesh.h"

namespace sh
{
	namespace scene
	{
		Model::Model()
		{

		}

		//////////////////////////////////////////////////////////////////////////////

		Model::Model(ModelBasePtr modelBase)
		{
			size_t meshesCount = modelBase->GetMeshesCount();
			m_meshes.resize(meshesCount);
			for (size_t i = 0; i < meshesCount; ++i)
			{
				m_meshes[i] = MeshPtr(new Mesh(modelBase->GetMesh(i)));
			}
		}

		//////////////////////////////////////////////////////////////////////////////

		Model::~Model()
		{

		}

		//////////////////////////////////////////////////////////////////////////////

		void Model::SetPosition(const math::Vector3f& pos) 
		{ 
			m_position = pos; 
			m_worldMatrix.SetIdentity();
			m_worldMatrix.SetTranslation(m_position);
		}

		//////////////////////////////////////////////////////////////////////////////

		void Model::SetWorldMatrix(const math::Matrix4f& matrix) 
		{ 
			m_worldMatrix = matrix; 
			for (auto mesh : m_meshes)
			{
				mesh->SetWorldMatrix(m_worldMatrix);
			}
		}

		//////////////////////////////////////////////////////////////////////////////

		void Model::UpdateTransformationUniforms()
		{
			for (auto mesh : m_meshes)
			{
				mesh->SetWorldMatrix(m_worldMatrix);
				mesh->UpdateTransformationUniforms();
			}
		}

		//////////////////////////////////////////////////////////////////////////////

		void Model::AddMesh(MeshPtr mesh)
		{
			m_meshes.push_back(mesh);
		}

		//////////////////////////////////////////////////////////////////////////////

		size_t Model::GetMeshesCount() const
		{
			return m_meshes.size();
		}

		//////////////////////////////////////////////////////////////////////////////

		MeshPtr Model::GetMesh(size_t index)
		{
			return m_meshes[index];
		}

		//////////////////////////////////////////////////////////////////////////////
	}
}