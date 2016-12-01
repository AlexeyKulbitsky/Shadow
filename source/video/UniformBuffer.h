#ifndef SHADOW_UNIFORM_BUFFER_INCLUDE
#define SHADOW_UNIFORM_BUFFER_INCLUDE

#include <vector>

namespace sh
{
	namespace video
	{
		class Uniform;
		class Sampler;

		class UniformBuffer
		{
		public:
			UniformBuffer() {}
			virtual ~UniformBuffer() {}

			virtual void Init();
			virtual void Upload();
			virtual UniformBuffer* Clone();
			virtual void AddUniform(Uniform* uniform);
			virtual void AddSampler(Sampler* sampler);

			virtual size_t GetUniformsCount() const { return m_uniforms.size(); }
			virtual Uniform* GetUniform(size_t index) { return m_uniforms[index]; }
			Uniform* GetUniform(const std::string& name);

			virtual size_t GetSamplersCount() const { return m_samplers.size(); }
			virtual Sampler* GetSampler(size_t index) { return m_samplers[index]; }
			Sampler* GetSampler(const std::string& name);

		private:
			std::vector<Uniform*> m_uniforms;
			std::vector<Sampler*> m_samplers;
		};
	}
}

#endif
