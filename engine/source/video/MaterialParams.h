#ifndef SHADOW_MATERIAL_PARAMS_INCLUDE
#define SHADOW_MATERIAL_PARAMS_INCLUDE

#include "../Globals.h"
#include "MaterialParam.h"

namespace sh
{

namespace video
{
	struct MaterialParamsDescription
	{
		Map<String, MaterialParamDescription> params;
	};

	class MaterialParams
	{
	public:
		MaterialParams(const GpuParamsPtr& gpuParams);
		MaterialParams(const GpuParamsPtr& gpuParams, const MaterialParamsDescription& description);

		u32 GetParamsCount() const { return m_params.size(); }
		MaterialParam* GetParam(u32 idx) { return &m_params[idx]; }
		const std::vector<MaterialParam>& GetParams() const { return m_params; }
		const std::vector<MaterialSamplerParam>& GetSamplerParams() const { return m_samplerParams; }

	private:
		void ReadDataParams(const GpuParamsPtr& gpuParams);
		void ReadSamplers(const GpuParamsPtr& gpuParams);

	private:
		MaterialParamsDescription m_description;
		std::vector<MaterialParam> m_params;
		std::vector<MaterialSamplerParam> m_samplerParams;
	};

} // video

} // sh

#endif