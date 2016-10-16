#ifndef SHADOW_GLES20_SHADER_PROGRAM_INCLUDE
#define SHADOW_GLES20_SHADER_PROGRAM_INCLUDE

#include "../ShaderProgram.h"
#include "../GLContext/EGLContextManager.h"
#include <pugixml.hpp>

namespace sh
{
	namespace video
	{
		class GLES20ShaderProgram : public ShaderProgram
		{
		public:
			GLES20ShaderProgram();
			virtual void Load(const c8* filename) override;
			virtual void Unload() override;
			virtual void BindProgram() override;
			virtual void UnbindProgram() override;

		private:
			void LoadUniforms(const pugi::xml_node &node);
			void LoadAttributes(const pugi::xml_node &node);

			GLuint CompileShader(GLenum shaderType, const c8* source);
			

		private:
			GLuint m_programID;
			GLuint m_vertexShaderID;
			GLuint m_fragmentShaderID;
		};
	}
}

#endif