#ifndef SHADOW_SHADER_PROGRAM_INCLUDE
#define SHADOW_SHADER_PROGRAM_INCLUDE

#include "../Types.h"
#include <pugixml.hpp>

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

		class UniformBuffer;

		class ShaderProgram
		{
		public:
			virtual void Load(const char* filename) = 0;
			virtual void Load(const pugi::xml_node &node) = 0;
			virtual ShaderProgram* Clone() = 0;
			virtual void Unload() = 0;
			virtual void BindProgram() = 0;
			virtual void UnbindProgram() = 0;

			UniformBuffer* GetUniformBuffer() { return m_uniformBuffer; }

		protected:
			UniformBuffer* m_uniformBuffer;
		};
	}
}
#endif