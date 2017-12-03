#ifndef SHADOW_MESH_INCLUDE
#define SHADOW_MESH_INCLUDE

#include "../Globals.h"
#include "../video/GpuParams.h"

namespace sh
{
	namespace scene
	{		
		class SHADOW_API Mesh
		{
		public:			
			explicit Mesh(MeshBasePtr meshBase);
			~Mesh();	
			void SetWorldMatrix(const math::Matrix4& matrix);
			const math::Matrix4& GetWorldMatrix() const { return m_worldMatrix; }
			void SetMaterial(const sh::video::MaterialPtr& material);
			const video::MaterialPtr& GetMaterial();
			const String& GetName() const;

			const video::RenderablePtr& GetRanderable() const { return m_renderable; }

		private:
			Mesh(){}
			Mesh(const Mesh& other){}

		private:
			MeshBasePtr m_meshBase;
			math::Matrix4 m_worldMatrix;
			math::AABB m_boundingBox;

			sh::video::MaterialPtr m_material;



			video::RenderablePtr m_renderable;
		};
	}
}
#endif