#ifndef SHADOW_NSGLCONTEXTMANAGER_INCLUDE
#define SHADOW_NSGLCONTEXTMANAGER_INCLUDE

#include "GLContextManager.h"

#import <Cocoa/Cocoa.h>

namespace sh
{
    
namespace video
{
    
    class NSOpenGLContextManager : public GLContextManager
    {
    public:
        NSOpenGLContextManager();
        NSOpenGLContextManager(int majorVersion, int minorVersion, NSView* view);
        
        virtual bool CreateContext(void* winId) override;
        virtual bool DestroyContext() override;
        virtual void SwapBuffers() override;
        virtual bool IsContextCreated() override;
        
        virtual void Update() override;
        
    private:
        id m_pixelFormat;
        id m_context;
    };
    
} // video
    
} // sh

#endif /* SHADOW_NSGLCONTEXTMANAGER_INCLUDE */
