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
		friend class RenderStateManager;
	public:
		enum class ParamType
		{
			Sampler = 0U,

			Count
		};

		SPtr<GpuParamsDescription> GetParamsDescription(ShaderType shaderType) const { return m_paramsDescription[shaderType]; }
		virtual ~GpuPipelineParamsInfo() { }

		// Get elements count of specified type
		u32 GetElementsCount(ParamType type) const { return m_elementsPerTypeCount[static_cast<u32>(type)]; }
		
		// Get index in vector of selements in GPU Params
		u32 GetIndex(const u32 set, const u32 binding) const { return m_setInfos[set].bindingIndices[binding]; }

		static GpuPipelineParamsInfoPtr Create(const GpuPipelineParamsDescription& description);

	protected:
		GpuPipelineParamsInfo(const GpuPipelineParamsDescription& description);

	protected:
		std::array<SPtr<GpuParamsDescription>, 6U> m_paramsDescription;

		struct SetInfo
		{
			std::vector<u32> bindingIndices; // indices to map in GPU params
		};

		std::vector<SetInfo> m_setInfos;
		std::array<u32, static_cast<u32>(ParamType::Count)> m_elementsPerTypeCount;
	};

} // video

} // sh

#endif