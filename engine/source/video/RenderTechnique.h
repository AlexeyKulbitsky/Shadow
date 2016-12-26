#ifndef SHADOW_RENDER_TECHNIQUE_INLCLUDE
#define SHADOW_RENDER_TECHNIQUE_INLCLUDE

#include <vector>
#include "../resources/Resource.h"
#include "../Globals.h"

namespace sh
{
	namespace video
	{
		class RenderPass;

		class RenderTechnique : public Resource
		{
		public:
			RenderTechnique();
			~RenderTechnique();

			virtual void Load(const String& filePath) override;
			virtual void Unload() override;

			const String& GetName() const { return m_name; }
			size_t GetRenderPassesCount() const { return m_renderPasses.size(); }
			RenderPass* GetRenderPass(size_t index) { return m_renderPasses[index]; }

		protected:
			std::vector<RenderPass*> m_renderPasses;
			String m_name;
		};
	}
}

#endif
