#ifndef SHADOW_MATERIAL_PARAMS_INCLUDE
#define SHADOW_MATERIAL_PARAMS_INCLUDE

#include "../Globals.h"
#include "MaterialParam.h"

namespace sh
{

namespace video
{

	class MaterialParams
	{
	public:
		MaterialParams(const GpuParamsPtr& gpuParams);

		u32 GetParamsCount() const { return m_params.size(); }
		MaterialParam& GetParam(u32 idx) { return m_params[idx]; }
		const std::vector<MaterialParam>& GetParams() const { return m_params; }

	private:
		std::vector<MaterialParam> m_params;
	};

} // video

} // sh

#endif