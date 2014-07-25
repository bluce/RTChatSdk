//
//  ViewController.m
//  RTChatTest
//
//  Created by raymon_wang on 14-7-10.
//  Copyright (c) 2014年 yunwei. All rights reserved.
//

#import "ViewController.h"
#import "../RTChat/Classes/AQRecorder.h"
#import "../RTChat/Classes/AQPlayer.h"
#import "../RTChat/Classes/NetProcess/StreamConnection.h"

@interface ViewController ()
{
    AQRecorder*					recorder;
    StreamConnection*           connnection;
    AQPlayer*                   player;
}

@end

@implementation ViewController

- (void)viewDidLoad
{
    [super viewDidLoad];
	// Do any additional setup after loading the view, typically from a nib.
    recorder = new AQRecorder();
    
    player = new AQPlayer();
    
    connnection = new StreamConnection();
    if (connnection) {
        connnection->initConnection("180.168.126.249", 10086);
        connnection->sendLoginMsg();
    }
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
    else if ([sender tag] == 3) {
        CFStringRef recordFilePath = (__bridge CFStringRef)[NSTemporaryDirectory() stringByAppendingPathComponent: @"recordedFile.caf"];
        player->CreateQueueForFile(recordFilePath);
        player->StartQueue(false);
    }
}
@end
