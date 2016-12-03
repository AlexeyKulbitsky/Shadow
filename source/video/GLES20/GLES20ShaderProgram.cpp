#include "GLES20ShaderProgram.h"
#include <pugixml.hpp>
#include "../VertexDeclaration.h"
#include "../UniformBuffer.h"
#include "GLES20Sampler.h"
#include "../pempek_assert.h"

using namespace sh;
using namespace video;

GLES20ShaderProgram::GLES20ShaderProgram()
{
}

///////////////////////////////////////////////////////////////////////

void GLES20ShaderProgram::Load(const char* filename)
{
}

///////////////////////////////////////////////////////////////////////

void GLES20ShaderProgram::Load(const pugi::xml_node &node)
{
	// Load vertex shader data
	pugi::xml_node vertexShaderSrcNode = node.child("vertexShader");
	if (vertexShaderSrcNode)
	{
		const char* vertexShaderSource = vertexShaderSrcNode.child_value();
		m_vertexShaderID = CompileShader(GL_VERTEX_SHADER, vertexShaderSource);
	}

	// Load fragment shader data
	pugi::xml_node fragmentShaderSrcNode = node.child("fragmentShader");
	if (fragmentShaderSrcNode)
	{
		const char* fragmentShaderSource = fragmentShaderSrcNode.child_value();
		m_fragmentShaderID = CompileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);
	}

	// Create shader program
	m_programID = glCreateProgram();

	glAttachShader(m_programID, m_vertexShaderID);
	glAttachShader(m_programID, m_fragmentShaderID);

	glLinkProgram(m_programID);

	GLint linked;
	glGetProgramiv(m_programID, GL_LINK_STATUS, &linked);
	if (!linked)
	{
		GLsizei len;
		glGetProgramiv(m_programID, GL_INFO_LOG_LENGTH, &len);

		char* log = new char[len + 1];
		glGetProgramInfoLog(m_programID, len, &len, log);

		SH_ASSERT(0, "ERROR");
	}
}

///////////////////////////////////////////////////////////////////////

ShaderProgram* GLES20ShaderProgram::Clone()
{
	GLES20ShaderProgram* result = new GLES20ShaderProgram();
	result->m_fragmentShaderID = m_fragmentShaderID;
	result->m_vertexShaderID = m_vertexShaderID;
	result->m_programID = m_programID;
	return result;
}

///////////////////////////////////////////////////////////////////////

void GLES20ShaderProgram::Unload()
{
	glDeleteShader(m_vertexShaderID);
	glDeleteShader(m_fragmentShaderID);
	glDeleteProgram(m_programID);
}

///////////////////////////////////////////////////////////////////////

void GLES20ShaderProgram::BindProgram()
{
	glUseProgram(m_programID);
}

///////////////////////////////////////////////////////////////////////

void GLES20ShaderProgram::UnbindProgram()
{
	glUseProgram(0);
}

///////////////////////////////////////////////////////////////////////

GLuint GLES20ShaderProgram::CompileShader(GLenum shaderType, const c8* source)
{
	GLuint shader;
	GLint compiled;

	shader = glCreateShader(shaderType);

	if (shader == 0)
	{
		return 0;
	}

	glShaderSource(shader, 1, &source, NULL);

	// Compile the shader
	glCompileShader(shader);

	// Check the compile status
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);

	if (!compiled)
	{
		GLint infoLen = 0;

		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);

		if (infoLen > 1)
		{
			char *infoLog = new char[infoLen];

			glGetShaderInfoLog(shader, infoLen, NULL, infoLog);

			delete[] infoLog;
		}	

		glDeleteShader(shader);
		return 0;
	}

	return shader;
}