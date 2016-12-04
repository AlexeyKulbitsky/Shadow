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
		class MeshBase;
		class Mesh
		{
		public:			
			explicit Mesh(MeshBase* meshBase);
			~Mesh();	
			void Render();

			bool IsUseIndices() { return m_useIndices; }

			void SetVertexDeclaration(sh::video::VertexDeclaration vertexDeclaration);
			void SetVertexData(std::vector<float> data);
			void SetIndexData(std::vector<unsigned int> data);
			void SetMaterial(sh::video::Material* material);

			video::VertexBuffer* GetVertexBuffer();
			video::IndexBuffer* GetIndexBuffer();
			video::VertexDeclaration* GetVertexDeclaration();
			
			sh::video::RenderCommand* GetRenderCommand() { return m_renderCommand; }

		private:
			Mesh(){}
			Mesh(const Mesh& other){}

		private:
			MeshBase* m_meshBase;

			video::VertexBuffer* m_vertexBuffer;
			video::IndexBuffer* m_indexBuffer;
			video::VertexDeclaration m_vertexDeclaration;
			bool m_useIndices;

			sh::video::RenderCommand* m_renderCommand;
			sh::video::Material* m_material;
		};
	}
}
#endif