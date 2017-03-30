#ifndef SHADOW_SHADER_PROGRAM_INCLUDE
#define SHADOW_SHADER_PROGRAM_INCLUDE

#include "../Globals.h"
#include "Shader.h"

namespace sh
{
	namespace video
	{
		
		
		class ShaderProgram
		{
		public:
			virtual void Load(const char* filename) = 0;
			virtual void Load(const pugi::xml_node &node) = 0;
			virtual ShaderProgram* Clone() = 0;
			virtual void Unload() = 0;
			virtual void BindProgram() = 0;
			virtual void UnbindProgram() = 0;
		};
	}
}
#endif