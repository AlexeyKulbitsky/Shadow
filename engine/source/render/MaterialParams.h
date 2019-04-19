#ifndef SHADOW_MATERIAL_PARAMS_INCLUDE
#define SHADOW_MATERIAL_PARAMS_INCLUDE

#include "render/MaterialParam.h"

namespace sh
{

namespace video
{

	class SHADOW_API MaterialParams
	{
	public:
		MaterialParams(const GpuParamsPtr& gpuParams);

        uint32_t GetParamsCount() const;
        MaterialParam* GetParam(uint32_t idx);
        const std::vector<MaterialParam>& GetParams() const;
        const std::vector<MaterialSamplerParam>& GetSamplerParams() const;

	private:
		void ReadDataParams(const GpuParamsPtr& gpuParams);
		void ReadSamplers(const GpuParamsPtr& gpuParams);

	private:
		std::vector<MaterialParam> m_params;
		std::vector<MaterialSamplerParam> m_samplerParams;
	};

} // video

} // sh

#endif