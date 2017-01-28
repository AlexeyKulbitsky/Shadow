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
			void SetMaterial(sh::video::Material* material);
			video::Material* GetMaterial();
			
			sh::video::RenderCommand* GetRenderCommand(size_t index = 0) { return m_renderCommands[index]; }

		private:
			Mesh(){}
			Mesh(const Mesh& other){}

		private:
			MeshBasePtr m_meshBase;
			math::Matrix4f m_worldMatrix;

			std::vector<sh::video::RenderCommand*>  m_renderCommands;
			sh::video::Material* m_material;
		};
	}
}
#endif