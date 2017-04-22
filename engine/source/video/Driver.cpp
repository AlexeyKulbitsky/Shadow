#include "Driver.h"
#include "Material.h"
#include "RenderPipeline.h"
#include "DepthStencilState.h"
#include "RasterizationState.h"
#include "BlendingState.h"

#include "../scene/Mesh.h"
#include "../scene/Model.h"

namespace sh
{
	namespace video
	{
		void Driver::Render(scene::Model* model)
		{
			size_t meshesCount = model->GetMeshesCount();
			for (size_t i = 0; i < meshesCount; ++i)
			{
				sh::scene::MeshPtr mesh = model->GetMesh(i);
				Render(mesh.get());
			}
		}

		/////////////////////////////////////////////////////////////////////////

		void Driver::Render(scene::Mesh* mesh)
		{
			const MaterialPtr& material = mesh->GetMaterial();
			const RenderPipelinePtr& renderPipeline = material->GetRenderPipeline(0);

			SetDepthStencilState(renderPipeline->GetDepthStencilState());
			SetRasterizationState(renderPipeline->GetRasterizationState());
			SetBlendingState(renderPipeline->GetBlendingState());
//			renderPipeline->GetShaderProgram()->BindProgram();
//			renderPipeline->GetUniformBuffer()->Upload();


			//const RenderCommandPtr& renderCommand = mesh->GetRenderCommand();

			//Render(renderCommand);
//			renderPipeline->GetShaderProgram()->UnbindProgram();
		}

		/////////////////////////////////////////////////////////////////////////
		
		void Driver::SetViewport(u32 x, u32 y, u32 width, u32 height)
		{
			m_viewPort.x = x;
			m_viewPort.y = y;
			m_viewPort.z = width;
			m_viewPort.w = height;
		}

		/////////////////////////////////////////////////////////////////////////
	
		void Driver::SetDepthRange(f32 zMin, f32 zMax)
		{
			m_depthRange.x = zMin;
			m_depthRange.y = zMax;
		}

		/////////////////////////////////////////////////////////////////////////
	}
}