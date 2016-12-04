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

		UniformBuffer* UniformBuffer::Clone()
		{
			UniformBuffer* result = new UniformBuffer();
			result->m_uniforms.resize(m_uniforms.size());
			for (size_t i = 0, sz = m_uniforms.size(); i < sz; ++i)
			{
				result->m_uniforms[i] = m_uniforms[i]->Clone();
			}

			result->m_samplers.resize(m_samplers.size());
			for (size_t i = 0, sz = m_samplers.size(); i < sz; ++i)
			{
				result->m_samplers[i] = m_samplers[i]->Clone();
			}

			return result;
		}

		void UniformBuffer::AddUniform(Uniform* uniform)
		{
			m_uniforms.push_back(uniform);
		}

		void UniformBuffer::AddSampler(Sampler* sampler)
		{
			m_samplers.push_back(sampler);
		}

		Uniform* UniformBuffer::GetUniform(const std::string& name)
		{
			for (size_t i = 0, sz = m_uniforms.size(); i < sz; ++i)
			{
				if (m_uniforms[i]->GetName() == name)
					return m_uniforms[i];
			}
			return nullptr;
		}

		Uniform* UniformBuffer::GetUniform(Uniform::Usage usage)
		{
			return nullptr;
		}

		Sampler* UniformBuffer::GetSampler(const std::string& name)
		{
			for (size_t i = 0, sz = m_samplers.size(); i < sz; ++i)
			{
				if (m_samplers[i]->GetName() == name)
					return m_samplers[i];
			}
			return nullptr;
		}

		Sampler* UniformBuffer::GetSampler(Sampler::Usage usage)
		{
			return nullptr;
		}
	}
}