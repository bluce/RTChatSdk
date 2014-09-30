#include "HelloWorldScene.h"
#include "RTChatSdk.h"
#include "cocos2d.h"
#include "ui/UIButton.h"
#include "RTChatCommonTypes.h"
#include <functional>

USING_NS_CC;
using namespace ui;
using namespace std::placeholders;


Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}


void HelloWorld::menuReqRoomlist(Ref* pSender)
{
    //RTChatSDKMain::sharedInstance().requestRoomList();
    //RTChatSDKMain::sharedInstance().requestLogin("test1222222---");
    //RTChatSDKMain::sharedInstance().leaveRoom();
     RTChatSDKMain::sharedInstance().startPlayLocalVoice("http://uploadchat.ztgame.com:10000/upload/2014090518/3906570ed724");
   CCLOG("menuReqRoomlist  callback ---");
}

void HelloWorld::createFreeRoomB(Ref* pSender)
{
    RTChatSDKMain::sharedInstance().createRoom(ROOM_TYPE_FREE,ROOM_REASON_RANDCHAT);
    CCLOG("createFreeRoomB  callback ---");
}

void HelloWorld::createQueueRoomB(Ref* pSender)
{
    //RTChatSDKMain::sharedInstance().createRoom(ROOM_TYPE_QUEUE);
    CCLOG("createQueueRoomB  callback ---");
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    RTChatSDKMain::sharedInstance().registerMsgCallback(std::bind(&HelloWorld::RTChatCallBack, this, _1, _2, _3, _4));
    CCLOG("Hellowork::init ---");
    RTChatSDKMain::sharedInstance().initSDK("jianghu123","cocos2dx32test","test11111111");
    isMuted = true;
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
    
	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    //Label* roomslabel = Label::createWithTTF("room list", "Arial", 30);
    MenuItemFont* reqRoomListBtn = MenuItemFont::create("获取房间列表", CC_CALLBACK_1(HelloWorld::menuReqRoomlist, this));
    //reqRoomListBtn->setColor(Color3B(0, 0xff, 0x00));
    
    reqRoomListBtn->setPosition(Vec2(100,300));
    
    
    MenuItemFont* createFreeRoom = MenuItemFont::create("创建自由房间", CC_CALLBACK_1(HelloWorld::createFreeRoomB, this));
    createFreeRoom->setPosition(Vec2(100,200));
    
    
    MenuItemFont* createQueueRoom = MenuItemFont::create("创建排麦房间", CC_CALLBACK_1(HelloWorld::createQueueRoomB, this));
    createQueueRoom->setPosition(Vec2(100,100));



    auto menu = Menu::create(closeItem,reqRoomListBtn, createFreeRoom,createQueueRoom,NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 2);



    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label
    
    auto label = LabelTTF::create("Hello World", "Arial", 24);
    
    // position the label on the center of the screen
    label->setPosition(Vec2(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - label->getContentSize().height));

    // add the label as a child to this layer
    this->addChild(label, 1);

    /**/
    // add "HelloWorld" splash screen"
    auto sprite = Sprite::create("HelloWorld.png");

    // position the sprite on the center of the screen
    sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

    // add the sprite as a child to this layer
    //this->addChild(sprite, 0);

    return true;
}

void HelloWorld::RTChatCallBack(SdkResponseCmd cmdType, SdkErrorCode error, const unsigned char* dataPtr, uint32_t dataSize)
{
    switch (cmdType) {
        case enNotifyLoginResult:
        {
            StNotifyLoginResult* data = (StNotifyLoginResult*)dataPtr;
            if (error == OPERATION_OK) {
                CCLOG("the login result === userid = %lld",data->tempid);
            }
            else {
                printf("登录失败\n");
            }
            break;
        }
        case enNotifyCreateResult:
        {
            StNotifyCreateResult* data = (StNotifyCreateResult*)dataPtr;
            if (data) {
                
            }
            break;
        }
        case enNotifyRoomList:
        {
            
            StNotifyRoomList* data = (StNotifyRoomList*)dataPtr;
            for (int i = 0; i < data->size; i++) {
               
            }
            break;
        }
        case enNotifyEnterResult:
        {
            StNotifyEnterResult* data = (StNotifyEnterResult*)dataPtr;
            if (error == OPERATION_OK) {
                printf("进入房间成功\n");
            }
            else {
                printf("进入房间失败\n");
            }
            break;
        }
        case enNotifyTakeMic:
        {
            StNotifyTakeMic* data = (StNotifyTakeMic*)dataPtr;
            if (error == OPERATION_OK) {
               
            }
            
            break;
        }
        default:
            break;
    }
    
    
    
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{

    
    if(isMuted)
    {
      isMuted = false;
      CCLOG("-----------------mic  open");
      RTChatSDKMain::sharedInstance().setMuteSelf(isMuted);
    }else
    {
      isMuted = true;
      CCLOG("------------------mic close");

    RTChatSDKMain::sharedInstance().setMuteSelf(isMuted);
    }

    /*
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
    return;
#endif



    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
*/
}
