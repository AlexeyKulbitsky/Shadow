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
					const auto it = matParamsMap.find(param.second.name);

					MaterialParamType type;
					switch (param.second.type)
					{
					case GPDT_FLOAT1:
						type = MaterialParamType::Float;
						break;
					case GPDT_FLOAT2:
						type = MaterialParamType::Float2;
						break;
					case GPDT_FLOAT3:
						type = MaterialParamType::Float3;
						break;
					case GPDT_FLOAT4:
						type = MaterialParamType::Float4;
						break;
					case GPDT_INT1:
						type = MaterialParamType::Int;
						break;
					case GPDT_INT2:
						type = MaterialParamType::Int2;
						break;
					case GPDT_INT3:
						type = MaterialParamType::Int3;
						break;
					case GPDT_INT4:
						type = MaterialParamType::Int4;
						break;
					case GPDT_MATRIX4:
						type = MaterialParamType::Matrix4;
						break;
					case GPDT_MATRIX3:
						type = MaterialParamType::Matrix3;
						break;
					default:
						break;
					}

					
					if (it != matParamsMap.end())
					{
						MaterialParam matParam(param.second.name, type, it->second, const_cast<u8*>(dataPtr + param.second.offset));
						m_params.push_back(matParam);
					}
					else
					{
						MaterialParam matParam(param.second.name, type, MaterialParamUsage::Undefined, const_cast<u8*>(dataPtr + param.second.offset));
						m_params.push_back(matParam);
					}
				}
			}
		}


	} // video

} // sh