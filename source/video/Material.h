#ifndef SHADOW_MATERIAL_INCLUDE
#define SHADWO_MATERIAL_INCLUDE

#include "Color.h"

namespace sh
{
	namespace video
	{
		class ShaderProgram;

		class Material
		{
		public:
			Material() {}
			void SetShaderProgram(ShaderProgram* program) { m_shaderProgram = program; }

			ShaderProgram* GetShaderProgram() { return m_shaderProgram; }
		private:
			ShaderProgram* m_shaderProgram = nullptr;
		};
	}
}

#endif