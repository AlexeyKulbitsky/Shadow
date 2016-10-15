#ifndef SHADOW_DRIVER_INCLUDE
#define SHADOW_DRIVER_INCLUDE

namespace sh
{
	namespace video
	{
		struct HardwareBuffer;

		enum class DriverType
		{
			OPENGL_ES_2_0,
			OPENGL_ES_3_0
		};

		class Driver
		{
		public:
			Driver(){}
			~Driver(){}

			virtual bool Init() = 0;
			virtual void BeginRendering() = 0;
			virtual void EndRendering() = 0;
			virtual void DrawHardwareBuffer(const HardwareBuffer *buffer) = 0;
		};
	}
}

#endif
