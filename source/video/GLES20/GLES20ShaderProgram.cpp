#include "GLES20ShaderProgram.h"
#include <pugixml.hpp>
#include "../Vertex.h"
#include "../pempek_assert.h"

using namespace sh;
using namespace video;

GLES20ShaderProgram::GLES20ShaderProgram()
{

}

///////////////////////////////////////////////////////////////////////

void GLES20ShaderProgram::Load(const c8* filename)
{
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file(filename);
	pugi::xml_node shaderProgramNode = doc.first_child();

	// Load uniforms
	pugi::xml_node uniformsNode = shaderProgramNode.child("uniforms");
	LoadUniforms(uniformsNode);

	// Load attributes
	pugi::xml_node attributesNode = shaderProgramNode.child("attributes");
	LoadAttributes(attributesNode);

	// Load vertex shader data
	pugi::xml_node vertexShaderSrcNode = shaderProgramNode.child("vertexShader");
	if (vertexShaderSrcNode)
	{
		const c8* vertexShaderSource = vertexShaderSrcNode.child_value();
		m_vertexShaderID = CompileShader(GL_VERTEX_SHADER, vertexShaderSource);		
	}

	// Load fragment shader data
	pugi::xml_node fragmentShaderSrcNode = shaderProgramNode.child("fragmentShader");
	if (fragmentShaderSrcNode)
	{
		const c8* fragmentShaderSource = fragmentShaderSrcNode.child_value();
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

	// Collect attributes indices from shader program
	std::vector<GLES20VertexAttribute>& attributes = m_vertexDeclaration.GetAttributes();
	for (u32 i = 0; i < m_vertexDeclaration.attributes.size(); ++i)
	{
		m_vertexDeclaration.attributes[i].index = glGetAttribLocation(m_programID, m_vertexDeclaration.attributes[i].name.c_str());
	}
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

void GLES20ShaderProgram::LoadUniforms(const pugi::xml_node &node)
{
	if (node)
	{
		pugi::xml_node uniformNode = node.first_child();
		while (!uniformNode.empty())
		{
			std::string name = uniformNode.name();
			printf("uniform : %s\n", name.c_str());

			uniformNode = uniformNode.next_sibling();
		}
	}
}

///////////////////////////////////////////////////////////////////////

void GLES20ShaderProgram::LoadAttributes(const pugi::xml_node &node)
{
	if (node)
	{
		pugi::xml_node attributeNode = node.first_child();
		while (!attributeNode.empty())
		{
			GLES20VertexAttribute attribute;

			std::string name = attributeNode.name();
			if (name == "position")
			{
				attribute.semantic = AttributeSemantic::POSITION;
				printf("Postion attribute\n");
			}
			else if (name == "normal")
			{
				attribute.semantic = AttributeSemantic::NORMAL;
				printf("Normal attribute\n");
			}
			else if (name == "color")
			{
				attribute.semantic = AttributeSemantic::COLOR;
				printf("Color attribute\n");
			}
			else if (name == "uv")
			{
				attribute.semantic = AttributeSemantic::UV;
				printf("UV attribute\n");
			}
			else
			{
				printf("Unknown attribute\n");
			}

			attribute.name = attributeNode.attribute("name").as_string();

			pugi::xml_attribute typeAttr = attributeNode.attribute("type");
			name = typeAttr.as_string();

			if (name == "vec3")
			{
				attribute.type = GL_FLOAT;
				attribute.size = 3;
			}
			else if (name == "vec2")
			{
				attribute.type = GL_FLOAT;
				attribute.size = 2;
			}

			m_vertexDeclaration.AddAttribute(attribute);

			attributeNode = attributeNode.next_sibling();
		}
	}
	printf("Done\n");
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