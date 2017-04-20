#include "GpuParams.h"
#include "Shader.h"
#include "RenderPipeline.h"

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
	/*
	GpuParamsPtr GpuParams::Create( const GpuPipelineParamsDescription& pipelineParamsDesc )
	{
		GpuParamsPtr result;
		result.reset(new GpuParams(pipelineParamsDesc));
		return result;
	}

	GpuParamsPtr GpuParams::Create( const RenderPipelinePtr& pipeline )
	{
		GpuPipelineParamsDescription pipelineParamsDesc;

		if( pipeline->m_description.vertexShader)
			pipelineParamsDesc.vertexParams = pipeline->m_description.vertexShader->GetParamsDescription();

		if( pipeline->m_description.fragmentShader)
			pipelineParamsDesc.fragmentParams = pipeline->m_description.fragmentShader->GetParamsDescription();

		if( pipeline->m_description.geometryShader)
			pipelineParamsDesc.geometryParams = pipeline->m_description.geometryShader->GetParamsDescription();

		if( pipeline->m_description.tesselationControlShader)
			pipelineParamsDesc.tesselationControlParams = pipeline->m_description.tesselationControlShader->GetParamsDescription();

		if( pipeline->m_description.tesselationEvaluationShader)
			pipelineParamsDesc.tesselationEvaluationParams = pipeline->m_description.tesselationEvaluationShader->GetParamsDescription();

		if( pipeline->m_description.computeShader)
			pipelineParamsDesc.computeParams = pipeline->m_description.computeShader->GetParamsDescription();

		return Create(pipelineParamsDesc);

	}
	*/
	GpuParamsPtr GpuParams::Create(const GpuPipelineParamsInfoPtr& pipelineParamsInfo)
	{
		GpuParamsPtr result;
		result.reset(new GpuParams(pipelineParamsInfo));
		return result;
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
				m_samplers[sampler.first] = Sampler::Create(sampler.second.samplerDesc);
			}
			samplersCount += m_paramsDescriptions[i]->samplers.size();
		}

		totalSize += paramsSize;
		totalSize += sizeof(Sampler*) * samplersCount;
		m_data = new u8[totalSize];
		samplers = reinterpret_cast<Sampler*>(m_data + sizeof(Sampler*) * samplersCount);
	}

} // video

} // sh