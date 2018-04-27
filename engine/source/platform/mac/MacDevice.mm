#include "MacDevice.h"

#include "../../Application.h"
#include "../../io/mac/MacFileSystem.h"
#include "../../video/GL/GLES20/GLES20Driver.h"
#include "../../video/GL/GLContext/NSGLContextManager.h"

#include <GL/glew.h>

#import <Cocoa/Cocoa.h>


//short int keyCodes;
static const std::map<short int, sh::KeyCode> keyCodes =
{
    {0x00, sh::KeyCode::KEY_KEY_A},
    {0x0B, sh::KeyCode::KEY_KEY_B},
    {0x08, sh::KeyCode::KEY_KEY_C},
    {0x02, sh::KeyCode::KEY_KEY_D},
    {0x0E, sh::KeyCode::KEY_KEY_E},
    {0x03, sh::KeyCode::KEY_KEY_F},
    {0x05, sh::KeyCode::KEY_KEY_G},
    {0x04, sh::KeyCode::KEY_KEY_H},
    {0x22, sh::KeyCode::KEY_KEY_I},
    {0x26, sh::KeyCode::KEY_KEY_J},
    {0x28, sh::KeyCode::KEY_KEY_K},
    {0x25, sh::KeyCode::KEY_KEY_L},
    {0x2E, sh::KeyCode::KEY_KEY_M},
    {0x2D, sh::KeyCode::KEY_KEY_N},
    {0x1F, sh::KeyCode::KEY_KEY_O},
    {0x23, sh::KeyCode::KEY_KEY_P},
    {0x0C, sh::KeyCode::KEY_KEY_Q},
    {0x0F, sh::KeyCode::KEY_KEY_R},
    {0x01, sh::KeyCode::KEY_KEY_S},
    {0x11, sh::KeyCode::KEY_KEY_T},
    {0x20, sh::KeyCode::KEY_KEY_U},
    {0x09, sh::KeyCode::KEY_KEY_V},
    {0x0D, sh::KeyCode::KEY_KEY_W},
    {0x07, sh::KeyCode::KEY_KEY_X},
    {0x10, sh::KeyCode::KEY_KEY_Y},
    {0x06, sh::KeyCode::KEY_KEY_Z},
    
    {0x1D, sh::KeyCode::KEY_KEY_0},
    {0x12, sh::KeyCode::KEY_KEY_1},
    {0x13, sh::KeyCode::KEY_KEY_2},
    {0x14, sh::KeyCode::KEY_KEY_3},
    {0x15, sh::KeyCode::KEY_KEY_4},
    {0x17, sh::KeyCode::KEY_KEY_5},
    {0x16, sh::KeyCode::KEY_KEY_6},
    {0x1A, sh::KeyCode::KEY_KEY_7},
    {0x1C, sh::KeyCode::KEY_KEY_8},
    {0x19, sh::KeyCode::KEY_KEY_9},
    
    {0x2F, sh::KeyCode::KEY_PERIOD},
    
    {0x33, sh::KeyCode::KEY_BACK},
    {0x24, sh::KeyCode::KEY_RETURN},
    {0x7B, sh::KeyCode::KEY_LEFT},
    {0x7C, sh::KeyCode::KEY_RIGHT}
};

static sh::KeyCode translateFlags(NSUInteger flags)
{
    //int mods = 0;
    
//    if (flags & NSEventModifierFlagShift)
//        mods |= GLFW_MOD_SHIFT;
//    if (flags & NSEventModifierFlagControl)
//        mods |= GLFW_MOD_CONTROL;
//    if (flags & NSEventModifierFlagOption)
//        mods |= GLFW_MOD_ALT;
//    if (flags & NSEventModifierFlagCommand)
//        mods |= GLFW_MOD_SUPER;
//    if (flags & NSEventModifierFlagCapsLock)
//        mods |= GLFW_MOD_CAPS_LOCK;
    
    //return mods;
    
    sh::KeyCode code = sh::KeyCode::KEY_NONCONVERT;
    if (flags & NSEventModifierFlagOption)
        code = sh::KeyCode::KEY_MENU;
    
    return code;
}

static const NSRange kEmptyRange = { NSNotFound, 0 };

sh::CreationParameters* params = nullptr;

@interface MyView : NSView //<NSTextInputClient>
{
    NSTrackingArea *trackingArea;
}
@end

@implementation MyView
-(void) mouseUp:(NSEvent *)event
{
    NSPoint pos = [event locationInWindow];
    int x = pos.x;
    int y = self.bounds.size.height - pos.y;
    
    
    
    sh::Device* device = sh::Device::GetInstance();
    if (device)
        device->mouseEvent(x, y, sh::MouseEventType::ButtonReleased, sh::MouseCode::ButtonLeft);
    
    auto code = translateFlags([event modifierFlags]);
    sh::KeyboardEventType ev = sh::KeyboardEventType::KeyReleased;
    if (code == sh::KeyCode::KEY_MENU)
    {
        ev = sh::KeyboardEventType::KeyPressed;
    }
    if (device)
        device->keyboardEvent(ev, sh::KeyCode::KEY_MENU);
}

