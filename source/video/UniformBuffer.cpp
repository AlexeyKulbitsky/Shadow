#include "UniformBuffer.h"
#include "Uniform.h"

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
		}

		void UniformBuffer::AddUniform(Uniform* uniform)
		{
			m_uniforms.push_back(uniform);
		}
	}
}