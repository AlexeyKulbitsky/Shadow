#ifndef SHADOW_GPU_PIPELINE_PARAMS_INFO_INCLUDE
#define SHADOW_GPU_PIPELINE_PARAMS_INFO_INCLUDE

#include "video/GpuParamsDescription.h"

namespace sh
{

namespace video
{
	struct SHADOW_API GpuPipelineParamsDescription
	{
		std::shared_ptr<GpuParamsDescription> vertexParams;
        std::shared_ptr<GpuParamsDescription> fragmentParams;
        std::shared_ptr<GpuParamsDescription> geometryParams;
        std::shared_ptr<GpuParamsDescription> tesselationEvaluationParams;
        std::shared_ptr<GpuParamsDescription> tesselationControlParams;
        std::shared_ptr<GpuParamsDescription> computeParams;
	};

	class SHADOW_API GpuPipelineParamsInfo
	{
	public:
		enum class ParamType
		{
			Sampler = 0U,

			Count
		};

        std::shared_ptr<GpuParamsDescription> GetParamsDescription(ShaderType shaderType) const;
        virtual ~GpuPipelineParamsInfo();

		// Get elements count of specified type
        uint32_t GetElementsCount(ParamType type) const;
		
		// Get index in vector of selements in GPU Params
        uint32_t GetIndex(const uint32_t set, const uint32_t binding) const;

		static GpuPipelineParamsInfoPtr Create(const GpuPipelineParamsDescription& description);

	protected:
		GpuPipelineParamsInfo(const GpuPipelineParamsDescription& description);

	protected:
		std::array<std::shared_ptr<GpuParamsDescription>, 6U> m_paramsDescription;

		struct SetInfo
		{
			std::vector<uint32_t> bindingIndices; // indices to map in GPU params
		};

		std::vector<SetInfo> m_setInfos;
		std::array<uint32_t, static_cast<uint32_t>(ParamType::Count)> m_elementsPerTypeCount;
	};

} // video

} // sh

#endif