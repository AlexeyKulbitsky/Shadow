#ifndef SHADOW_GLES20_DRIVER_INCLUDE
#define SHADOW_GLES20_DRIVER_INCLUDE

#include "../Driver.h"

namespace sh
{
	namespace video
	{
		class EGLContextManager;

		class GLES20Driver : public Driver 
		{
		public:
			GLES20Driver(){}
			GLES20Driver(EGLContextManager* contextManager);
			virtual ~GLES20Driver(){}

			virtual bool Init() override;
			virtual void BeginRendering() override;
			virtual void EndRendering() override;
			virtual void DrawHardwareBuffer(const HardwareBuffer *buffer) override;
			virtual HardwareBuffer* CreateHardwareBuffer(const scene::Mesh* mesh) override;

		private:
			EGLContextManager* m_contextManager = nullptr;
		};
	}
}
#endif
