#ifndef SHADOW_GLES20_RENDER_BATCH_MANAGER_INCLDUE
#define SHADOW_GLES20_RENDER_BATCH_MANAGER_INCLDUE

#include "../RenderBatchManager.h"
#include "GLES20RenderPipeline.h"

namespace sh
{
	namespace video
	{
		class GLES20RenderBatchManager : public RenderBatchManager
		{
		public:
			virtual ~GLES20RenderBatchManager();
			virtual void AddMesh(scene::Mesh* mesh) override;
			virtual void Submit() override;
			virtual void Clear() override;

		private:
			typedef Map<String, RenderBatchPtr> PerTechniqueRenderBatchesMap;

			PerTechniqueRenderBatchesMap m_renderBatches[(size_t)RenderPipeline::Layer::COUNT];
		};
	}
}

#endif