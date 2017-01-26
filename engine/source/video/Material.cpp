#include "Material.h"
#include "RenderTechnique.h"
#include "RenderPipeline.h"

namespace sh
{
	namespace video
	{
		void Material::SetRenderTechnique(RenderTechnique* technique) 
		{ 
			m_renderTechnique = technique; 
			u32 pipelinesCount = technique->GetRenderPipelinesCount();
			m_renderPipelines.resize(pipelinesCount);

			for (size_t i = 0; i < pipelinesCount; ++i)
			{
				RenderPipeline* pipeline = technique->GetRenderPipeline(i);
				m_renderPipelines[i] = pipeline->Clone();
			}
		}
	}
}