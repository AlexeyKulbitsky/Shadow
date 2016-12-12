#include "CommandPool.h"
#include "RenderBatch.h"
#include "Material.h"
#include "RenderTechnique.h"
#include "RenderPass.h"
#include "RenderState.h"
#include "ShaderProgram.h"
#include "../scene/Mesh.h"

namespace sh
{
	namespace video
	{
		void CommandPool::AddMesh(scene::Mesh* mesh)
		{
			Material* material = mesh->GetMaterial();
			String techniqueName = material->GetRenderTechnique()->GetName();
			RenderPass::Layer layer = material->GetRenderPass(0)->GetLayer();
			size_t layerIndex = (size_t)layer;

			if (m_renderBatches[layerIndex].find(techniqueName) == m_renderBatches[layerIndex].end())
			{
				RenderBatch* renderBatch = new RenderBatch();
				ShaderProgram* shader = material->GetRenderTechnique()->GetRenderPass(0)->GetShaderProgram();
				RenderState* renderState = material->GetRenderTechnique()->GetRenderPass(0)->GetRenderState();
				renderBatch->SetShaderProgram(shader);
				renderBatch->SetRenderState(renderState);
				renderBatch->SetTechniqueName(techniqueName);

				m_renderBatches[layerIndex][techniqueName] = renderBatch;
			}

			m_renderBatches[layerIndex][techniqueName]->AddCommand(mesh->GetRenderCommand());
		}

		///////////////////////////////////////////////////////////////////

		void CommandPool::Submit()
		{
			for (size_t i = 0; i < (size_t)RenderPass::Layer::COUNT; ++i)
			{
				for (auto renderBatch : m_renderBatches[i])
				{
					renderBatch.second->Submit();
				}
			}
			
		}

		///////////////////////////////////////////////////////////////////
	}
}