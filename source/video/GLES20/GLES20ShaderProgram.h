#ifndef SHADOW_GLES20_SHADER_PROGRAM_INCLUDE
#define SHADOW_GLES20_SHADER_PROGRAM_INCLUDE

#include "../ShaderProgram.h"
#include "../GLContext/EGLContextManager.h"
#include "GLES20VertexDeclaration.h"
#include "GLES20Uniform.h"
#include <pugixml.hpp>

namespace sh
{
	namespace video
	{
		class UniformBuffer;

		class GLES20ShaderProgram : public ShaderProgram
		{
		public:
			GLES20ShaderProgram();
			virtual void Load(const char* filename) override;
			virtual void Unload() override;
			virtual void BindProgram() override;
			virtual void UnbindProgram() override;

			GLES20VertexDeclaration* GetVertexDeclaration() { return &m_vertexDeclaration; }
			UniformBuffer* GetUniformBuffer() { return m_uniformBuffer; }

		private:
			void LoadUniforms(const pugi::xml_node &node);
			void LoadAttributes(const pugi::xml_node &node);
			void LoadSamplers(const pugi::xml_node &node);
			GLuint CompileShader(GLenum shaderType, const c8* source);
			
		private:
			GLuint m_programID;
			GLuint m_vertexShaderID;
			GLuint m_fragmentShaderID;

			GLES20VertexDeclaration m_vertexDeclaration;		
			UniformBuffer* m_uniformBuffer;
		};
	}
}

#endif