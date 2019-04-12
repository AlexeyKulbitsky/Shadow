#import "ViewController.h"
#import "MainView.h"

#import <Metal/Metal.h>

#include <Shadow.h>

@implementation ViewController
{
    CADisplayLink* m_displayLink;
    CFTimeInterval m_lastDisplayTime;
}

- (void)viewDidLoad
{
    [super viewDidLoad];
    // Do any additional setup after loading the view, typically from a nib.
    
    m_displayLink = [CADisplayLink displayLinkWithTarget: self selector: @selector(update)];
    [m_displayLink addToRunLoop: [NSRunLoop currentRunLoop] forMode: NSDefaultRunLoopMode];
    
    m_lastDisplayTime = ((CADisplayLink*)m_displayLink).timestamp;
}


- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (void) loadView
{
    MainView* mainView = [[MainView alloc] initWithFrame:[UIScreen mainScreen].bounds];
    
    // Enable or disable multiple touches
    [mainView setMultipleTouchEnabled:YES];

    self.view = mainView;
}

- (void) update
{
    CFTimeInterval dt = ((CADisplayLink*)m_displayLink).timestamp - m_lastDisplayTime;
    m_lastDisplayTime = ((CADisplayLink*)m_displayLink).timestamp;
    
    sh::Device::GetInstance()->Update((float)dt);
}


@end
