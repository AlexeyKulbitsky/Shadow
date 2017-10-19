#ifndef SHADOW_GL3_RENDER_BATCH_INCLUDE
#define SHADOW_GL_3RENDER_BATCH_INCLUDE

#include "../../../../Globals.h"

namespace sh
{
	namespace video
	{
		class GL3RenderBatch
		{
		public:
			/*
			This class is used to hold render commands with the same material
			*/
			GL3RenderBatch() {}
			~GL3RenderBatch() {}
			void SetRenderPipeline(const RenderPipelinePtr& pipeline);
			void SetGpuParams(const GpuParamsPtr& gpuParams);
			void SetMaterialParams(const MaterialParamsPtr& materialParams);
			void AddMesh(const scene::MeshPtr& mesh);
			void RemoveMesh(const scene::MeshPtr& mesh);
			void Submit();
			void Clear();
			void UpdateLight(scene::Light* light);

			void SetTechniqueName(const String& name) { m_techniqueName = name; }
			const String& GetTechniqueName() const { return m_techniqueName; }

		private:
			void UpdateParams();

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