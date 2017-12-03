#ifndef SHADOW_MODEL_INCLUDE
#define SHADOW_MODEL_INCLUDE

#include "../Globals.h"
#include "../resources/Resource.h"

namespace sh
{
	namespace scene
	{
		class SHADOW_API Model : public Resource
		{
		public:
			Model();
			Model(ModelBasePtr modelBase);
			~Model();

			void SetPosition(const math::Vector3& pos);
			const math::Vector3& GetPosition() const { return m_position; }
			void SetBoundingBox(const math::AABB& boundingBox) { m_boundingBox = boundingBox; }
			const math::AABB& GetBoundingBox() const { return m_boundingBox; }
			const math::AABB& GetInitialBoundingBox() const { return m_initialBoundingBox; }
			void SetWorldMatrix(const math::Matrix4& matrix);
			const math::Matrix4& GetWorldMatrix() const { return m_worldMatrix; }
			void UpdateTransformationUniforms();
			void AddMesh(MeshPtr mesh);
			size_t GetMeshesCount() const;
			const MeshPtr& GetMesh(size_t index) const;

			void SetMaterial(const video::MaterialPtr& material);

		private:
			std::vector<MeshPtr> m_meshes;

			math::AABB m_boundingBox;
			math::AABB m_initialBoundingBox;
			math::Vector3 m_position;
			math::Quaternion m_rotation;
			math::Vector3 m_scale;
			math::Matrix4 m_worldMatrix;
		};
	}
}

#endif
