#ifndef SHADOW_MATERIAL_INCLUDE
#define SHADOW_MATERIAL_INCLUDE

#include "../Globals.h"
#include "MaterialParams.h"
#include "GpuParams.h"

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

			const GpuParamsPtr& GetCommonGpuParams() const { return m_commonGpuParams; }
			const GpuParamsPtr& GetTransformGpuParams() const { return m_transfromsGpuParams; }

			const std::vector<SPtr<MaterialParam>>& GetTransformParams() const { return m_transformParams; }

		private:
			String m_name = "default";
			RenderTechniquePtr m_renderTechnique;
			std::vector<RenderPipelinePtr> m_renderPipelines;

			GpuParamsPtr m_commonGpuParams;
			GpuParamsPtr m_transfromsGpuParams;
			std::vector<SPtr<MaterialParam>> m_transformParams;
		};
	}
}

#endif