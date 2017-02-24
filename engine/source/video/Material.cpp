#include "Material.h"
#include "RenderTechnique.h"
#include "RenderPipeline.h"

namespace sh
{
	namespace video
	{
		void Material::SetRenderTechnique(const RenderTechniquePtr& technique) 
		{ 
			m_renderTechnique = technique; 
			size_t pipelinesCount = technique->GetRenderPipelinesCount();
			if (m_renderPipelines.size() != pipelinesCount)
			{
				m_renderPipelines.resize(pipelinesCount);
			}

			for (size_t i = 0; i < pipelinesCount; ++i)
			{			
				RenderPipelinePtr pipeline = technique->GetRenderPipeline(i);
				m_renderPipelines[i] = pipeline->Clone();
			}
		}
	}
}