#ifndef SHADOW_UNIFORM_BUFFER_INCLUDE
#define SHADOW_UNIFORM_BUFFER_INCLUDE

#include "../Globals.h"
#include "Uniform.h"
#include "Sampler.h"

namespace sh
{
	namespace video
	{
		// TODO : implement ShaderData
		//class Uniform;
		//class Sampler;
		class ShaderProgram;

		class UniformBuffer
		{
		public:
			UniformBuffer() {}
			virtual ~UniformBuffer() {}

			virtual void Load(const pugi::xml_node &node){}
			virtual void SetShaderProgram(ShaderProgram* shaderProgram){}
			virtual void Init();
			virtual void Upload();
			virtual UniformBuffer* Clone();
			virtual void AddUniform(Uniform* uniform);
			virtual void AddSampler(Sampler* sampler);

			virtual size_t GetUniformsCount() const { return m_uniforms.size(); }
			virtual Uniform* GetUniform(size_t index) { return m_uniforms[index]; }
			Uniform* GetUniform(const std::string& name);
			Uniform* GetUniform(Uniform::Usage usage);

			virtual size_t GetSamplersCount() const { return m_samplers.size(); }
			virtual Sampler* GetSampler(size_t index) { return m_samplers[index]; }
			Sampler* GetSampler(const std::string& name);
			Sampler* GetSampler(Sampler::Usage usage);

		private:
			std::vector<Uniform*> m_uniforms;
			std::vector<Uniform*> m_globalUniforms;
			std::vector<Sampler*> m_samplers;
		};
	}
}

#endif
