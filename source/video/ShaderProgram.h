#ifndef SHADOW_SHADER_PROGRAM_INCLUDE
#define SHADOW_SHADER_PROGRAM_INCLUDE

#include "../Types.h"

namespace sh
{
	namespace video
	{
		enum class ShaderType
		{
			VERTEX = 0,
			FRAGMENT,
			NONE
		};

		class ShaderProgram
		{
		public:
			virtual void Load(const c8* filename) = 0;
			virtual void Unload() = 0;
			virtual void BindProgram() = 0;
			virtual void UnbindProgram() = 0;
		};
	}
}
#endif