#ifndef SHADOW_MODEL_INCLUDE
#define SHADOW_MODEL_INCLUDE

#include "../Globals.h"
#include "../resources/Resource.h"

namespace sh
{
	namespace scene
	{
		class Model : public Resource
		{
		public:
			Model();
			Model(ModelBasePtr modelBase);
			~Model();

			void SetPosition(const math::Vector3f& pos);
			const math::Vector3f& GetPosition() const { return m_position; }
			void SetWorldMatrix(const math::Matrix4f& matrix);
			void UpdateTransformationUniforms();
			void AddMesh(MeshPtr mesh);
			size_t GetMeshesCount() const;
			MeshPtr GetMesh(size_t index);

		private:
			std::vector<MeshPtr> m_meshes;

			math::Vector3f m_position;
			math::Quaternionf m_rotation;
			math::Vector3f m_scale;
			math::Matrix4f m_worldMatrix;
		};
	}
}

#endif
