#ifndef SHADOW_MESH_INCLUDE
#define SHADOW_MESH_INCLUDE

#include <vector>
#include "../video/HardwareBuffer.h"
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
		class Mesh
		{
		public:
			Mesh();
			~Mesh();
			void Init();	
			void Render();

			void SetVertexDeclaration(sh::video::VertexDeclaration vertexDeclaration);
			void SetVertexData(std::vector<float> data);
			void SetIndexData(std::vector<unsigned int> data);
			void SetMaterial(sh::video::Material* material);
			
			sh::video::RenderCommand* GetRenderCommand() { return m_renderCommand; }
		private:
			sh::video::RenderCommand* m_renderCommand;
			sh::video::Material* m_material;
		};
	}
}
#endif