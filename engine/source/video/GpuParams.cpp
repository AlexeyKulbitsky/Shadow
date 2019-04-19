#include "video/GpuParams.h"

namespace sh
{

namespace video
{

	GpuParams::~GpuParams()
	{
		if (m_data != nullptr)
		{
			delete[] m_data;
			m_data = nullptr;
		}
	}

    const uint8_t* GpuParams::GetData() const 
    { 
        return m_data; 
    }

    const std::vector<SamplerPtr>& GpuParams::GetSamplers() const 
    { 
        return m_samplers; 
    }

	void GpuParams::SetSampler(ShaderType shaderType, const std::string& name, const TexturePtr& texture)
	{
		/*const auto& desc = m_paramsInfo->GetParamsDescription(shaderType);
		auto it = desc->samplers.find(name);
		if (it != desc->samplers.end())
			m_samplers[m_paramsInfo->GetIndex(it->second.set, it->second.binding)]->Set(texture);*/
	}

	const SamplerPtr GpuParams::GetSampler(const std::string& name) const
	{
		for (uint32_t i = 0U; i < 6U; ++i)
		{
			const auto& desc = m_paramsInfo->GetParamsDescription(ShaderType(i));
			auto it = desc->samplers.find(name);
			if (it != desc->samplers.end())
				return m_samplers[m_paramsInfo->GetIndex(it->second.set, it->second.binding)];
		}
		return SamplerPtr();
	}

	const SamplerPtr GpuParams::GetSampler(const uint32_t set, const uint32_t binding)
	{
		const uint32_t index = m_paramsInfo->GetIndex(set, binding);
		return m_samplers[index];
	}

	void GpuParams::SetSampler(const std::string& name, const SamplerPtr& sampler)
	{
		for (uint32_t i = 0U; i < 6U; ++i)
		{
			const auto& desc = m_paramsInfo->GetParamsDescription(ShaderType(i));
			auto it = desc->samplers.find(name);
			if (it != desc->samplers.end())
			{
				m_samplers[m_paramsInfo->GetIndex(it->second.set, it->second.binding)] = sampler;
				return;
			}
		}
	}

	void GpuParams::SetSampler(const std::string& name, const TexturePtr& texture)
	{
		/*for (uint32_t i = 0U; i < 6U; ++i)
		{
			const auto& desc = m_paramsInfo->GetParamsDescription(ShaderType(i));
			auto it = desc->samplers.find(name);
			if (it != desc->samplers.end())
			{
				m_samplers[m_paramsInfo->GetIndex(it->second.set, it->second.binding)]->Set(texture);
				return;
			}
		}*/
	}

	void GpuParams::SetSampler(const SamplerPtr& sampler, const uint32_t set, const uint32_t binding)
	{
		const uint32_t index = m_paramsInfo->GetIndex(set, binding);
		m_samplers[index] = sampler;
	}

    const GpuPipelineParamsInfoPtr& GpuParams::GetParamsInfo() const 
    { 
        return m_paramsInfo; 
    }
	
	GpuParamsPtr GpuParams::Create(const GpuPipelineParamsInfoPtr& pipelineParamsInfo)
	{
        GpuParamsPtr result;
        result.reset(new GpuParams(pipelineParamsInfo));
        return result;
	}

	GpuParams::GpuParams(const GpuPipelineParamsInfoPtr& pipelineParamsInfo)
		: m_paramsInfo(pipelineParamsInfo)
	{
		uint32_t samplersCount = m_paramsInfo->GetElementsCount(GpuPipelineParamsInfo::ParamType::Sampler);
		m_samplers.resize(samplersCount);
		
        uint32_t paramsSize = 0U;
		for (size_t i = 0; i < 6; ++i)
		{
			const auto& paramsDescription = m_paramsInfo->GetParamsDescription(static_cast<ShaderType>(i));
			if (!paramsDescription)
				continue;

			// Collect data params
			for (auto& param : paramsDescription->params)
			{
				param.second.offset = paramsSize;
				paramsSize += param.second.size;
			}

			// Collest samplers
			/*for (auto& sampler : paramsDescription->samplers)
			{
				SamplerDescription samplerDescription;
				samplerDescription.type = sampler.second.type;

				switch (sampler.second.type)
				{
				case GPOT_SAMPLER_1D:
					break;
				case GPOT_SAMPLER_2D:
					break;
				case GPOT_SAMPLER_3D:
					break;
				case GPOT_SAMPLER_CUBE:
					break;
				default:
					break;
				}
				const uint32_t index = m_paramsInfo->GetIndex(sampler.second.set, sampler.second.binding);
				m_samplers[index] = Sampler::Create(samplerDescription);
			}*/
		}
		if (paramsSize == 0)
			return;
		m_data = new uint8_t[paramsSize];
		std::memset(m_data, 0, paramsSize);
	}

} // video

} // sh