#include "VulkanRenderBatch.h"

#include "../../../Device.h"
#include "../../../scene/Mesh.h"
#include "../../Renderable.h"
#include "../../Material.h"
#include "VulkanDriver.h"
#include "VulkanCommandBuffer.h"

namespace sh
{

namespace video
{

	void VulkanRenderBatch::SetRenderPipeline( const RenderPipelinePtr& pipeline )
	{
		m_pipeline = pipeline;
	}

	void VulkanRenderBatch::SetGpuParams(const GpuParamsPtr& gpuParams)
	{
		m_gpuParams = gpuParams;
	}

	void VulkanRenderBatch::AddMesh(const scene::MeshPtr& mesh)
	{
		m_renderables.push_back(mesh->GetRanderable());
	}

	void VulkanRenderBatch::Submit()
	{
		if( !m_commandBuffer )
		{
			CommandBufferDescription desc;
			desc.type = COMMAND_BUFFER_TYPE_SECONDARY;
			m_commandBuffer = CommandBuffer::Create(desc);
		}

		Driver* driver = Device::GetInstance()->GetDriver();
		VulkanDriver* vulkanDriver = static_cast<VulkanDriver*>(driver);
		m_commandBuffer->Begin();

		/*
		if( m_renderables.size() > 0 )
		{
			driver->SetVertexDeclaration(m_renderables[0]->GetVertexInputDeclaration(), m_commandBuffer);
			driver->SetRenderPipeline(m_renderables[0]->GetParent()->GetMaterial()->GetRenderPipeline(), m_commandBuffer);
			driver->SetGpuParams(m_renderables[0]->GetParent()->GetMaterial()->GetCommonGpuParams(), m_commandBuffer);

		}
		*/

		for(auto& renderable : m_renderables)
		{
			driver->SetVertexDeclaration(renderable->GetVertexInputDeclaration(), m_commandBuffer);
			driver->SetRenderPipeline(m_pipeline, m_commandBuffer);
			driver->SetGpuParams(m_gpuParams, m_commandBuffer);
			driver->SetGpuParams(renderable->GetAutoGpuParams(), m_commandBuffer);
			driver->SetIndexBuffer(renderable->GetIndexBuffer(), m_commandBuffer);
			driver->SetVertexBuffer(renderable->GetVertexBuffer(), m_commandBuffer);
			driver->DrawIndexed(0, renderable->GetIndexBuffer()->GetIndicesCount(), 1U, m_commandBuffer);		
		}

		m_commandBuffer->End();

		VulkanCommandBuffer* cmdBuf = static_cast<VulkanCommandBuffer*>(m_commandBuffer.get());
		vulkanDriver->GetPrimaryCommandBuffer()->Append(cmdBuf);
	}

	void VulkanRenderBatch::Clear()
	{

	}


} // video

} // sh