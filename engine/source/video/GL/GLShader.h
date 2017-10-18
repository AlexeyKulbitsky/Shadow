#ifndef SHADOW_GL_SHADER_INCLUDE
#define SHADOW_GL_SHADER_INCLUDE

#include "../Shader.h"
#include "GLCommon.h"

namespace sh
{

namespace video
{
	
	class GLShader : public Shader
	{
		//friend class GLES20Driver;
		friend class GLRenderStateManager;
	public:
		GLuint GetGLId() const { return m_glID; }

	private:
		GLShader(const ShaderDescription& description);

	private:
		GLuint m_glID;
	};

} // video

} // sh


#endif