#ifndef SHADOW_ANDROID_DEVICE_INCLUDE
#define SHADOW_ANDROID_DEVICE_INCLUDE

#include "../../Device.h"

namespace sh
{
	class AndroidDevice : public Device 
	{
	public:
		AndroidDevice();
		explicit AndroidDevice(const CreationParameters &parameters);
		virtual ~AndroidDevice();

		virtual void Init() override;
		virtual void Update(f32 deltaTime) override;
		virtual bool Run() override;
//		virtual void OnEvent(const Event& e) override;
		virtual u64 GetTime() override;

		bool CreateDriver();
	private:
	};
}

#endif
