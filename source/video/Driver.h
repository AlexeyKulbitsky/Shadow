#ifndef SHADOW_DRIVER_INCLUDE
#define SHADOW_DRIVER_INCLUDE

namespace sh
{
	namespace video
	{

		enum class DriverType
		{
			OPENGL_ES_2_0
		};

		class Driver
		{
		public:
			Driver(){}
			~Driver(){}

			virtual bool Init() = 0;
			virtual void BeginRendering() = 0;
			virtual void EndRendering() = 0;
		};
	}
}

#endif
