#include "CommandPool.h"
#include "RenderBatch.h"
#include "Material.h"
#include "RenderTechnique.h"
#include "RenderPipeline.h"
#include "ShaderProgram.h"
#include "../scene/Mesh.h"

namespace sh
{
	namespace video
	{
		void CommandPool::AddMesh(scene::Mesh* mesh)
		{
			const MaterialPtr& material = mesh->GetMaterial();
			String techniqueName = material->GetRenderTechnique()->GetName();
			RenderPipeline::Layer layer = material->GetRenderPipeline(0)->GetLayer();
			size_t layerIndex = (size_t)layer;

			if (m_renderBatches[layerIndex].find(techniqueName) == m_renderBatches[layerIndex].end())
			{
				RenderBatchPtr renderBatch(new RenderBatch());
				const RenderPipelinePtr& renderPipeline = material->GetRenderPipeline(0);
				const ShaderProgramPtr& shader = renderPipeline->GetShaderProgram();
				renderBatch->SetShaderProgram(shader);
				renderBatch->SetDepthStencilState(renderPipeline->GetDepthStencilState());
				renderBatch->SetRasterizationState(renderPipeline->GetRasterizationState());
				renderBatch->SetBlendingState(renderPipeline->GetBlendingState());
				renderBatch->SetUniformBuffer(renderPipeline->GetUniformBuffer());
				renderBatch->SetTechniqueName(techniqueName);

				m_renderBatches[layerIndex][techniqueName] = renderBatch;
			}

			m_renderBatches[layerIndex][techniqueName]->AddCommand(mesh->GetRenderCommand());
		}

		///////////////////////////////////////////////////////////////////

		void CommandPool::Submit()
		{
			for (size_t i = 0; i < (size_t)RenderPipeline::Layer::COUNT; ++i)
			{
				for (auto renderBatch : m_renderBatches[i])
				{
					renderBatch.second->Submit();
				}
			}
			
		}

		///////////////////////////////////////////////////////////////////

		void CommandPool::Clear()
		{
			for (size_t i = 0; i < (size_t)RenderPipeline::Layer::COUNT; ++i)
			{
				m_renderBatches[i].clear();
			}
		}

		///////////////////////////////////////////////////////////////////
	}
}