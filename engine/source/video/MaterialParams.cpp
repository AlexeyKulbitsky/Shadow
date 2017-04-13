#include "MaterialParams.h"

#include "GpuParams.h"

namespace sh
{

	namespace video
	{
		MaterialParams::MaterialParams(const GpuParamsPtr& gpuParams)
		{
			const u8* dataPtr = gpuParams->GetData();

			for (size_t shaderIdx = 0; shaderIdx < 6U; ++shaderIdx)
			{
				const auto& desc = gpuParams->GetDescription((ShaderType)shaderIdx);
				if (!desc)
					continue;
				for (const auto& param : desc->params)
				{
					if (param.second.name == "matWVP")
					{
						MaterialParam matParam(MaterialParamType::MatrixWorldViewProjection, 
											   const_cast<u8*>(dataPtr + param.second.offset));
						m_params.push_back(matParam);
					}
				}
			}
		}


	} // video

} // sh