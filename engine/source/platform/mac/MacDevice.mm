#include "MacDevice.h"

#include "../../Application.h"
#include "../../io/mac/MacFileSystem.h"
#include "../../video/GL/GLES20/GLES20Driver.h"
#include "../../video/GL/GLContext/NSGLContextManager.h"

#include <GL/glew.h>

#import <Cocoa/Cocoa.h>

NSWindow *rootWindow;

sh::CreationParameters* params = nullptr;

@interface MyView : NSView
{
    NSTrackingArea *trackingArea;
}
@end

@implementation MyView
-(void) mouseUp:(NSEvent *)event
{
    NSPoint pos = [event locationInWindow];
    int x = pos.x;
    int y = params->height - pos.y;
    
    sh::Device* device = sh::Device::GetInstance();
    if (device)
        device->mouseEvent(x, y, sh::MouseEventType::ButtonReleased, sh::MouseCode::ButtonLeft);
}

-(void) mouseDown:(NSEvent *)event
{
    NSPoint pos = [event locationInWindow];
    int x = pos.x;
    int y = params->height - pos.y;
    
    sh::Device* device = sh::Device::GetInstance();
    if (device)
        device->mouseEvent(x, y, sh::MouseEventType::ButtonPressed, sh::MouseCode::ButtonLeft);
}

-(void) rightMouseDown:(NSEvent *)event
{
    NSPoint pos = [event locationInWindow];
    int x = pos.x;
    int y = params->height - pos.y;
    
    sh::Device* device = sh::Device::GetInstance();
    if (device)
        device->mouseEvent(x, y, sh::MouseEventType::ButtonPressed, sh::MouseCode::ButtonRight);
}

-(void) rightMouseUp:(NSEvent *)event
{
    NSPoint pos = [event locationInWindow];
    int x = pos.x;
    int y = params->height - pos.y;
    
    sh::Device* device = sh::Device::GetInstance();
    if (device)
        device->mouseEvent(x, y, sh::MouseEventType::ButtonPressed, sh::MouseCode::ButtonRight);
}

-(void) otherMouseDown:(NSEvent *)event
{
    NSPoint pos = [event locationInWindow];
    int x = pos.x;
    int y = params->height - pos.y;
    
    sh::Device* device = sh::Device::GetInstance();
    if (device)
        device->mouseEvent(x, y, sh::MouseEventType::ButtonPressed, sh::MouseCode::ButtonWheel);
}

-(void) otherMouseUp:(NSEvent *)event
{
    NSPoint pos = [event locationInWindow];
    int x = pos.x;
    int y = params->height - pos.y;
    
    sh::Device* device = sh::Device::GetInstance();
    if (device)
        device->mouseEvent(x, y, sh::MouseEventType::ButtonReleased, sh::MouseCode::ButtonWheel);
}


-(void) mouseMoved:(NSEvent *)event
{
    NSPoint pos = [event locationInWindow];
    int x = pos.x;
    int y = params->height - pos.y;
    
    //NSLog(@"Mouse moved: x=%d, y=%d", x, y);
    
    sh::Device* device = sh::Device::GetInstance();
    if (device)
        device->mouseEvent(x, y, sh::MouseEventType::Moved, sh::MouseCode::ButtonLeft);
}

-(void) mouseDragged:(NSEvent *)event
{
    NSPoint pos = [event locationInWindow];
    int x = pos.x;
    int y = params->height - pos.y;
    
    //NSLog(@"Mouse moved: x=%d, y=%d", x, y);
    
    sh::Device* device = sh::Device::GetInstance();
    if (device)
        device->mouseEvent(x, y, sh::MouseEventType::Moved, sh::MouseCode::ButtonLeft);
}

-(void) scrollWheel:(NSEvent *)event
{
    NSPoint pos = [event locationInWindow];
    int x = pos.x;
    int y = params->height - pos.y;
    
    int d = event.deltaY < 0 ? -1 : 1;
    sh::Device* device = sh::Device::GetInstance();
    if (device)
        device->mouseWheelEvent(x, y, d);
}

