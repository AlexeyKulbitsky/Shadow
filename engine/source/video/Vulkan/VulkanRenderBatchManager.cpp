#include "VulkanRenderBatchManager.h"

#include "../../scene/Mesh.h"
#include "../../Device.h"
#include "../Material.h"
#include "VulkanDriver.h"

namespace sh
{

namespace video
{

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

		for(auto& mesh : m_meshes)
		{
			driver->SetRenderPipeline(mesh->GetMaterial()->GetRenderPipeline());
			driver->SetGpuParams(mesh->GetGpuParams());
			driver->SetVertexDeclaration(mesh->GetVertexDeclaration());
			driver->SetIndexBuffer(mesh->GetIndexBuffer());
			driver->SetVertexBuffer(mesh->GetVertexBuffer());
			driver->DrawIndexed(0, mesh->GetIndexBuffer()->GetIndicesCount());			
		}
	}

	void VulkanRenderBatchManager::Clear()
	{
	}

} // video

} // sh