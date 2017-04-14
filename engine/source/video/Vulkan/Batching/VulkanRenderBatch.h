#ifndef SHADOW_VULKAN_RENDER_BATCH_INCLUDE
#define SHADOW_VULKAN_RENDER_BATCH_INCLUDE

#include "../../../Globals.h"

namespace sh
{

namespace video
{
	
	class VulkanRenderBatch
	{
	public:
		/*
		This class is used to hold render commands with the same material
		*/
		VulkanRenderBatch() {}
		virtual ~VulkanRenderBatch() {}
		virtual void SetRenderPipeline(const RenderPipelinePtr& pipeline);
		virtual void SetGpuParams(const GpuParamsPtr& gpuParams);
		virtual void AddMesh(const scene::MeshPtr& mesh);
		virtual void Submit();
		virtual void Clear();

		void SetTechniqueName(const String& name) { m_techniqueName = name; }
		const String& GetTechniqueName() const { return m_techniqueName; }

	protected:
		String m_techniqueName;
		std::vector<RenderablePtr> m_renderables;

		RenderPipelinePtr m_pipeline;
		GpuParamsPtr m_gpuParams;

		CommandBufferPtr m_commandBuffer;
	};

} // video

} // sh

#endif