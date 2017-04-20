#ifndef SHADOW_GPU_PIPELINE_PARAMS_INFO_INCLUDE
#define SHADOW_GPU_PIPELINE_PARAMS_INFO_INCLUDE

#include "GpuParamsDescription.h"

namespace sh
{

namespace video
{
	struct GpuPipelineParamsDescription
	{
		SPtr<GpuParamsDescription> vertexParams;
		SPtr<GpuParamsDescription> fragmentParams;
		SPtr<GpuParamsDescription> geometryParams;
		SPtr<GpuParamsDescription> tesselationEvaluationParams;
		SPtr<GpuParamsDescription> tesselationControlParams;
		SPtr<GpuParamsDescription> computeParams;
	};

	class GpuPipelineParamsInfo
	{
		friend class RenderPipeline;
	public:
		SPtr<GpuParamsDescription> GetParamsDescription(ShaderType shaderType) const { return m_paramsDescription[shaderType]; }

	protected:
		GpuPipelineParamsInfo(const GpuPipelineParamsDescription& description);

	private:
		std::array<SPtr<GpuParamsDescription>, 6U> m_paramsDescription;

		struct SetInfo
		{

		};
	};

} // video

} // sh

#endif