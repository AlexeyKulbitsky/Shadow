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
			struct GLES20UniformLoader<float>
			{
				static void Load(const int location, const float& value)
				{
					glUniform1f(location, value);
				}
			};

			template <>
			struct GLES20UniformLoader<int>
			{
				static void Load(const int location, const int& value)
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
			GLES20Uniform(const T& value) : Uniform(value) {}
			virtual void Load() override;

			// GLES 2 - specific interface
			void SetGLId(int id);

		private:
			T m_data;
			int m_glID;
		};

		/////////////////////////////////////////////////////////////////////////////////

		template <typename T>
		inline void GLES20Uniform<T>::Load()
		{
			GLES20Private::GLES20UniformLoader<T>::Load(m_glID, m_data);
		}

		template <typename T>
		inline void GLES20Uniform<T>::SetGLId(int id)
		{
			m_glId = id;
		}

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////

		typedef GLES20Uniform<float> GLES20UniformFloat;
		typedef GLES20Uniform<int> GLES20UniformInt;
	}
}



#endif
