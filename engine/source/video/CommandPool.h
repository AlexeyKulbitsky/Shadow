#ifndef SHADOW_COMMAND_POOL_INCLUDE
#define SHADOW_COMMAND_POOL_INCLUDE

#include "../Globals.h"
#include "RenderPipeline.h"

namespace sh
{
	namespace video
	{
		class CommandPool
		{
		public:
			void AddMesh(scene::Mesh* mesh);
			void Submit();

		protected:
			typedef HashMap<String, RenderBatch*> PerTechniqueRenderBatchesMap;

			PerTechniqueRenderBatchesMap m_renderBatches[(size_t)RenderPipeline::Layer::COUNT];
		};
	}
}

#endif