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
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file(filename);
	pugi::xml_node shaderProgramNode = doc.first_child();

	// Load attributes
	pugi::xml_node attributesNode = shaderProgramNode.child("attributes");
	LoadAttributes(attributesNode);

	// Load vertex shader data
	pugi::xml_node vertexShaderSrcNode = shaderProgramNode.child("vertexShader");
	if (vertexShaderSrcNode)
	{
		const char* vertexShaderSource = vertexShaderSrcNode.child_value();
		m_vertexShaderID = CompileShader(GL_VERTEX_SHADER, vertexShaderSource);		
	}

	// Load fragment shader data
	pugi::xml_node fragmentShaderSrcNode = shaderProgramNode.child("fragmentShader");
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

	// Collect attributes indices from shader program
	std::vector<GLES20VertexAttribute>& attributes = m_vertexDeclaration.GetAttributes();
	for (u32 i = 0; i < attributes.size(); ++i)
	{
		attributes[i].index = glGetAttribLocation(m_programID, attributes[i].name.c_str());
	}

	// Load uniforms
	pugi::xml_node uniformsNode = shaderProgramNode.child("uniforms");
	LoadUniforms(uniformsNode);
	m_uniformBuffer->Init();

	// Load texure samplers
	pugi::xml_node samplersNode = shaderProgramNode.child("samplers");
	LoadSamplers(samplersNode);

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

	// Load attributes
	pugi::xml_node attributesNode = node.child("attributes");
	LoadAttributes(attributesNode);

	// Collect attributes indices from shader program
	std::vector<GLES20VertexAttribute>& attributes = m_vertexDeclaration.GetAttributes();
	for (u32 i = 0; i < attributes.size(); ++i)
	{
		attributes[i].index = glGetAttribLocation(m_programID, attributes[i].name.c_str());
	}

	// Load uniforms
	pugi::xml_node uniformsNode = node.child("uniforms");
	LoadUniforms(uniformsNode);
	m_uniformBuffer->Init();

	// Load texure samplers
	pugi::xml_node samplersNode = node.child("samplers");
	LoadSamplers(samplersNode);
}

///////////////////////////////////////////////////////////////////////

