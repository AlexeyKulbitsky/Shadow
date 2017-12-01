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

			void SetPosition(const math::Vector3f& pos);
			const math::Vector3f& GetPosition() const { return m_position; }
			void SetBoundingBox(const math::AABBf& boundingBox) { m_boundingBox = boundingBox; }
			const math::AABBf& GetBoundingBox() const { return m_boundingBox; }
			const math::AABBf& GetInitialBoundingBox() const { return m_initialBoundingBox; }
			void SetWorldMatrix(const math::Matrix4f& matrix);
			const math::Matrix4f& GetWorldMatrix() const { return m_worldMatrix; }
			void UpdateTransformationUniforms();
			void AddMesh(MeshPtr mesh);
			size_t GetMeshesCount() const;
			const MeshPtr& GetMesh(size_t index) const;

			void SetMaterial(const video::MaterialPtr& material);

		private:
			std::vector<MeshPtr> m_meshes;

			math::AABBf m_boundingBox;
			math::AABBf m_initialBoundingBox;
			math::Vector3f m_position;
			math::Quaternionf m_rotation;
			math::Vector3f m_scale;
			math::Matrix4f m_worldMatrix;
		};
	}
}

#endif
