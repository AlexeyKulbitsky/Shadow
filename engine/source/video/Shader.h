#ifndef SHADOW_SHADER_INCLUDE
#define SHADOW_SHADER_INCLUDE

#include "prerequisities/Prerequisities.h"
#include "video/Common.h"

namespace sh
{

namespace video
{
	struct SHADOW_API ShaderDescription
	{
		std::string source;
		std::string entryPoint;
		std::string language;
		ShaderType type;
		//SPtr<GpuParamsDescription> paramsDescription;
		//SPtr<GpuParamsDescription> autoParamsDescription;
	};

	class SHADOW_API Shader
	{
	public:
        ShaderType GetType() const;
		static ShaderPtr Create(const ShaderDescription& description);

		//SPtr<GpuParamsDescription> GetParamsDescription() const { return m_paramsDescription; }
		//SPtr<GpuParamsDescription> GetAutoParamsDescription() const { return m_autoParamsDescription; }

	protected:
		ShaderType m_type;
		//SPtr<GpuParamsDescription> m_paramsDescription;
		//SPtr<GpuParamsDescription> m_autoParamsDescription;
	};

} // video

} // sh

#endif
