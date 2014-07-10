//
//  ViewController.m
//  RTChatTest
//
//  Created by raymon_wang on 14-7-10.
//  Copyright (c) 2014年 yunwei. All rights reserved.
//

#import "ViewController.h"
#import "../RTChat/Classes/AQRecorder.h"

@interface ViewController ()
{
    AQRecorder*					recorder;
}

@end

@implementation ViewController

- (void)viewDidLoad
{
    [super viewDidLoad];
	// Do any additional setup after loading the view, typically from a nib.
    recorder = new AQRecorder();
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (IBAction)onBtnClick:(id)sender
{
    if ([sender tag] == 1) {
        recorder->StartRecord();
    }
    else if ([sender tag] == 2) {
        recorder->StopRecord();
    }
}
@end
