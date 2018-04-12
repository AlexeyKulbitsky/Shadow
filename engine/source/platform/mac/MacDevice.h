#ifndef SHADOW_MAC_DEVICE_INCLUDE
#define SHADOW_MAC_DEVICE_INCLUDE

#include "../../Device.h"

namespace sh
{
    class MacDevice : public Device
    {
    public:
        MacDevice();
        explicit MacDevice(const CreationParameters &parameters);
        virtual ~MacDevice();
        
        virtual void Init() override;
        virtual void Update(f32 deltaTime) override;
        virtual void Run() override;
        virtual u64 GetTime() override;
        
        virtual String ShowOpenFileDialog() override;
        virtual String ShowSaveFileDialog() override;
        
        bool CreateDriver();
    };
} // sh

#endif