-(void) setFrameSize:(NSSize)newSize
{
    [super setFrameSize:newSize];
    
    sh::Device* device = sh::Device::GetInstance();

    int width = newSize.width;
    int height = newSize.height;
    if (device)
        device->windowResizeEvent(width, height);
}


//- (void) mouseEntered:(NSEvent*)theEvent
//{
//    // Mouse entered tracking area.
//}
//
//- (void) mouseExited:(NSEvent*)theEvent
//{
//    // Mouse exited tracking area.
//}

- (void)updateTrackingAreas
{
    if (trackingArea != nil)
    {
        [self removeTrackingArea:trackingArea];
        [trackingArea release];
    }
    
//    const NSTrackingAreaOptions options =
//    NSTrackingMouseEnteredAndExited |
//    NSTrackingActiveInKeyWindow |
//    NSTrackingEnabledDuringMouseDrag |
//    NSTrackingCursorUpdate |
//    NSTrackingInVisibleRect |
//    NSTrackingActiveInActiveApp |
//    NSTrackingAssumeInside;
    
//    const NSTrackingAreaOptions options =
//    NSTrackingMouseEnteredAndExited | NSTrackingActiveInActiveApp | NSTrackingInVisibleRect;
    
    NSTrackingAreaOptions options = (NSTrackingActiveAlways | NSTrackingInVisibleRect |
                                     NSTrackingMouseEnteredAndExited | NSTrackingMouseMoved);
    
    NSRect b = [self bounds];
    trackingArea = [[NSTrackingArea alloc] initWithRect:b
                                                                options:options
                                                                  owner:self
                                                               userInfo:nil];
    
    [self addTrackingArea:trackingArea];
    [super updateTrackingAreas];
    
    [[self window] makeFirstResponder:self];
    [[self window] setAcceptsMouseMovedEvents:YES];
}
@end


///////////////////////////////////////////////////


@interface AppDelegate : NSObject <NSApplicationDelegate>
@end

NSOpenGLView  *view = NULL;
NSView         *testView = NULL;

@implementation AppDelegate

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification {
    [NSApp stop:nil];
    // Insert code here to initialize your application
    
    // init window.
    //NSWindow *window = [[NSWindow alloc]
    //                    initWithContentRect:NSMakeRect(100, 100, 500, 500)
    //                    styleMask:NSBorderlessWindowMask
    //                    backing:NSBackingStoreBuffered
    //                    defer:NO];
    rootWindow = [[NSWindow alloc]
                        initWithContentRect:NSMakeRect(0,0,params->width,params->height)
                        styleMask:NSTitledWindowMask|NSClosableWindowMask|NSMiniaturizableWindowMask|NSResizableWindowMask
                        backing:NSBackingStoreBuffered
                        defer:YES];
    
    NSRect frame = [rootWindow contentRectForFrameRect:[rootWindow frame]];
    testView = [[MyView alloc] initWithFrame:frame];
    
    // configure window.
    [rootWindow setLevel:NSPopUpMenuWindowLevel];
    [rootWindow setHasShadow:NO];
    //[window setIgnoresMouseEvents:YES];
    //[rootWindow setAcceptsMouseMovedEvents:YES];
    
    [rootWindow setContentView:testView];
    [rootWindow makeFirstResponder:testView];
    
    
    [testView updateTrackingAreas];
    
    // show window.
    [rootWindow makeKeyAndOrderFront:self];
    //#endif
    
    
}


- (void)applicationWillTerminate:(NSNotification *)aNotification {
    // Insert code here to tear down your application
}


@end
//#endif

namespace sh
{
    
    MacDevice::MacDevice()
    {
        io::FileSystem::CreateInstance<io::MacFileSystem>();
        m_fileSystem = io::FileSystem::GetInstance();
        
        // Add engine's internal resources
        m_fileSystem->AddInternalFolder(sh::String("../../../engine/data"));
        
        m_creationParameters.driverType = video::DriverType::OPENGL_ES_2_0;
        
        params = &m_creationParameters;
        
        [NSApplication sharedApplication];
        
        AppDelegate *appDelegate = [[AppDelegate alloc] init];
        [NSApp setDelegate:appDelegate];
        [NSApp run];
        //[NSApp finishLaunching];
        
        CreateDriver();
        
        
    }
    
