#ifndef SHADOW_GLES20_UNIFORM_INCLUDE
#define SHADOW_GLES20_UNIFORM_INCLUDE

#include "../Uniform.h"
#include "../GLContext/EGLContextManager.h"

namespace sh
{
	namespace video
	{

		namespace GLES20Private
		{
			template <typename T>
			struct GLES20UniformLoader;

			template <>
			struct GLES20UniformLoader<f32>
			{
				static void Load(const s32 location, const f32& value)
				{
					glUniform1f(location, value);
				}
			};

			template <>
			struct GLES20UniformLoader<s32>
			{
				static void Load(const s32 location, const s32& value)
				{
					glUniform1i(location, value);
				}
			};

			template <>
			struct GLES20UniformLoader<math::Vector2f>
			{
				static void Load(const s32 location, const math::Vector2f& value)
				{
					glUniform2f(location, value.x, value.y);
				}
			};

			template <>
			struct GLES20UniformLoader<math::Vector3f>
			{
				static void Load(const s32 location, const math::Vector3f& value)
				{
					glUniform3f(location, value.x, value.y, value.z);
				}
			};

			template <>
			struct GLES20UniformLoader<math::Vector4f>
			{
				static void Load(const s32 location, const math::Vector4f& value)
				{
					glUniform4f(location, value.x, value.y, value.z, value.w);
				}
			};

			template <>
			struct GLES20UniformLoader<math::Matrix4f>
			{
				static void Load(const s32 location, const math::Matrix4f& value)
				{
					glUniformMatrix4fv(location, 1, GL_FALSE, &value._m[0]);
				}
			};
		}

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////

		template <typename T>
		class GLES20Uniform : public Uniform
		{
		public:			
			GLES20Uniform(const T& value) : Uniform(value), m_shaderProgramId(0) {}
			GLES20Uniform(const std::string& name, unsigned int shaderProgramId) : Uniform(name), m_shaderProgramId(shaderProgramId) {}
			virtual void Load() override;
			virtual void Init() override;
			virtual Uniform* Clone() override;

			// GLES 2 - specific interface
			void SetGLId(s32 id);

		private:
			s32 m_glID;
			u32 m_shaderProgramId;
		};

		/////////////////////////////////////////////////////////////////////////////////

		template <typename T>
		inline void GLES20Uniform<T>::Load()
		{
			GLES20Private::GLES20UniformLoader<T>::Load(m_glID, Get<T>());
		}

		template <typename T>
		inline void GLES20Uniform<T>::Init()
		{
			m_glID = glGetUniformLocation(m_shaderProgramId, m_name.c_str());
		}

		template <typename T>
		inline Uniform* GLES20Uniform<T>::Clone()
		{
			const T& value = Get<T>();
			GLES20Uniform<T>* result = new GLES20Uniform<T>(value);
			result->m_glID = m_glID;
			result->m_name = m_name;
			result->m_shaderProgramId = m_shaderProgramId;
			result->m_type = m_type;
			result->m_usage = m_usage;
			result->m_globalName = m_globalName;

			return result;
		}

		template <typename T>
		inline void GLES20Uniform<T>::SetGLId(s32 id)
		{
			m_shaderProgramId = id;
		}

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////

		typedef GLES20Uniform<f32>				GLES20UniformFloat;
		typedef GLES20Uniform<s32>				GLES20UniformInt;
		typedef GLES20Uniform<math::Vector2f>	GLES20UniformVector2f;
		typedef GLES20Uniform<math::Vector3f>	GLES20UniformVector3f;
		typedef GLES20Uniform<math::Vector4f>	GLES20UniformVector4f;
		typedef GLES20Uniform<math::Matrix4f>	GLES20UniformMatrix4f;
	}
}



#endif
