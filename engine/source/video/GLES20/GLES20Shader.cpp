#include "GLES20Shader.h"

namespace sh
{

namespace video
{
	GLES20Shader::GLES20Shader(const ShaderDescription& description)
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
				SH_ASSERT(0, "Invalid shader type for GLES20 shader!");
				return;
		}

		GLint compiled;

		m_glID = glCreateShader(shaderType);
		
		if (m_glID == 0)
		{
			SH_ASSERT(0, "Can not compile shader!");
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

				delete[] infoLog;
			}	

			glDeleteShader(m_glID);
			return;
		}
	}

} // video

} // sh