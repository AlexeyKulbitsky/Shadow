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
			virtual void Load(const pugi::xml_node &node) override;
			virtual ShaderProgram* Clone() override;
			virtual void Unload() override;
			virtual void BindProgram() override;
			virtual void UnbindProgram() override;

			u32 GetGLId() const { return m_programID; }

		private:
			u32 CompileShader(u32 shaderType, const c8* source);
			
		private:
			u32 m_programID;
			u32 m_vertexShaderID;
			u32 m_fragmentShaderID;	
		};
	}
}

#endif