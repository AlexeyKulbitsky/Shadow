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
		inline void GLES20Uniform<T>::SetGLId(s32 id)
		{
			m_shaderProgramId = id;
		}

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////

		typedef GLES20Uniform<f32> GLES20UniformFloat;
		typedef GLES20Uniform<s32> GLES20UniformInt;
	}
}



#endif
