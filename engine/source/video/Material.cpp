#include "Material.h"
#include "RenderTechnique.h"
#include "RenderPass.h"

namespace sh
{
	namespace video
	{
		void Material::SetRenderTechnique(RenderTechnique* technique) 
		{ 
			m_renderTechnique = technique; 
			m_renderPasses.resize(technique->GetRenderPassesCount());

			for (size_t i = 0, sz = technique->GetRenderPassesCount(); i < sz; ++i)
			{
				RenderPass* pass = technique->GetRenderPass(i);
				m_renderPasses[i] = pass->Clone();
			}
		}
	}
}