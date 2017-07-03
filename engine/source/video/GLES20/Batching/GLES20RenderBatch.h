#ifndef SHADOW_RENDER_BATCH_INCLUDE
#define SHADOW_RENDER_BATCH_INCLUDE

#include "../../../Globals.h"

namespace sh
{
	namespace video
	{
		class GLES20RenderBatch
		{
		public:
			/*
			This class is used to hold render commands with the same material
			*/
			GLES20RenderBatch() {}
			~GLES20RenderBatch() {}
			void SetRenderPipeline(const RenderPipelinePtr& pipeline);
			void SetGpuParams(const GpuParamsPtr& gpuParams);
			void SetMaterialParams(const MaterialParamsPtr& materialParams);
			void AddMesh(const scene::MeshPtr& mesh);
			void Submit();
			void Clear();
			void UpdateLight(scene::Light* light);

			void SetTechniqueName(const String& name) { m_techniqueName = name; }
			const String& GetTechniqueName() const { return m_techniqueName; }


		protected:
			String m_techniqueName;
			std::vector<RenderablePtr> m_renderables;

			RenderPipelinePtr m_pipeline;
			GpuParamsPtr m_gpuParams;
			MaterialParamsPtr m_materialParams;
		};
	}
}

#endif