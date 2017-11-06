#ifndef SHADOW_RENDER_TECHNIQUE_INLCLUDE
#define SHADOW_RENDER_TECHNIQUE_INLCLUDE

#include "../resources/Resource.h"
#include "../Globals.h"
#include "RenderPipeline.h"
#include "MaterialParams.h"

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
			virtual void Load(const std::vector<char>& data) override;
			void LoadInternal(const pugi::xml_document& document);
			virtual void Unload() override;

			const String& GetName() const { return m_name; }
			size_t GetRenderPipelinesCount() const { return m_renderPipelines.size(); }
			const RenderPipelinePtr& GetRenderPipeline(size_t index) { return m_renderPipelines[index]; }
			const MaterialParamsDescription& GetMaterialParamsDescription(size_t index) { return m_paramDescriptions[index]; }

		protected:
			DepthStencilStatePtr LoadDepthStencilState(const pugi::xml_node& node);
			RasterizationStatePtr LoadRasterizationState(const pugi::xml_node& node);
			BlendingStatePtr LoadBlendingState(const pugi::xml_node& node);
			MaterialParamsDescription LoadParams(const pugi::xml_node& node);

		protected:
			std::vector<RenderPipelinePtr> m_renderPipelines;
			std::vector<MaterialParamsDescription> m_paramDescriptions;
			String m_name;
		};
	}
}

#endif
