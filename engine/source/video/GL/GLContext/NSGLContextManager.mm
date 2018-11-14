//#import <Foundation/Foundation.h>

#include "NSGLContextManager.h"


#import <OpenGL/OpenGL.h>

namespace sh
{

namespace video
{
    NSOpenGLContextManager::NSOpenGLContextManager()
    {
        
    }
    
    NSOpenGLContextManager::NSOpenGLContextManager(int majorVersion, int minorVersion, NSView* view)
    {
        NSOpenGLPixelFormatAttribute profile = /*NSOpenGLProfileVersion3_2Core*/NSOpenGLProfileVersionLegacy;

        // Set pixel format
        //NSOpenGLPixelFormatAttribute attribs[40];
        //NSOpenGLPixelFormatAttribute attribs[] = { NSOpenGLPFAAccelerated, 0 };
        NSOpenGLPixelFormatAttribute attribs[] =
        {
            NSOpenGLPFAAccelerated, true,
            NSOpenGLPFABackingStore, true,
            NSOpenGLPFADoubleBuffer, true,
            //NSOpenGLPFAClosestPolicy, true,
            NSOpenGLPFAColorSize, 24,
            NSOpenGLPFAAlphaSize, 8,
            NSOpenGLPFADepthSize, 24,
            NSOpenGLPFAOpenGLProfile, profile,
            //NSOpenGLProfileVersion3_2Core,
            0, 0
            
        };

        m_pixelFormat = [[NSOpenGLPixelFormat alloc] initWithAttributes:attribs];

        // Create context
        //m_context = [[NSOpenGLContext alloc] initWithFormat:m_pixelFormat shareContext:share];
        //m_context = [[NSOpenGLContext alloc] initWithFormat:m_pixelFormat];
         //m_context = [NSOpenGLContext alloc];
        m_context = [[[NSOpenGLContext alloc] initWithFormat:m_pixelFormat shareContext:nil] autorelease];
        
        // Link context to the view
        [m_context setView:view];
        [m_context makeCurrentContext];
        auto res = glewInit();
        SH_ASSERT(res == GLEW_OK, "Can not initialize GLEW!");
        
        int a = 0;
        a++;
    }
    
    bool NSOpenGLContextManager::CreateContext(void* winId)
    {
        return false;
    }
    
    bool NSOpenGLContextManager::DestroyContext()
    {
        return false;
    }
    
    void NSOpenGLContextManager::SwapBuffers()
    {
        [m_context flushBuffer];
    }
    
    bool NSOpenGLContextManager::IsContextCreated()
    {
        return false;
    }
    
    void NSOpenGLContextManager::Update()
    {
        [m_context makeCurrentContext];
        [m_context update];
    }
    
} // video
    
} // sh


