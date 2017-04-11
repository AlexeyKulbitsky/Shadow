#include "GpuPipelineParamsInfo.h"

namespace sh
{

namespace video
{
	
	GpuPipelineParamsInfo::GpuPipelineParamsInfo( const GpuPipelineParamsDescription& description )
	{
		m_paramsDescription[ST_VERTEX] = description.vertexParams;
		m_paramsDescription[ST_FRAGMENT] = description.fragmentParams;
		m_paramsDescription[ST_GEOMETRY] = description.geometryParams;
		m_paramsDescription[ST_TESSELATION_EVALUATION] = description.tesselationEvaluationParams;
		m_paramsDescription[ST_TESSELATION_CONTROL] = description.tesselationControlParams;
		m_paramsDescription[ST_COMPUTE] = description.computeParams;
	}

} // video

} // sh