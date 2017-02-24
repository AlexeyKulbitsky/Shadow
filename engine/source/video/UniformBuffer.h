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
		class UniformBuffer
		{
		public:
			UniformBuffer() {}
			virtual ~UniformBuffer() {}

			virtual void Load(const pugi::xml_node &node){}
			virtual void Unload();
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

			virtual size_t GetGlobalUniformsCount() const { return m_globalUniforms.size(); }
			virtual Uniform* GetGlobalUniform(size_t index) { return m_globalUniforms[index]; }

			virtual size_t GetAutoUniformsCount() const { return m_autoUniforms.size(); }
			virtual Uniform* GetAutoUniform(size_t index) { return m_autoUniforms[index]; }

			virtual size_t GetSamplersCount() const { return m_samplers.size(); }
			virtual Sampler* GetSampler(size_t index) { return m_samplers[index]; }
			Sampler* GetSampler(const std::string& name);
			Sampler* GetSampler(Sampler::Usage usage);

			const std::vector<Uniform*>& GetUniforms() const { return m_uniforms; }
			const std::vector<Uniform*>& GetGlobalUniforms() const { return m_globalUniforms; }
			const std::vector<Uniform*>& GetAutoUniforms() const { return m_autoUniforms; }
			const std::vector<Sampler*>& GetSamplers() const { return m_samplers; }

		private:
			std::vector<Uniform*> m_uniforms;
			std::vector<Uniform*> m_globalUniforms;
			std::vector<Uniform*> m_autoUniforms;
			std::vector<Sampler*> m_samplers;
		};
	}
}

#endif
