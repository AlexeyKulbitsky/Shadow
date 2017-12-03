#include "Model.h"
#include "ModelBase.h"
#include "Mesh.h"

namespace sh
{
	namespace scene
	{
		Model::Model()
			: m_boundingBox(math::Vector3(0.0f))
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

			m_boundingBox = modelBase->GetBoundingBox();
			m_initialBoundingBox = modelBase->GetBoundingBox();

			SetFileInfo(modelBase->GetFileInfo());
		}

		//////////////////////////////////////////////////////////////////////////////

		Model::~Model()
		{
		}

		//////////////////////////////////////////////////////////////////////////////

		void Model::SetPosition(const math::Vector3& pos) 
		{ 
			m_position = pos; 
			m_worldMatrix.SetIdentity();
			m_worldMatrix.SetTranslation(m_position);
		}

		//////////////////////////////////////////////////////////////////////////////

		void Model::SetWorldMatrix(const math::Matrix4& matrix) 
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

		const MeshPtr& Model::GetMesh(size_t index) const
		{
			return m_meshes[index];
		}

		//////////////////////////////////////////////////////////////////////////////

		void Model::SetMaterial(const video::MaterialPtr& material)
		{
			for (const auto& mesh : m_meshes)
			{
				mesh->SetMaterial(material);
			}
		}

		//////////////////////////////////////////////////////////////////////////////
	}
}