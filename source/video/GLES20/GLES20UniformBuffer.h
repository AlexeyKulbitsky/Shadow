#ifndef SHADOW_GLES20_UNIFORM_BUFFER
#define SHADOW_GLES20_UNIFORM_BUFFER

#include "../UniformBuffer.h"


namespace sh
{
	namespace video
	{
		class GLES20ShaderProgram;

		class GLES20UniformBuffer : public UniformBuffer
		{
		public:
			GLES20UniformBuffer() {}
			virtual ~GLES20UniformBuffer() {}

			virtual void Load(const pugi::xml_node &node) override;
			virtual void Init() override;
			virtual void SetShaderProgram(ShaderProgram* shaderProgram) override;

		private:
			void LoadUniforms(const pugi::xml_node &node);
			void LoadSamplers(const pugi::xml_node &node);

		private:
			GLES20ShaderProgram* m_shaderProgram;
		};
	}
}



#endif
