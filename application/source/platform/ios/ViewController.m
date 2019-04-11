#import "ViewController.h"
#import "MainView.h"

@interface ViewController ()
@property (weak, nonatomic) IBOutlet UILabel *helloWorldLabel;
@end

@implementation ViewController

- (void)viewDidLoad
{
    [super viewDidLoad];
    // Do any additional setup after loading the view, typically from a nib.
    
    self.helloWorldLabel.text = @"gfgffgfg";
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

@end
