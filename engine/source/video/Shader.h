#ifndef SHADOW_SHADER_INCLUDE
#define SHADOW_SHADER_INCLUDE

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
	};

	class Shader
	{
	public:
		ShaderType GetType() const { return m_type; }
		static ShaderPtr Create(const ShaderDescription& description);
	protected:
		ShaderType m_type;
	};

} // video

} // sh

#endif
