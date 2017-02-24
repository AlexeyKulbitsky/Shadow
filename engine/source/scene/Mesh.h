#ifndef SHADOW_MESH_INCLUDE
#define SHADOW_MESH_INCLUDE

#include "../Globals.h"

namespace sh
{
	namespace scene
	{		
		class Mesh
		{
		public:			
			explicit Mesh(MeshBasePtr meshBase);
			~Mesh();	
			void UpdateTransformationUniforms();
			void SetWorldMatrix(const math::Matrix4f& matrix);
			void SetMaterial(const sh::video::MaterialPtr& material);
			const video::MaterialPtr& GetMaterial();
			
			const sh::video::RenderCommandPtr& GetRenderCommand(size_t index = 0U) { return m_renderCommands[index]; }

		private:
			Mesh(){}
			Mesh(const Mesh& other){}

		private:
			MeshBasePtr m_meshBase;
			math::Matrix4f m_worldMatrix;

			std::vector<sh::video::RenderCommandPtr>  m_renderCommands;
			sh::video::MaterialPtr m_material;
		};
	}
}
#endif