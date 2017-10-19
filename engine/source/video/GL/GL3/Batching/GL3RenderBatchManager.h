#ifndef SHADOW_GL3_RENDER_BATCH_MANAGER_INCLDUE
#define SHADOW_GL3_RENDER_BATCH_MANAGER_INCLDUE

#include "../../../RenderBatchManager.h"
#include "../GL3RenderPipeline.h"

namespace sh
{
	namespace video
	{
		DEFINE_CLASS_PTR(GL3RenderBatch)

		class GL3RenderBatchManager : public RenderBatchManager
		{
		public:
			virtual ~GL3RenderBatchManager();
			virtual void AddMesh(const scene::MeshPtr& mesh) override;
			virtual void RemoveMesh(const scene::MeshPtr& mesh) override;
			virtual void Submit() override;
			virtual void Clear() override;
			virtual void UpdateLight(scene::Light* light) override;

		private:
			typedef Map<String, GL3RenderBatchPtr> PerTechniqueRenderBatchesMap;

			PerTechniqueRenderBatchesMap m_renderBatches[(size_t)RenderLayer::Count];
		};
	}
}

#endif