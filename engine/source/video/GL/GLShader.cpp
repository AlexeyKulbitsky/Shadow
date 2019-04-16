#include "video/GL/GLShader.h"

namespace sh
{

namespace video
{
    GLuint GLShader::GetGLId() const 
    { 
        return m_glID; 
    }

	GLShader::GLShader(const ShaderDescription& description)
	{
		GLenum shaderType;
		switch (description.type)
		{
			case ST_VERTEX:
				shaderType = GL_VERTEX_SHADER;
				break;
			case ST_FRAGMENT:
				shaderType = GL_FRAGMENT_SHADER;
				break;
			default:
				assert(0 && "Invalid shader type for GL shader!");
				return;
		}

		GLint compiled;

		m_glID = glCreateShader(shaderType);
		
		if (m_glID == 0)
		{
            assert(0 && "Can not compile shader!");
			return;
		}

		const char* src = description.source.data();
		glShaderSource(m_glID, 1, &src, NULL);

		// Compile the shader
		glCompileShader(m_glID);

		// Check the compile status
		glGetShaderiv(m_glID, GL_COMPILE_STATUS, &compiled);

		if (!compiled)
		{
			GLint infoLen = 0;

			glGetShaderiv(m_glID, GL_INFO_LOG_LENGTH, &infoLen);

			if (infoLen > 1)
			{
				char *infoLog = new char[infoLen];

				glGetShaderInfoLog(m_glID, infoLen, NULL, infoLog);
                assert(0 && "Error while compiling GL shader ! Error message: %s", infoLog);

				delete[] infoLog;
			}	

			glDeleteShader(m_glID);
			return;
		}
	}

} // video

} // sh