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
			void Load(const pugi::xml_node &node);
			void Save(pugi::xml_node &parent);
			void SetRenderTechnique(const RenderTechniquePtr& technique);

			const RenderTechniquePtr& GetRenderTechnique() { return m_renderTechnique; }

			size_t GetRenderPipelinesCount() const { return m_renderPipelines.size(); }
			const RenderPipelinePtr& GetRenderPipeline(size_t index = 0U) { return m_renderPipelines[index]; }
			const String& GetName() const { return m_name; }

		private:
			String m_name = "default";
			RenderTechniquePtr m_renderTechnique;
			std::vector<RenderPipelinePtr> m_renderPipelines;
		};
	}
}

#endif