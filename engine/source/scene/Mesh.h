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
			void UpdateTransformationUniforms();
			void SetWorldMatrix(const math::Matrix4f& matrix);
			void SetMaterial(const sh::video::MaterialPtr& material);
			const video::MaterialPtr& GetMaterial();
			const video::GpuParamsPtr& GetGpuParams(u32 idx = 0U) const { return m_gpuParams[idx]; }

			const video::VertexInputDeclarationPtr& GetVertexDeclaration() const { return m_vertexDeclaration[0]; }
			const video::VertexBufferPtr& GetVertexBuffer() const { return m_vertexBuffer; }
			const video::IndexBufferPtr& GetIndexBuffer() const { return m_indexBuffer; }

		private:
			Mesh(){}
			Mesh(const Mesh& other){}

		private:
			MeshBasePtr m_meshBase;
			math::Matrix4f m_worldMatrix;

			sh::video::MaterialPtr m_material;

			video::VertexBufferPtr m_vertexBuffer;
			video::IndexBufferPtr m_indexBuffer;
			std::vector<video::VertexInputDeclarationPtr> m_vertexDeclaration;
			std::vector<video::GpuParamsPtr> m_gpuParams;

			video::GpuParamMatrix4f m_worldMatrixParam;
			video::GpuParamMatrix4f m_viewMatrixParam;
			video::GpuParamMatrix4f m_projectionMatrixParam;
			video::GpuParamMatrix4f m_worldViewdProjectionMatrixParam;
			video::GpuParamVector3f m_lightDirectionParam;
		};
	}
}
#endif