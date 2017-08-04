#ifndef SHADOW_MESH_INCLUDE
#define SHADOW_MESH_INCLUDE

#include "../Globals.h"
#include "../video/GpuParams.h"

namespace sh
{
	namespace scene
	{		
		class Mesh
		{
		public:			
			explicit Mesh(MeshBasePtr meshBase);
			~Mesh();	
			void SetWorldMatrix(const math::Matrix4f& matrix);
			const math::Matrix4f& GetWorldMatrix() const { return m_worldMatrix; }
			void SetMaterial(const sh::video::MaterialPtr& material);
			const video::MaterialPtr& GetMaterial();

			const video::RenderablePtr& GetRanderable() const { return m_renderable; }

		private:
			Mesh(){}
			Mesh(const Mesh& other){}

		private:
			MeshBasePtr m_meshBase;
			math::Matrix4f m_worldMatrix;
			math::AABBf m_boundingBox;

			sh::video::MaterialPtr m_material;



			video::RenderablePtr m_renderable;
		};
	}
}
#endif