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
		SPtr<GpuParamsDescription> paramsDescription;
	};

	class Shader
	{
	public:
		ShaderType GetType() const { return m_type; }
		static ShaderPtr Create(const ShaderDescription& description);

		SPtr<GpuParamsDescription> GetParamsDescription() const { return m_paramsDescription; }

	protected:
		ShaderType m_type;
		SPtr<GpuParamsDescription> m_paramsDescription;
	};

} // video

} // sh

#endif
