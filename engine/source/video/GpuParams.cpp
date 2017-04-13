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

	GpuParams::GpuParams( const GpuPipelineParamsDescription& pipelineParamsDesc )
	{
		m_paramsDescriptions[ST_VERTEX] = pipelineParamsDesc.vertexParams;
		m_paramsDescriptions[ST_FRAGMENT] = pipelineParamsDesc.fragmentParams;
		m_paramsDescriptions[ST_GEOMETRY] = pipelineParamsDesc.geometryParams;
		m_paramsDescriptions[ST_TESSELATION_EVALUATION] = pipelineParamsDesc.tesselationControlParams;
		m_paramsDescriptions[ST_TESSELATION_CONTROL] = pipelineParamsDesc.tesselationEvaluationParams;
		m_paramsDescriptions[ST_COMPUTE] = pipelineParamsDesc.computeParams;

		u32 totalSize = 0U;
		// For each params description in each stage
		for( size_t i = 0; i < 6; ++i )
		{
			if(!m_paramsDescriptions[i] )
				continue;

			for( auto& param : m_paramsDescriptions[i]->params )
			{
				param.second.offset = totalSize;
				totalSize += param.second.size;
			}
		}

		m_data = new u8[totalSize];
	}

} // video

} // sh