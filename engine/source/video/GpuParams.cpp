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
		auto it = m_paramsDescriptions[shaderType]->samplers.find(name);
		if (it == m_paramsDescriptions[shaderType]->samplers.end())
		{
			SH_ASSERT(0, "Can not find sampler!");
			return;
		}
		m_samplers[it->first]->Set(texture);
	}

	const SamplerPtr GpuParams::GetSampler(const String& name) const
	{
		auto it = m_samplers.find(name);
		if (it == m_samplers.end())
		{
			return it->second;
		}
		return SamplerPtr();
	}

	void GpuParams::SetSampler(const String& name, const SamplerPtr& sampler)
	{
		auto it = m_samplers.find(name);
		if (it != m_samplers.end())
		{
			it->second = sampler;
			return;
		}
	}

	void GpuParams::SetSampler(const String& name, const TexturePtr& texture)
	{
		auto it = m_samplers.find(name);
		if (it != m_samplers.end())
		{
			it->second->Set(texture);
			return;
		}
	}
	
	GpuParamsPtr GpuParams::Create(const GpuPipelineParamsInfoPtr& pipelineParamsInfo)
	{
		return HardwareBufferManager::GetInstance()->CreateGpuParams(pipelineParamsInfo);
	}

	GpuParams::GpuParams(const GpuPipelineParamsInfoPtr& pipelineParamsInfo)
	{
		m_paramsDescriptions[ST_VERTEX] = pipelineParamsInfo->GetParamsDescription(ST_VERTEX);
		m_paramsDescriptions[ST_FRAGMENT] = pipelineParamsInfo->GetParamsDescription(ST_FRAGMENT);
		m_paramsDescriptions[ST_GEOMETRY] = pipelineParamsInfo->GetParamsDescription(ST_GEOMETRY);
		m_paramsDescriptions[ST_TESSELATION_EVALUATION] = pipelineParamsInfo->GetParamsDescription(ST_TESSELATION_EVALUATION);
		m_paramsDescriptions[ST_TESSELATION_CONTROL] = pipelineParamsInfo->GetParamsDescription(ST_TESSELATION_CONTROL);
		m_paramsDescriptions[ST_COMPUTE] = pipelineParamsInfo->GetParamsDescription(ST_COMPUTE);

		
		u32 totalSize = 0U;
		u32 paramsSize = 0U;
		u32 samplersCount = 0U;
		// For each params description in each stage
		for( size_t i = 0; i < 6; ++i )
		{
			if(!m_paramsDescriptions[i] )
				continue;

			// Collect data params
			for( auto& param : m_paramsDescriptions[i]->params )
			{
				param.second.offset = paramsSize;
				paramsSize += param.second.size;
			}

			// Collect samplers
			for(auto& sampler : m_paramsDescriptions[i]->samplers)
			{
				SamplerDescription samplerDescription;
				m_samplers[sampler.first] = Sampler::Create(samplerDescription);
			}
			samplersCount += m_paramsDescriptions[i]->samplers.size();
		}

		totalSize += paramsSize;
		//totalSize += sizeof(Sampler*) * samplersCount;
		m_data = new u8[totalSize];
		std::memset(m_data, 0, totalSize);
		//samplers = reinterpret_cast<Sampler*>(m_data + sizeof(Sampler*) * samplersCount);
	}

} // video

} // sh