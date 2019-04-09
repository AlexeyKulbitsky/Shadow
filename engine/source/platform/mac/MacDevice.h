#ifndef SHADOW_MAC_DEVICE_INCLUDE
#define SHADOW_MAC_DEVICE_INCLUDE

#include "Device.h"

namespace sh
{
    class MacDevice : public Device
    {
    public:
        MacDevice();
        explicit MacDevice(const CreationParameters &parameters);
        virtual ~MacDevice();
        
        void Init() override final;
        void Update(float deltaTime) override final;
        void Run() override final;
        uint64_t GetTime() override final;
        
    };
} // sh

#endif
