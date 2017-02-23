#ifndef SHADOW_MATERIAL_INCLUDE
#define SHADOW_MATERIAL_INCLUDE

#include "../Globals.h"

namespace sh
{
	namespace video
	{
		class Material
		{
		public:
			Material() {}
			void SetRenderTechnique(const RenderTechniquePtr& technique);

			RenderTechnique* GetRenderTechnique() { return m_renderTechnique.get(); }

			size_t GetRenderPipelinesCount() const { return m_renderPipelines.size(); }
			RenderPipeline* GetRenderPipeline(size_t index) { return m_renderPipelines[index]; }
			
		private:
			RenderTechniquePtr m_renderTechnique = nullptr;
			std::vector<RenderPipeline*> m_renderPipelines;
		};
	}
}

#endif