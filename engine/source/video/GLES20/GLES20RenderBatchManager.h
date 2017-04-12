#ifndef SHADOW_GLES20_RENDER_BATCH_MANAGER_INCLDUE
#define SHADOW_GLES20_RENDER_BATCH_MANAGER_INCLDUE

#include "../RenderBatchManager.h"
#include "GLES20RenderPipeline.h"

namespace sh
{
	namespace video
	{
		DEFINE_CLASS_PTR(GLES20RenderBatch)

		class GLES20RenderBatchManager : public RenderBatchManager
		{
		public:
			virtual ~GLES20RenderBatchManager();
			virtual void AddMesh(const scene::MeshPtr& mesh) override;
			virtual void Submit() override;
			virtual void Clear() override;

		private:
			typedef Map<String, GLES20RenderBatchPtr> PerTechniqueRenderBatchesMap;

			PerTechniqueRenderBatchesMap m_renderBatches[(size_t)RenderPipeline::Layer::COUNT];
		};
	}
}

#endif