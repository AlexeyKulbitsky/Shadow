#ifndef SHADOW_MESH_INCLUDE
#define SHADOW_MESH_INCLUDE

#include <vector>
#include "../video/VertexBuffer.h"
#include "../video/IndexBuffer.h"
#include "../Types.h"

namespace sh
{
	namespace video
	{		
		class Material;
		class RenderCommand;
		class ShaderProgram;
	}

	namespace scene
	{		
		class MeshBase;
		class Mesh
		{
		public:			
			explicit Mesh(MeshBase* meshBase);
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
			MeshBase* m_meshBase;
			math::Matrix4f m_worldMatrix;

			std::vector<sh::video::RenderCommand*>  m_renderCommands;
			sh::video::Material* m_material;
		};
	}
}
#endif