ShaderProgram* GLES20ShaderProgram::Clone()
{
	GLES20ShaderProgram* result = new GLES20ShaderProgram();
	result->m_fragmentShaderID = m_fragmentShaderID;
	result->m_vertexShaderID = m_vertexShaderID;
	result->m_programID = m_programID;
	result->m_vertexDeclaration = m_vertexDeclaration;
	result->m_uniformBuffer = m_uniformBuffer->Clone();
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

void GLES20ShaderProgram::LoadUniforms(const pugi::xml_node &node)
{
	m_uniformBuffer = new UniformBuffer();

	Uniform* uniform = nullptr;

	if (node)
	{
		pugi::xml_node uniformNode = node.first_child();
		while (!uniformNode.empty())
		{
			pugi::xml_attribute nameAttr = uniformNode.attribute("name");
			pugi::xml_attribute typeAttr = uniformNode.attribute("type");
			pugi::xml_attribute valAttr = uniformNode.attribute("val");

			printf("Uniform:\n");
			std::string name = nameAttr.as_string();
			printf("\tName: %s ", name.c_str());

			std::string typeName = typeAttr.as_string();
			printf("Type: %s ", typeName.c_str());
			if (typeName == "float")
			{
				GLES20UniformFloat* glesUniform = new GLES20UniformFloat(0.0f);
				glesUniform->SetGLId(m_programID);
				uniform = glesUniform;
				if (valAttr)
				{
					float value = valAttr.as_float();
					printf("Value: %f", value);
					uniform->Set(value);
				}

			}
			else if (typeName == "int")
			{
				GLES20UniformInt *glesUniform = new GLES20UniformInt(0);
				glesUniform->SetGLId(m_programID);
				uniform = glesUniform;
				if (valAttr)
				{
					int value = valAttr.as_int();
					printf("Value: %d", value);
					uniform->Set(value);
				}
			}
			else if (typeName == "vec2")
			{
				GLES20UniformVector2f *glesUniform = new GLES20UniformVector2f(sh::math::Vector2f(0.0f));
				glesUniform->SetGLId(m_programID);
				uniform = glesUniform;
				if (valAttr)
				{

				}
			}
			else if (typeName == "vec3")
			{
				GLES20UniformVector3f *glesUniform = new GLES20UniformVector3f(sh::math::Vector3f(0.0f));
				glesUniform->SetGLId(m_programID);
				uniform = glesUniform;
				if (valAttr)
				{

				}
			}
			else if (typeName == "vec4")
			{
				GLES20UniformVector4f *glesUniform = new GLES20UniformVector4f(sh::math::Vector4f(0.0f));
				glesUniform->SetGLId(m_programID);
				uniform = glesUniform;
				if (valAttr)
				{

				}
			}
			else if (typeName == "mat4")
			{
				GLES20UniformMatrix4f *glesUniform = new GLES20UniformMatrix4f(sh::math::Matrix4f());
				glesUniform->SetGLId(m_programID);
				uniform = glesUniform;
				if (valAttr)
				{

				}
			}


			printf("\n");

			uniform->SetName(name);

			m_uniformBuffer->AddUniform(uniform);
			uniform = nullptr;

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

void GLES20ShaderProgram::LoadSamplers(const pugi::xml_node &node)
{
	if (node)
	{
		pugi::xml_node uniformNode = node.first_child();

		GLES20Sampler* sampler = new GLES20Sampler(m_programID);

		while (!uniformNode.empty())
		{
			pugi::xml_attribute nameAttr = uniformNode.attribute("name");
			pugi::xml_attribute typeAttr = uniformNode.attribute("type");
			pugi::xml_attribute tilingUAttr = uniformNode.attribute("tilingU");
			pugi::xml_attribute tilingVAttr = uniformNode.attribute("tilingV");
			pugi::xml_attribute filerAttr = uniformNode.attribute("filter");

			printf("Sampler:\n");
			std::string name = nameAttr.as_string();
			printf("\tName: %s ", name.c_str());

			// Read texture type
			std::string typeName = typeAttr.as_string();
			printf("Type: %s ", typeName.c_str());
			if (typeName == "2D")
			{				
				sampler->SetType(Texture::Type::TEXTURE_2D);
			}
			else if (typeName == "cube")
			{
				sampler->SetType(Texture::Type::TEXTURE_CUBE);
			}

			
			// Read tiling
			Texture::Tiling tilingU = Texture::Tiling::REPEAT;
			Texture::Tiling tilingV = Texture::Tiling::REPEAT;

			if (tilingUAttr)
			{
				std::string tilingTypeName = tilingUAttr.as_string();
				if (tilingTypeName == "clamp")
				{
					tilingU = Texture::Tiling::CLAMP_TO_EDGE;
				}
			}

			if (tilingVAttr)
			{
				std::string tilingTypeName = tilingVAttr.as_string();
				if (tilingTypeName == "clamp")
				{
					tilingV = Texture::Tiling::CLAMP_TO_EDGE;
				}
			}


			// Read filter
			Texture::Filtering filtering = Texture::Filtering::NEAREST;
			if (filerAttr)
			{
				std::string filterTypeName = filerAttr.as_string();
				if (filterTypeName == "nearest")
				{
					filtering = Texture::Filtering::NEAREST;
				}
				else if (filterTypeName == "linear")
				{
					filtering = Texture::Filtering::LINEAR;
				}
				else if (filterTypeName == "bilinear")
				{
					filtering = Texture::Filtering::BILINEAR;
				}
				else if (filterTypeName == "trilinear")
				{
					filtering = Texture::Filtering::TRILINEAR;
				}
			}

			printf("\n");			

			sampler->SetName(name);
			sampler->SetTiling(tilingU, tilingV);
			sampler->SetFiltering(filtering);
			sampler->Init();



			m_uniformBuffer->AddSampler(sampler);

			uniformNode = uniformNode.next_sibling();
		}
	}
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