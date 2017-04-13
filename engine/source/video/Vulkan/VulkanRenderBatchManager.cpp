#include "VulkanRenderBatchManager.h"

#include "../../scene/Mesh.h"
#include "../../Device.h"
#include "../Material.h"
#include "VulkanDriver.h"
#include "VulkanCommandBuffer.h"

namespace sh
{

namespace video
{
	VulkanRenderBatchManager::VulkanRenderBatchManager()
	{
		CommandBufferDescription desc;
		desc.type = COMMAND_BUFFER_TYPE_SECONDARY;

		m_commandBuffer = CommandBuffer::Create(desc);
	}

	VulkanRenderBatchManager::~VulkanRenderBatchManager()
	{
	}

	void VulkanRenderBatchManager::AddMesh(const scene::MeshPtr& mesh)
	{
		m_meshes.push_back(mesh);
	}

	void VulkanRenderBatchManager::Submit()
	{
		Driver* driver = Device::GetInstance()->GetDriver();
		VulkanDriver* vulkanDriver = static_cast<VulkanDriver*>(driver);
		m_commandBuffer->Begin();

		for(auto& mesh : m_meshes)
		{
			driver->SetVertexDeclaration(mesh->GetVertexDeclaration(), m_commandBuffer);
			driver->SetRenderPipeline(mesh->GetMaterial()->GetRenderPipeline(), m_commandBuffer);
			driver->SetGpuParams(mesh->GetGpuParams(), m_commandBuffer);
			driver->SetGpuParams(mesh->GetAutoGpuParams(), m_commandBuffer);
			driver->SetIndexBuffer(mesh->GetIndexBuffer(), m_commandBuffer);
			driver->SetVertexBuffer(mesh->GetVertexBuffer(), m_commandBuffer);
			driver->DrawIndexed(0, mesh->GetIndexBuffer()->GetIndicesCount(), 1U, m_commandBuffer);			
		}

		m_commandBuffer->End();

		VulkanCommandBuffer* cmdBuf = static_cast<VulkanCommandBuffer*>(m_commandBuffer.get());
		vulkanDriver->GetPrimaryCommandBuffer()->Append(cmdBuf);
	}

	void VulkanRenderBatchManager::Clear()
	{
	}

} // video

} // sh