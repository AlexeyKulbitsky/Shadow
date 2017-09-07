#ifndef SHADOW_SHADER_INCLUDE
#define SHADOW_SHADER_INCLUDE

#include "../Globals.h"

namespace sh
{

namespace video
{
	struct ShaderDescription
	{
		String source;
		String entryPoint;
		String language;
		ShaderType type;
		//SPtr<GpuParamsDescription> paramsDescription;
		//SPtr<GpuParamsDescription> autoParamsDescription;
	};

	class Shader
	{
	public:
		ShaderType GetType() const { return m_type; }
		static ShaderPtr Create(const ShaderDescription& description);

		SPtr<GpuParamsDescription> GetParamsDescription() const { return m_paramsDescription; }
		SPtr<GpuParamsDescription> GetAutoParamsDescription() const { return m_autoParamsDescription; }

	protected:
		ShaderType m_type;
		SPtr<GpuParamsDescription> m_paramsDescription;
		SPtr<GpuParamsDescription> m_autoParamsDescription;
	};

} // video

} // sh

#endif
