#ifndef SHADOW_ANDROID_DEVICE_INCLUDE
#define SHADOW_ANDROID_DEVICE_INCLUDE

#include "../../Device.h"
#include "AndroidEventsQueue.h"

//#include <jni.h>
//#include <android/asset_manager.h>

struct AAssetManager;
struct AndroidCreationParams : public sh::CreationParameters
{
    AAssetManager* assetManager = nullptr;
    sh::String assetsDataPath = "";
};

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
		virtual void Run() override;
		virtual u64 GetTime() override;

		bool CreateDriver();

		void OnEvent(const AndroidEvent& event);

	public:
		static AndroidCreationParams params;

	private:
		void ProcessEvent(const AndroidEvent& ev);

	private:
		AndroidEventsQueue m_eventsQueue;
	};

	void AndroidMainThreadFunction();
}

#endif
