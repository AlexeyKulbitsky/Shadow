#ifndef SHADOW_IOS_DEVICE_INCLUDE
#define SHADOW_IOS_DEVICE_INCLUDE

#include "Device.h"

namespace sh
{
    class iOSDevice : public Device
    {
    public:
        iOSDevice();
        explicit iOSDevice(const CreationParameters &parameters);
        virtual ~iOSDevice();
        
        void Init() override final;
        void Update(float deltaTime) override final;
        void Run() override final;
        uint64_t GetTime() override final;
        
    };
} // sh

#endif
