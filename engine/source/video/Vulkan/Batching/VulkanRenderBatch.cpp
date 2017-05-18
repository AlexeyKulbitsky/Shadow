#include "VulkanRenderBatch.h"

#include "../../../Device.h"
#include "../../../scene/Mesh.h"
#include "../../Renderable.h"
#include "../../Material.h"
#include "../VulkanDriver.h"
#include "../VulkanCommandBuffer.h"

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
		*/

		for(auto& renderable : m_renderables)
		{
			driver->SetRenderPipeline(m_pipeline, m_commandBuffer);
			driver->SetGpuParams(m_gpuParams, m_commandBuffer);
			driver->SetGpuParams(renderable->GetAutoGpuParams(), m_commandBuffer);
			driver->SetVertexBuffer(renderable->GetVertexBuffer(), m_commandBuffer);
			driver->SetVertexDeclaration(renderable->GetVertexInputDeclaration(), m_commandBuffer);
			driver->SetIndexBuffer(renderable->GetIndexBuffer(), m_commandBuffer);
			driver->DrawIndexed(0, renderable->GetIndexBuffer()->GetIndicesCount(), 1U, m_commandBuffer);		



			/*
			driver->SetRenderPipeline(m_textBatch.material->GetRenderPipeline(), m_textBatch.commandBuffer);
			driver->SetGpuParams(m_textBatch.material->GetCommonGpuParams(), m_textBatch.commandBuffer);
			driver->SetTopology(Topology::TOP_TRIANGLE_LIST, m_textBatch.commandBuffer);
			driver->SetVertexBuffer(m_textBatch.vertexBuffer, m_textBatch.commandBuffer);
			driver->SetVertexDeclaration(m_textBatch.inputDeclaration, m_textBatch.commandBuffer);
			driver->SetIndexBuffer(m_textBatch.indexBuffer, m_textBatch.commandBuffer);				
			driver->DrawIndexed(0, m_textBatch.indexBuffer->GetIndicesCount(), 1U, m_textBatch.commandBuffer);
			*/
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