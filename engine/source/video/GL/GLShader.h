#ifndef SHADOW_GL_SHADER_INCLUDE
#define SHADOW_GL_SHADER_INCLUDE

#include "video/Shader.h"
#include "video/GL/GLCommon.h"

namespace sh
{

namespace video
{
	
	class GLShader : public Shader
	{
		friend class GLRenderStateManager;
	public:
        GLuint GetGLId() const;

	private:
		GLShader(const ShaderDescription& description);

	private:
		GLuint m_glID;
	};

} // video

} // sh


#endif