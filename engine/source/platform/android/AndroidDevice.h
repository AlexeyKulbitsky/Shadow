#ifndef SHADOW_ANDROID_DEVICE_INCLUDE
#define SHADOW_ANDROID_DEVICE_INCLUDE

#include "device/Device.h"
#include "platform/android/AndroidEventsQueue.h"

struct AndroidCreationParams : public sh::CreationParameters
{
	//sh::String apkPath = "";
};

namespace sh
{
	class AndroidDevice : public Device
	{
	public:
		AndroidDevice();
		explicit AndroidDevice(const CreationParameters &parameters);
		virtual ~AndroidDevice();

		void Init() override final;
		void Update(float deltaTime) override final;
		void Run() override final;
		uint64_t GetTime() override final;

		void OnEvent(const AndroidEvent& event);

	public:
		static AndroidCreationParams params;

	private:
		void ProcessEvent(const AndroidEvent& ev);

	private:
		AndroidEventsQueue m_eventsQueue;
	};
}

#endif
