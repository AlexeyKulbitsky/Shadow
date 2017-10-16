#include "GpuParams.h"
#include "Shader.h"
#include "RenderPipeline.h"
#include "Managers/HardwareBufferManager.h"

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

	void GpuParams::SetSampler(ShaderType shaderType, const String& name, const TexturePtr& texture)
	{
		const auto& desc = m_paramsInfo->GetParamsDescription(shaderType);
		auto it = desc->samplers.find(name);
		if (it != desc->samplers.end())
			m_samplers[m_paramsInfo->GetIndex(it->second.set, it->second.binding)]->Set(texture);
	}

	const SamplerPtr GpuParams::GetSampler(const String& name) const
	{
		for (u32 i = 0U; i < 6U; ++i)
		{
			const auto& desc = m_paramsInfo->GetParamsDescription(ShaderType(i));
			auto it = desc->samplers.find(name);
			if (it != desc->samplers.end())
				return m_samplers[m_paramsInfo->GetIndex(it->second.set, it->second.binding)];
		}
		return SamplerPtr();
	}

	const SamplerPtr GpuParams::GetSampler(const u32 set, const u32 binding)
	{
		const u32 index = m_paramsInfo->GetIndex(set, binding);
		return m_samplers[index];
	}

	void GpuParams::SetSampler(const String& name, const SamplerPtr& sampler)
	{
		for (u32 i = 0U; i < 6U; ++i)
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

	void GpuParams::SetSampler(const String& name, const TexturePtr& texture)
	{
		for (u32 i = 0U; i < 6U; ++i)
		{
			const auto& desc = m_paramsInfo->GetParamsDescription(ShaderType(i));
			auto it = desc->samplers.find(name);
			if (it != desc->samplers.end())
			{
				m_samplers[m_paramsInfo->GetIndex(it->second.set, it->second.binding)]->Set(texture);
				return;
			}
		}
	}

	void GpuParams::SetSampler(const SamplerPtr& sampler, const u32 set, const u32 binding)
	{
		const u32 index = m_paramsInfo->GetIndex(set, binding);
		m_samplers[index] = sampler;
	}
	
	GpuParamsPtr GpuParams::Create(const GpuPipelineParamsInfoPtr& pipelineParamsInfo)
	{
		return HardwareBufferManager::GetInstance()->CreateGpuParams(pipelineParamsInfo);
	}

	GpuParams::GpuParams(const GpuPipelineParamsInfoPtr& pipelineParamsInfo)
		: m_paramsInfo(pipelineParamsInfo)
	{
		//m_paramsDescriptions[ST_VERTEX] = pipelineParamsInfo->GetParamsDescription(ST_VERTEX);
		//m_paramsDescriptions[ST_FRAGMENT] = pipelineParamsInfo->GetParamsDescription(ST_FRAGMENT);
		//m_paramsDescriptions[ST_GEOMETRY] = pipelineParamsInfo->GetParamsDescription(ST_GEOMETRY);
		//m_paramsDescriptions[ST_TESSELATION_EVALUATION] = pipelineParamsInfo->GetParamsDescription(ST_TESSELATION_EVALUATION);
		//m_paramsDescriptions[ST_TESSELATION_CONTROL] = pipelineParamsInfo->GetParamsDescription(ST_TESSELATION_CONTROL);
		//m_paramsDescriptions[ST_COMPUTE] = pipelineParamsInfo->GetParamsDescription(ST_COMPUTE);

		u32 samplersCount = m_paramsInfo->GetElementsCount(GpuPipelineParamsInfo::ParamType::Sampler);
		m_samplers.resize(samplersCount);

		
		u32 paramsSize = 0U;
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
			for (auto& sampler : paramsDescription->samplers)
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
				const u32 index = m_paramsInfo->GetIndex(sampler.second.set, sampler.second.binding);
				m_samplers[index] = Sampler::Create(samplerDescription);
			}
		}
		if (paramsSize == 0)
			return;
		m_data = new u8[paramsSize];
		std::memset(m_data, 0, paramsSize);
	}

} // video

} // sh