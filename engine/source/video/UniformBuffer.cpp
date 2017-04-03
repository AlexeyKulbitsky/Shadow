#include "UniformBuffer.h"
#include "Driver.h"
#include "../Device.h"

namespace sh
{
	namespace video
	{
		UniformsBatch::~UniformsBatch()
		{
			for (size_t i = 0U; i < m_uniforms.size(); ++i)
			{
				delete m_uniforms[i];
			}
		}

		void UniformsBatch::AddUniform(Uniform* uniform)
		{ 
			m_uniforms.push_back(uniform); 
			m_size += uniform->GetSize();
		}

		UniformsBatchPtr UniformsBatch::Clone() const
		{
			UniformsBatchPtr result(new UniformsBatch());
			
			size_t size = m_uniforms.size();
			result->m_uniforms.resize(size);
			for (size_t i = 0U; i < size; ++i)
			{
				result->m_uniforms[i] = m_uniforms[i]->Clone();
			}
			result->m_size = m_size;
			return result;
		}

		///////////////////////////////////////////////////////////////

		void UniformBuffer::Unload()
		{
			for (size_t i = 0; i < m_uniforms.size(); ++i)
			{
				delete m_uniforms[i];
				m_uniforms[i] = nullptr;
			}
			for (size_t i = 0; i < m_autoUniforms.size(); ++i)
			{
				delete m_autoUniforms[i];
				m_autoUniforms[i] = nullptr;
			}
			for (size_t i = 0; i < m_globalUniforms.size(); ++i)
			{
				delete m_globalUniforms[i];
				m_globalUniforms[i] = nullptr;
			}
			for (size_t i = 0; i < m_samplers.size(); ++i)
			{
				delete m_samplers[i];
				m_samplers[i] = nullptr;
			}		
		}

		//////////////////////////////////////////////////////////////

		void UniformBuffer::Init()
		{
			for (size_t i = 0; i < m_uniforms.size(); ++i)
			{
				m_uniforms[i]->Init();
			}
			m_autoUniformsBatch.reset(new UniformsBatch());
			//m_autoUniformsBatch->m_uniforms.resize(m_autoUniforms.size());			
			m_autoUniformsBatch->m_uniforms.reserve(m_autoUniforms.size());
			for (size_t i = 0; i < m_autoUniforms.size(); ++i)
			{
				m_autoUniforms[i]->Init();
				//m_autoUniformsBatch->m_uniforms[i] = m_autoUniforms[i]->Clone();
				m_autoUniformsBatch->AddUniform(m_autoUniforms[i]->Clone());
			}

			for (size_t i = 0; i < m_globalUniforms.size(); ++i)
			{
				m_globalUniforms[i]->Init();
			}
		}

		//////////////////////////////////////////////////////////////

		void UniformBuffer::Upload()
		{
			Driver* driver = Device::GetInstance()->GetDriver();

			for (size_t i = 0; i < m_uniforms.size(); ++i)
			{
				m_uniforms[i]->Upload();
			}

			for (size_t i = 0; i < m_globalUniforms.size(); ++i)
			{
				GlobalUniformName globalUniformName = m_globalUniforms[i]->GetGlobalUniformName();
				Uniform* globalUniform = driver->GetGlobalUniform(globalUniformName);

				switch (globalUniform->GetType())
				{
					case Uniform::Type::FLOAT:
					{
						f32 value = globalUniform->Get<f32>();
						m_globalUniforms[i]->Set(value);
					}
						break;
					case Uniform::Type::INT:
					{
						int value = globalUniform->Get<int>();
						m_globalUniforms[i]->Set(value);
					}					
						break;
					case Uniform::Type::MAT4:
					{
						const math::Matrix4f& value = globalUniform->Get<math::Matrix4f>();
						m_globalUniforms[i]->Set(value);
					}
						break;
					case Uniform::Type::VEC2:
					{
						const math::Vector2f& value = globalUniform->Get<math::Vector2f>();
						m_globalUniforms[i]->Set(value);
					}			
						break;
					case Uniform::Type::VEC3:
					{
						const math::Vector3f& value = globalUniform->Get<math::Vector3f>();
						m_globalUniforms[i]->Set(value);
					}
						break;
					case Uniform::Type::VEC4:
					{
						const math::Vector4f& value = globalUniform->Get<math::Vector4f>();
						m_globalUniforms[i]->Set(value);
					}
						break;
					case Uniform::Type::VEC3_ARRAY:
					{
						const std::vector<math::Vector3f>& value = globalUniform->Get<std::vector<math::Vector3f>>();
						m_globalUniforms[i]->Set(value);
					}
						break;
					case Uniform::Type::UNKNOWN:
						SH_ASSERT(0, "Unknown uniform type");
						break;
					default:
						SH_ASSERT(0, "Unknown uniform type");
						break;

				}			
				m_globalUniforms[i]->Upload();
			}

			int textureChannel = 0;
			for (size_t i = 0; i < m_samplers.size(); ++i)
			{
				m_samplers[i]->Load(textureChannel++);
			}
		}

		//////////////////////////////////////////////////////////////

		UniformBuffer* UniformBuffer::Clone()
		{
			UniformBuffer* result = new UniformBuffer();
			result->m_uniforms.resize(m_uniforms.size());
			result->m_autoUniforms.resize(m_autoUniforms.size());
			result->m_globalUniforms.resize(m_globalUniforms.size());
			for (size_t i = 0, sz = m_uniforms.size(); i < sz; ++i)
			{
				result->m_uniforms[i] = m_uniforms[i]->Clone();
			}
			for (size_t i = 0, sz = m_autoUniforms.size(); i < sz; ++i)
			{
				result->m_autoUniforms[i] = m_autoUniforms[i]->Clone();
			}
			result->m_autoUniformsBatch = m_autoUniformsBatch->Clone();
			for (size_t i = 0, sz = m_globalUniforms.size(); i < sz; ++i)
			{
				result->m_globalUniforms[i] = m_globalUniforms[i]->Clone();
			}


			return result;
		}

		//////////////////////////////////////////////////////////////

		void UniformBuffer::AddUniform(Uniform* uniform)
		{
			if (uniform->GetGlobalUniformName() == GlobalUniformName::MODEL_WORLD_MATRIX ||
				uniform->GetGlobalUniformName() == GlobalUniformName::MODEL_NORMAL_MATRIX ||
				uniform->GetGlobalUniformName() == GlobalUniformName::MODEL_WORLD_VIEW_PROJECTION_MATRIX)
			{
				m_autoUniforms.push_back(uniform);
			}
			else if (uniform->GetGlobalUniformName() != GlobalUniformName::NONE)
				m_globalUniforms.push_back(uniform);
			else
				m_uniforms.push_back(uniform);
		}

		//////////////////////////////////////////////////////////////

		void UniformBuffer::AddSampler(Sampler* sampler)
		{
			m_samplers.push_back(sampler);
		}

		//////////////////////////////////////////////////////////////

		Uniform* UniformBuffer::GetUniform(const std::string& name)
		{
			for (size_t i = 0, sz = m_uniforms.size(); i < sz; ++i)
			{
				if (m_uniforms[i]->GetName() == name)
					return m_uniforms[i];
			}
			return nullptr;
		}

		//////////////////////////////////////////////////////////////

		Uniform* UniformBuffer::GetUniform(Uniform::Usage usage)
		{
			for (size_t i = 0, sz = m_uniforms.size(); i < sz; ++i)
			{
				if (m_uniforms[i]->GetUsage() == usage)
					return m_uniforms[i];
			}
			return nullptr;
		}

		//////////////////////////////////////////////////////////////
	}
}