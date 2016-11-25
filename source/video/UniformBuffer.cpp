#include "UniformBuffer.h"
#include "Uniform.h"
#include "Sampler.h"

namespace sh
{
	namespace video
	{
		void UniformBuffer::Init()
		{
			for (size_t i = 0; i < m_uniforms.size(); ++i)
			{
				m_uniforms[i]->Init();
			}
		}

		void UniformBuffer::Upload()
		{
			for (size_t i = 0; i < m_uniforms.size(); ++i)
			{
				m_uniforms[i]->Load();
			}

			int textureChannel = 0;
			for (size_t i = 0; i < m_samplers.size(); ++i)
			{
				m_samplers[i]->Load(textureChannel++);
			}
		}

		void UniformBuffer::AddUniform(Uniform* uniform)
		{
			m_uniforms.push_back(uniform);
		}

		void UniformBuffer::AddSampler(Sampler* sampler)
		{
			m_samplers.push_back(sampler);
		}
	}
}