-(void) mouseDown:(NSEvent *)event
{
    NSPoint pos = [event locationInWindow];
    int x = pos.x;
    int y = self.bounds.size.height - pos.y;
    
    sh::Device* device = sh::Device::GetInstance();
    if (device)
        device->mouseEvent(x, y, sh::MouseEventType::ButtonPressed, sh::MouseCode::ButtonLeft);
    
    auto code = translateFlags([event modifierFlags]);
    sh::KeyboardEventType ev = sh::KeyboardEventType::KeyReleased;
    if (code == sh::KeyCode::KEY_MENU)
    {
        ev = sh::KeyboardEventType::KeyPressed;
    }
    if (device)
        device->keyboardEvent(ev, sh::KeyCode::KEY_MENU);
}

-(void) rightMouseDown:(NSEvent *)event
{
    NSPoint pos = [event locationInWindow];
    int x = pos.x;
    int y = self.bounds.size.height - pos.y;
    
    sh::Device* device = sh::Device::GetInstance();
    if (device)
        device->mouseEvent(x, y, sh::MouseEventType::ButtonPressed, sh::MouseCode::ButtonRight);
    
    auto code = translateFlags([event modifierFlags]);
    sh::KeyboardEventType ev = sh::KeyboardEventType::KeyReleased;
    if (code == sh::KeyCode::KEY_MENU)
    {
        ev = sh::KeyboardEventType::KeyPressed;
    }
    if (device)
        device->keyboardEvent(ev, sh::KeyCode::KEY_MENU);
}

-(void) rightMouseUp:(NSEvent *)event
{
    NSPoint pos = [event locationInWindow];
    int x = pos.x;
    int y = self.bounds.size.height - pos.y;
    
    sh::Device* device = sh::Device::GetInstance();
    if (device)
        device->mouseEvent(x, y, sh::MouseEventType::ButtonReleased, sh::MouseCode::ButtonRight);
    
    auto code = translateFlags([event modifierFlags]);
    sh::KeyboardEventType ev = sh::KeyboardEventType::KeyReleased;
    if (code == sh::KeyCode::KEY_MENU)
    {
        ev = sh::KeyboardEventType::KeyPressed;
    }
    if (device)
        device->keyboardEvent(ev, sh::KeyCode::KEY_MENU);
}

-(void) rightMouseDragged:(NSEvent *)event
{
    [self mouseMoved:event];
}

-(void) otherMouseDown:(NSEvent *)event
{
    NSPoint pos = [event locationInWindow];
    int x = pos.x;
    int y = self.bounds.size.height - pos.y;
    
    sh::Device* device = sh::Device::GetInstance();
    if (device)
        device->mouseEvent(x, y, sh::MouseEventType::ButtonPressed, sh::MouseCode::ButtonWheel);
    
    auto code = translateFlags([event modifierFlags]);
    sh::KeyboardEventType ev = sh::KeyboardEventType::KeyReleased;
    if (code == sh::KeyCode::KEY_MENU)
    {
        ev = sh::KeyboardEventType::KeyPressed;
    }
    if (device)
        device->keyboardEvent(ev, sh::KeyCode::KEY_MENU);
}

-(void) otherMouseUp:(NSEvent *)event
{
    NSPoint pos = [event locationInWindow];
    int x = pos.x;
    int y = self.bounds.size.height - pos.y;
    
    sh::Device* device = sh::Device::GetInstance();
    if (device)
        device->mouseEvent(x, y, sh::MouseEventType::ButtonReleased, sh::MouseCode::ButtonWheel);
    
    auto code = translateFlags([event modifierFlags]);
    sh::KeyboardEventType ev = sh::KeyboardEventType::KeyReleased;
    if (code == sh::KeyCode::KEY_MENU)
    {
        ev = sh::KeyboardEventType::KeyPressed;
    }
    if (device)
        device->keyboardEvent(ev, sh::KeyCode::KEY_MENU);
}

-(void) otherMouseDragged:(NSEvent *)event
{
    [self mouseMoved:event];
}

-(void) mouseMoved:(NSEvent *)event
{
    NSPoint pos = [event locationInWindow];
    int x = pos.x;
    int y = self.bounds.size.height - pos.y;
    
    //NSLog(@"Mouse moved: x=%d, y=%d", x, y);
    
    sh::Device* device = sh::Device::GetInstance();
    if (device)
        device->mouseEvent(x, y, sh::MouseEventType::Moved, sh::MouseCode::ButtonLeft);
    
    auto code = translateFlags([event modifierFlags]);
    sh::KeyboardEventType ev = sh::KeyboardEventType::KeyReleased;
    if (code == sh::KeyCode::KEY_MENU)
    {
        ev = sh::KeyboardEventType::KeyPressed;
    }
    if (device)
        device->keyboardEvent(ev, sh::KeyCode::KEY_MENU);
}

-(void) mouseDragged:(NSEvent *)event
{
    [self mouseMoved:event];
}

