#ifndef SHADOW_MATERIAL_INCLUDE
#define SHADWO_MATERIAL_INCLUDE

#include "../Globals.h"

namespace sh
{
	namespace video
	{
		class RenderTechnique;
		class RenderPipeline;

		class Material
		{
		public:
			Material() {}
			void SetRenderTechnique(RenderTechnique* technique);

			RenderTechnique* GetRenderTechnique() { return m_renderTechnique; }

			size_t GetRenderPassesCount() const { return m_renderPasses.size(); }
			RenderPipeline* GetRenderPipeline(size_t index) { return m_renderPasses[index]; }
			
		private:
			RenderTechnique* m_renderTechnique = nullptr;
			std::vector<RenderPipeline*> m_renderPasses;
		};
	}
}

#endif