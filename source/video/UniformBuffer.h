#ifndef SHADOW_UNIFORM_BUFFER_INCLUDE
#define SHADOW_UNIFORM_BUFFER_INCLUDE

#include <vector>

namespace sh
{
	namespace video
	{
		class Uniform;

		class UniformBuffer
		{
		public:
			UniformBuffer() {}
			virtual ~UniformBuffer() {}

			virtual void Init();
			virtual void Upload();
			virtual void AddUniform(Uniform* uniform);
			virtual size_t GetUniformsCount() const { return m_uniforms.size(); }
			virtual Uniform* GetUniform(size_t index) { return m_uniforms[index]; }

		private:
			std::vector<Uniform*> m_uniforms;
		};
	}
}

#endif