    MacDevice::MacDevice(const CreationParameters &parameters)
    {
        [NSApplication sharedApplication];
        
        AppDelegate *appDelegate = [[AppDelegate alloc] init];
        [NSApp setDelegate:appDelegate];
        [NSApp finishLaunching];
        //[NSApp run];
    }
    
    MacDevice::~MacDevice()
    {
        
    }
    
    
    void MacDevice::Init()
    {
        m_driver->SetWindow(m_creationParameters.WinId, m_creationParameters.width, m_creationParameters.height);
        m_driver->Init();
        m_driver->SetViewport(0U, 0U, m_creationParameters.width, m_creationParameters.height);
        
        Device::Init();
        
        m_application->Init();
    }
    
    void MacDevice::Update(f32 deltaTime)
    {
        
    }
    
    void MacDevice::Run()
    {
        // Loop until there is a quit message from the window or the user.
        bool done = false;
        while (!done)
        {
            // Handle the messages.
            @autoreleasepool
            {
                for (;;)
                {
                    NSEvent* event = [NSApp nextEventMatchingMask:NSEventMaskAny
                                                        untilDate:[NSDate distantPast]
                                                           inMode:NSDefaultRunLoopMode
                                                          dequeue:YES];
                    if (event == nil)
                        break;
                    
                    [NSApp sendEvent:event];
                }
            }
            
            
            // If windows signals to end the application then exit out.
            //if (msg.message == WM_QUIT || m_application->NeedsToBeClosed())
            if (false)
            {
                done = true;
                //m_application->Destroy();
                return;
            }
            const u64 currentTimePoint = GetTime();
            const u64 delta = currentTimePoint - m_lastFrameTimePoint;
            
            m_application->Update(delta);
            [view setNeedsDisplay:YES];
            
            m_lastFrameTimePoint = currentTimePoint;
        }
    }
    
    u64 MacDevice::GetTime()
    {
        return 0;
    }
    
    String MacDevice::ShowOpenFileDialog()
    {
        //String result;
        //NSString *str;// = [[NSString alloc] init];
        
        NSOpenPanel* openDlg = [NSOpenPanel openPanel];
        [openDlg setPrompt:@"Select"];
        [openDlg setCanChooseFiles:YES];
        [openDlg setCanChooseDirectories:NO];
        [openDlg setAllowsMultipleSelection:NO];
        [openDlg setMessage:@"Import one or more files or directories."];
        
//        [openDlg beginSheetModalForWindow:rootWindow completionHandler:^(NSInteger result){
//            if (result == NSFileHandlingPanelOKButton) {
//                NSArray* urls = [openDlg URLs];
//                NSURL* url = urls[0];
//                //str = [urls[0] absoluteString];
//
//                result = String([url.absoluteString copy]);
//            }
//
//        }];
        if ([openDlg runModal] == NSOKButton )
        //if ([openDlg runModalForDirectory:nil file:nil] == NSOKButton )
        {
            //NSArray* urls = [openDlg URLs];
            //NSURL* url = urls[0];
            
            NSString *FilePath = [NSString stringWithFormat:@"%@",[openDlg URL]];
            
            String subsrt = "file://";
            String result = String([FilePath UTF8String]);
            result.erase(0, subsrt.size());
            return result;
        }
        
        //result = String([str UTF8String]);
        return String();
    }
    
    String MacDevice::ShowSaveFileDialog()
    {
        return String();
    }
    
    bool MacDevice::CreateDriver()
    {
        switch (m_creationParameters.driverType)
        {
            case video::DriverType::OPENGL_ES_2_0:
            {
                video::NSOpenGLContextManager* contextManager = new video::NSOpenGLContextManager(2, 1, testView);
                if (contextManager)
                {
                    m_driver = new video::GLES20Driver(contextManager);
                    m_GLContextManager = contextManager;
                }
            }
                break;

            default:
                break;
        }
        return true;
    }
    
} // sh
