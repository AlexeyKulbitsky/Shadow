#include "CommandPool.h"
#include "CommandBuffer.h"
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

			if (m_buffers[layerIndex].find(techniqueName) == m_buffers[layerIndex].end())
			{
				CommandBuffer* buffer = new CommandBuffer();
				ShaderProgram* shader = material->GetRenderTechnique()->GetRenderPass(0)->GetShaderProgram();
				RenderState* renderState = material->GetRenderTechnique()->GetRenderPass(0)->GetRenderState();
				buffer->SetShaderProgram(shader);
				buffer->SetRenderState(renderState);
				buffer->SetTechniqueName(techniqueName);

				m_buffers[layerIndex][techniqueName] = buffer;
			}

			m_buffers[layerIndex][techniqueName]->AddCommand(mesh->GetRenderCommand());
		}

		///////////////////////////////////////////////////////////////////

		void CommandPool::Submit()
		{
			for (size_t i = 0; i < (size_t)RenderPass::Layer::COUNT; ++i)
			{
				for (auto buffer : m_buffers[i])
				{
					buffer.second->Submit();
				}
			}
			
		}

		///////////////////////////////////////////////////////////////////
	}
}