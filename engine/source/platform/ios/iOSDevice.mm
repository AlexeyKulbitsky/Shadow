#include "platform/ios/iOSDevice.h"
#include "../../Application.h"

namespace sh
{
    iOSDevice::iOSDevice()
    {
        
    }
    
    iOSDevice::iOSDevice(const CreationParameters &parameters)
    {
        
    }
    
    iOSDevice::~iOSDevice()
    {
        
    }
    
    void iOSDevice::Init()
    {
        
    }
    
    void iOSDevice::Update(float deltaTime)
    {
        m_application->Update(0U);
    }
    
    void iOSDevice::Run()
    {
        
    }
    
    uint64_t iOSDevice::GetTime()
    {
        return 0U;
    }
}
