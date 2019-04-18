#include "video/GpuPipelineParamsInfo.h"

namespace sh
{

namespace video
{
    std::shared_ptr<GpuParamsDescription> GpuPipelineParamsInfo::GetParamsDescription(ShaderType shaderType) const 
    { 
        return m_paramsDescription[shaderType]; 
    }

    GpuPipelineParamsInfo::~GpuPipelineParamsInfo() 
    { 
    }

    uint32_t GpuPipelineParamsInfo::GetElementsCount(ParamType type) const 
    { 
        return m_elementsPerTypeCount[static_cast<uint32_t>(type)]; 
    }

    uint32_t GpuPipelineParamsInfo::GetIndex(const uint32_t set, const uint32_t binding) const 
    { 
        return m_setInfos[set].bindingIndices[binding]; 
    }

	GpuPipelineParamsInfoPtr GpuPipelineParamsInfo::Create( const GpuPipelineParamsDescription& description )
	{
        GpuPipelineParamsInfoPtr result;
        result.reset(new GpuPipelineParamsInfo(description));
        return result;
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
				const uint32_t set = sampler.second.set;
				const uint32_t binding = sampler.second.binding;

				if (set >= m_setInfos.size())
				{
					m_setInfos.resize(set + 1U);
				}
				if (binding >= m_setInfos[set].bindingIndices.size())
				{
					m_setInfos[set].bindingIndices.resize(binding + 1U);
				}
                uint32_t index = m_elementsPerTypeCount[static_cast<uint32_t>(ParamType::Sampler)]++;
				m_setInfos[set].bindingIndices[binding] = index;
			}
		}
	}

} // video

} // sh