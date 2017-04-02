#ifndef SHADOW_RENDER_TECHNIQUE_INLCLUDE
#define SHADOW_RENDER_TECHNIQUE_INLCLUDE

#include "../resources/Resource.h"
#include "../Globals.h"

namespace sh
{
	namespace video
	{
		class RenderTechnique : public Resource
		{
		public:
			RenderTechnique();
			~RenderTechnique();

			virtual void Load(const String& filePath) override;
			void LoadAlternative(const String& filePath);
			virtual void Unload() override;

			const String& GetName() const { return m_name; }
			size_t GetRenderPipelinesCount() const { return m_renderPipelines.size(); }
			const RenderPipelinePtr& GetRenderPipeline(size_t index) { return m_renderPipelines[index]; }

		protected:
			DepthStencilStatePtr LoadDepthStencilState(const pugi::xml_node& node);
			RasterizationStatePtr LoadRasterizationState(const pugi::xml_node& node);
			BlendingStatePtr LoadBlendingState(const pugi::xml_node& node);

		protected:
			std::vector<RenderPipelinePtr> m_renderPipelines;
			String m_name;
		};
	}
}

#endif
