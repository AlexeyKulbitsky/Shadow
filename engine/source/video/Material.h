#ifndef SHADOW_MATERIAL_INCLUDE
#define SHADOW_MATERIAL_INCLUDE

#include "../Globals.h"
#include "MaterialParams.h"
#include "GpuParams.h"

namespace sh
{
	namespace video
	{
		class Material : public Resource
		{
		public:
			Material() {}
			void Load(const pugi::xml_node &node);
			void Save(pugi::xml_node &parent);
			void SetRenderTechnique(const RenderTechniquePtr& technique);
			void SetRenderTechnique( const String& techniqueName);

			const RenderTechniquePtr& GetRenderTechnique() { return m_renderTechnique; }

			size_t GetRenderPipelinesCount() const { return m_renderPipelines.size(); }
			const RenderPipelinePtr& GetRenderPipeline(size_t index = 0U) { return m_renderPipelines[index]; }
			const String& GetName() const { return m_name; }

			// Common params (usually constant) that will be sent to GPU
			const GpuParamsPtr& GetCommonGpuParams() const { return m_commonGpuParams; }
			// Common params (usually constant) that can be modified by user
			const MaterialParamsPtr& GetParams() const { return m_commonParams; }

			// Auto params (usually updated each frame such as matrices, etc.) that will be sent to GPU
			const GpuParamsPtr& GetAutoGpuParams() const { return m_autoGpuParams; }
			// Auto params (usually updated each frame such as matrices, etc.) that can be modified by user
			const MaterialParamsPtr& GetAutoParams() const { return m_autoParams; }

		private:
			String m_name = "default";
			RenderTechniquePtr m_renderTechnique;
			std::vector<RenderPipelinePtr> m_renderPipelines;

			GpuParamsPtr m_commonGpuParams;
			MaterialParamsPtr m_commonParams;

			GpuParamsPtr m_autoGpuParams;
			MaterialParamsPtr m_autoParams;
		};
	}
}

#endif