-(void) scrollWheel:(NSEvent *)event
{
    NSPoint pos = [event locationInWindow];
    int x = pos.x;
    int y = self.bounds.size.height - pos.y;
    
    int d = event.deltaY < 0 ? -1 : 1;
    sh::Device* device = sh::Device::GetInstance();
    if (device)
        device->mouseWheelEvent(x, y, d);
    
    auto code = translateFlags([event modifierFlags]);
    sh::KeyboardEventType ev = sh::KeyboardEventType::KeyReleased;
    if (code == sh::KeyCode::KEY_MENU)
    {
        ev = sh::KeyboardEventType::KeyPressed;
    }
    if (device)
        device->keyboardEvent(ev, sh::KeyCode::KEY_MENU);
}

-(void) keyDown:(NSEvent *)event
{
    auto keyCode = [event keyCode];
    auto it = keyCodes.find(keyCode);
    if (it == keyCodes.end())
        return;
    
    sh::Device* device = sh::Device::GetInstance();
    if (device)
        device->keyboardEvent(sh::KeyboardEventType::KeyPressed, it->second);
    NSLog(@"Key down");
    //[self acceptsFirstResponder];
    //[super keyDown:event];
}

-(void) keyUp:(NSEvent *)event
{
    auto keyCode = [event keyCode];
    auto it = keyCodes.find(keyCode);
    if (it == keyCodes.end())
        return;
    sh::Device* device = sh::Device::GetInstance();
    if (device)
        device->keyboardEvent(sh::KeyboardEventType::KeyReleased, it->second);
    NSLog(@"Key up");
    //[self acceptsFirstResponder];
    //[super keyUp:event];
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
//
//-(BOOL) canBecomeKeyView
//{
//    return YES;
//}
//
//-(BOOL) acceptsFirstResponder
//{
//    return YES;
//}
//
//- (BOOL)wantsUpdateLayer
//{
//    return YES;
//}
//
//- (void)insertText:(id)string replacementRange:(NSRange)replacementRange
//{
//
//}
//
//- (void)doCommandBySelector:(SEL)selector
//{
//
//}
//
//- (void)setMarkedText:(id)string selectedRange:(NSRange)selectedRange replacementRange:(NSRange)replacementRange
//{
//
//}
//
//- (void)unmarkText
//{
//
//}
//
//- (NSRange)selectedRange
//{
//    return kEmptyRange;
//}
//
//- (NSRange)markedRange
//{
//    return kEmptyRange;
//}
//
//- (BOOL)hasMarkedText
//{
//    return NO;
//}
//
//- (nullable NSAttributedString *)attributedSubstringForProposedRange:(NSRange)range actualRange:(nullable NSRangePointer)actualRange
//{
//    return nil;
//}
//
//- (NSArray<NSAttributedStringKey> *)validAttributesForMarkedText
//{
//    return [NSArray array];
//}
//
//- (NSRect)firstRectForCharacterRange:(NSRange)range actualRange:(nullable NSRangePointer)actualRange
//{
//    const NSRect contentRect = [self frame];
//    return contentRect;
//}
//
//- (NSUInteger)characterIndexForPoint:(NSPoint)point
//{
//    return 0;
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
                                     NSTrackingMouseEnteredAndExited | NSTrackingMouseMoved |
                                     NSTrackingEnabledDuringMouseDrag | NSTrackingMouseMoved);
    
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


@interface TestWindow : NSWindow {}
@end

@implementation TestWindow

- (BOOL)canBecomeKeyWindow
{
    // Required for NSWindowStyleMaskBorderless windows
    return YES;
}

- (BOOL)canBecomeMainWindow
{
    return YES;
}

@end

TestWindow *rootWindow;

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
    rootWindow = [[TestWindow alloc]
                        initWithContentRect:NSMakeRect(0,0,params->width,params->height)
                        styleMask:NSTitledWindowMask|NSClosableWindowMask|NSMiniaturizableWindowMask|NSResizableWindowMask
                        backing:NSBackingStoreBuffered
                        defer:YES];
    
    NSRect frame = [rootWindow contentRectForFrameRect:[rootWindow frame]];
    testView = [[MyView alloc] initWithFrame:frame];
    
    // configure window.
    [rootWindow setLevel:NSPopUpMenuWindowLevel];
    //[rootWindow setLevel:NSNormalWindowLevel];
    [rootWindow setAcceptsMouseMovedEvents:YES];
    
    [rootWindow setContentView:testView];
    [rootWindow makeFirstResponder:testView];
    
    
    [testView updateTrackingAreas];
    
    // show window.
    [rootWindow makeKeyAndOrderFront:self];
    //#endif
    
}


- (void)applicationWillTerminate:(NSNotification *)aNotification {
    // Insert code here to tear down your application
    int a = 0;
    a++;
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
        
        //[NSThread detachNewThreadSelector:@selector(doNothing:)
        //                         toTarget:NSApp
        //                       withObject:nil];
        
        [NSApp setActivationPolicy:NSApplicationActivationPolicyRegular];
        
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
