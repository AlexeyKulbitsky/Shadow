#ifndef SHADOW_RENDER_TECHNIQUE_INLCLUDE
#define SHADOW_RENDER_TECHNIQUE_INLCLUDE

#include <vector>

namespace sh
{
	namespace video
	{
		class RenderPass;

		class RenderTechnique
		{
		public:
			RenderTechnique();
			~RenderTechnique();

			void Load(const char* filename);

			size_t GetRenderPassesCount() const { return m_renderPasses.size(); }
			RenderPass* GetRenderPass(size_t index) { return m_renderPasses[index]; }

		protected:
			std::vector<RenderPass*> m_renderPasses;
		};
	}
}

#endif
