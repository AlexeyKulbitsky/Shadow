#ifndef SHADOW_GLES20_SHADER_INCLUDE
#define SHADOW_GLES20_SHADER_INCLUDE

#include "../Shader.h"
#include "GLES20Common.h"

namespace sh
{

namespace video
{
	
	class GLES20Shader : public Shader
	{
		friend class GLES20Driver;
	public:
		GLuint GetGLId() const { return m_glID; }

	private:
		GLES20Shader(const ShaderDescription& description);

	private:
		GLuint m_glID;
	};

} // video

} // sh


#endif