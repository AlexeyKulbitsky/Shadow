#ifndef SHADOW_GLES20_INDEX_BUFFER_INCLUDE
#define SHADOW_GLES20_INDEX_BUFFER_INCLUDE


#include "../IndexBuffer.h"

namespace sh
{
	namespace video
	{
		class GLES20IndexBuffer : public IndexBuffer
		{
			friend class GLES20Driver;
		public:
			GLES20IndexBuffer();
			virtual ~GLES20IndexBuffer();

			unsigned int GetGLId() const { return m_glID; }
		private:
			unsigned int m_glID = 0U;
		};
	}
}
#endif
