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

			if (m_buffers.find(techniqueName) == m_buffers.end())
			{
				CommandBuffer* buffer = new CommandBuffer();
				ShaderProgram* shader = material->GetRenderTechnique()->GetRenderPass(0)->GetShaderProgram();
				RenderState* renderState = material->GetRenderTechnique()->GetRenderPass(0)->GetRenderState();
				buffer->SetShaderProgram(shader);
				buffer->SetRenderState(renderState);
				buffer->SetTechniqueName(techniqueName);

				m_buffers[techniqueName] = buffer;
			}

			m_buffers[techniqueName]->AddCommand(mesh->GetRenderCommand());
		}

		///////////////////////////////////////////////////////////////////

		void CommandPool::Submit()
		{
			for (auto buffer : m_buffers)
			{
				buffer.second->Submit();
			}
		}

		///////////////////////////////////////////////////////////////////
	}
}