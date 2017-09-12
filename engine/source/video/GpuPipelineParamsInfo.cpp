#include "GpuPipelineParamsInfo.h"
#include "Managers/RenderStateManager.h"

namespace sh
{

namespace video
{
	
	GpuPipelineParamsInfoPtr GpuPipelineParamsInfo::Create( const GpuPipelineParamsDescription& description )
	{
		return RenderStateManager::GetInstance()->CreateGpuPipelineParamsInfo(description);
	}

	GpuPipelineParamsInfo::GpuPipelineParamsInfo( const GpuPipelineParamsDescription& description )
	{
		m_paramsDescription[ST_VERTEX] = description.vertexParams;
		m_paramsDescription[ST_FRAGMENT] = description.fragmentParams;
		m_paramsDescription[ST_GEOMETRY] = description.geometryParams;
		m_paramsDescription[ST_TESSELATION_EVALUATION] = description.tesselationEvaluationParams;
		m_paramsDescription[ST_TESSELATION_CONTROL] = description.tesselationControlParams;
		m_paramsDescription[ST_COMPUTE] = description.computeParams;

		for (auto& el : m_elementsPerTypeCount)
		{
			el = 0U;
		}

		for (const auto& desc : m_paramsDescription)
		{
			if (!desc)
				continue;

			// Collect params
			for (const auto& param : desc->params)
			{
			}

			// Collect samplers
			for (const auto& sampler : desc->samplers)
			{
				const u32 set = sampler.second.set;
				const u32 binding = sampler.second.binding;

				if (set >= m_setInfos.size())
				{
					m_setInfos.resize(set + 1U);
				}
				if (binding >= m_setInfos[set].bindingIndices.size())
				{
					m_setInfos[set].bindingIndices.resize(binding + 1U);
				}
				u32 index = m_elementsPerTypeCount[static_cast<u32>(ParamType::Sampler)]++;
				m_setInfos[set].bindingIndices[binding] = index;
			}
		}
	}

} // video

} // sh