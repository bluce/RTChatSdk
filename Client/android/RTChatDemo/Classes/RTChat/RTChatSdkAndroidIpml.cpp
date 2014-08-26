//
//  RTChatSdkAndroidImpl.h
//  RTChat
//
//  Created by yibin on 14-8-14.
//  Copyright (c) 2014年 yunwei. All rights reserved.
//
#include "RTChatSdkAndroidIpml.h"
#include "RTChatCommonTypes.h"

#include <android/log.h>
#include <jni.h>
#include "cocos2d.h"
#include "../../cocos2d/cocos/platform/android/jni/JniHelper.h"
#include "../libs/cocos2dx/platform/android/jni/JniHelper.h"


#define MAX_BUFFER_SIZE 1024

template <class T>
inline void constructDynamic(T *ptr) {
    new ((void*)ptr) T();
};

#define BUFFER_CMD(cmd, name, len) char buffer##name[len]; \
    cmd* name = (cmd*)buffer##name; \
    constructDynamic(name); \



#define LOGD(msg)                                                  \
    __android_log_print(ANDROID_LOG_ERROR, "RTChatSDK", "%s:%d: %s", __FILE__, \
                        __LINE__, msg);                                \
// Macro for native functions that can be found by way of jni-auto discovery.
// Note extern "C" is needed for "discovery" of native methods to work.
#define JORT(rettype, name)                                             \
  extern "C" rettype JNIEXPORT JNICALL Java_com_ztgame_embededvoice_VoiceChannelEngine_##name

#define JVOEOBSERVER(rettype, name)                                             \
  extern "C" rettype JNIEXPORT JNICALL Java_com_ztgame_embededvoice_JNIObserver_##name

#define  CLASS_NAME "com/ztgame/embededvoice/VoiceChannelEngine"


static  RTChatSDKMain* s_RTChatSDKMain = NULL;
static jobject evoeObject = NULL;



RTChatSDKMain& RTChatSDKMain::sharedInstance()
{
    if (!s_RTChatSDKMain) {
        s_RTChatSDKMain = new RTChatSDKMain();
        //得到java 对象实例VoiceChannelEngineObserver
        
    }
    
    return *s_RTChatSDKMain;
}

jobject RTChatSDKMain::GetEVOEObject()
{
    cocos2d::JniMethodInfo voemethodInfo;
    if (! cocos2d::JniHelper::getStaticMethodInfo(voemethodInfo,CLASS_NAME, "GetIntance", "()Lcom/ztgame/embededvoice/VoiceChannelEngine;"))
    {
      return NULL;
    }
    
    jobject evoe = voemethodInfo.env->CallStaticObjectMethod(voemethodInfo.classID, voemethodInfo.methodID);

    return evoe;
}


void RTChatSDKMain::initSDK(const std::string& appid, const std::string& key,const char* uniqueid)
{
   
    cocos2d::JniMethodInfo methodInfo;
    if(!cocos2d::JniHelper::getMethodInfo(methodInfo,CLASS_NAME,"Init","(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V"))
    {
        return;
    }

    JNIEnv* env = methodInfo.env;

    jstring jappid = env->NewStringUTF(appid.c_str());
    jstring jappkey = env->NewStringUTF(key.c_str());

    jstring juniqueid;
    if(uniqueid != NULL)
        juniqueid = env->NewStringUTF(uniqueid);

    jobject evoe = GetEVOEObject();
    env->CallObjectMethod(evoe,methodInfo.methodID,jappid,jappkey,juniqueid);
}


 //激活SDK
void RTChatSDKMain::activateSDK()
{
     cocos2d::JniMethodInfo methodInfo;
    if(!cocos2d::JniHelper::getMethodInfo(methodInfo,CLASS_NAME,"activateSDK","()Z"))
    {
        return;
    }
    jobject evoe = GetEVOEObject();
    methodInfo.env->CallObjectMethod(evoe,methodInfo.methodID);
}
    
    //当应用最小化时需要调用这个，清理数据
void RTChatSDKMain::deActivateSDK()
{
    cocos2d::JniMethodInfo methodInfo;
    if(!cocos2d::JniHelper::getMethodInfo(methodInfo,CLASS_NAME,"deActivateSDK","()Z"))
    {
        return;
    }
    jobject evoe = GetEVOEObject();
    methodInfo.env->CallObjectMethod(evoe,methodInfo.methodID);

}
    
    //注册消息回调
void RTChatSDKMain::registerMsgCallback(const pMsgCallFunc& func)
{
   callbackfunc = func;
}
    
//请求登录in();
void RTChatSDKMain::requestLogin(const char* uniqueid)
{
    cocos2d::JniMethodInfo methodInfo;
    if(!cocos2d::JniHelper::getMethodInfo(methodInfo,CLASS_NAME,"loginServer","(Ljava/lang/String;)Z"))
    {
        return;
    }

    jstring juid;
    if(!uniqueid)
        juid = methodInfo.env->NewStringUTF(uniqueid);

    jobject evoe = GetEVOEObject();
    methodInfo.env->CallObjectMethod(evoe,methodInfo.methodID,juid);

}
    //申请房间列表
void RTChatSDKMain::requestRoomList()
{
    cocos2d::JniMethodInfo methodInfo;
    if(!cocos2d::JniHelper::getMethodInfo(methodInfo,CLASS_NAME,"ReqRoomList","()Z"))
    {
        return;
    }

    jobject evoe = GetEVOEObject();
    methodInfo.env->CallObjectMethod(evoe,methodInfo.methodID);
}
    
    //创建房间
void RTChatSDKMain::createRoom(enRoomType roomType,enRoomReason reason)
{
    cocos2d::JniMethodInfo methodInfo;
    if(!cocos2d::JniHelper::getMethodInfo(methodInfo,CLASS_NAME,"ReqCreateRoomByNative","(II)Z"))
    {
        return;
    }
    jobject evoe = GetEVOEObject();
    methodInfo.env->CallObjectMethod(evoe,methodInfo.methodID,(int)roomType,(int)reason);

}
    
    //加入房间
void RTChatSDKMain::joinRoom(uint64_t roomid)
{
    cocos2d::JniMethodInfo methodInfo;
    if(!cocos2d::JniHelper::getMethodInfo(methodInfo,CLASS_NAME,"ReqEnterRoom","(J)Z"))
    {
        return;
    }
    jobject evoe = GetEVOEObject();
    methodInfo.env->CallObjectMethod(evoe,methodInfo.methodID,roomid);

}

    //离开房间
void RTChatSDKMain::leaveRoom()
{
    cocos2d::JniMethodInfo methodInfo;
    if(!cocos2d::JniHelper::getMethodInfo(methodInfo,CLASS_NAME,"ReqLeaveRoom","(J)Z"))
    {
        return;
    }
    jobject evoe = GetEVOEObject();
    methodInfo.env->CallObjectMethod(evoe,methodInfo.methodID,0);
}
    
    //加入麦序
void RTChatSDKMain::requestInsertMicQueue()
{
    cocos2d::JniMethodInfo methodInfo;
    if(!cocos2d::JniHelper::getMethodInfo(methodInfo,CLASS_NAME,"ReqMicToken","()Z"))
    {
        return;
    }
    jobject evoe = GetEVOEObject();
    methodInfo.env->CallObjectMethod(evoe,methodInfo.methodID);

}
    
    //离开麦序
void RTChatSDKMain::leaveMicQueue()
{
     cocos2d::JniMethodInfo methodInfo;
    if(!cocos2d::JniHelper::getMethodInfo(methodInfo,CLASS_NAME,"ReqLeaveMicToken","()Z"))
    {
        return;
    }
    jobject evoe = GetEVOEObject();
    methodInfo.env->CallObjectMethod(evoe,methodInfo.methodID);
}

    //是否接收随机聊天，临时增加的接口
void RTChatSDKMain::returnRandChatRes(bool isAccept, uint64_t srctempid)
{
   cocos2d::JniMethodInfo methodInfo;
    if(!cocos2d::JniHelper::getMethodInfo(methodInfo,CLASS_NAME,"ReqAcceptRandChat","(ZJ)Z"))
    {
        return;
    }
    jobject evoe = GetEVOEObject();
    methodInfo.env->CallObjectMethod(evoe,methodInfo.methodID,isAccept,srctempid);

}
    
//设置本人Mac静音
void RTChatSDKMain::setMuteSelf(bool isMute)
{
    cocos2d::JniMethodInfo methodInfo;
    if(!cocos2d::JniHelper::getMethodInfo(methodInfo,CLASS_NAME,"ReqMuteSelf","(Z)Z"))
    {
        return;
    }
    jobject evoe = GetEVOEObject();
    methodInfo.env->CallObjectMethod(evoe,methodInfo.methodID,isMute);
}

/*
public native int RoomListResult(stRoomInfo[] roomsinfo);
public native int MicQueueResult(stMicInfo[] micsinfo);
public native int MicTakeNotify(cmdNotifyTakeMic micinfo);
public native int EnterRoomNotify(long roomid,int roomtype);
*/

uint64_t RTChatSDKMain::convertJstring2Uint64(jstring longstring)
{
    std::string sTempID = cocos2d::JniHelper::jstring2string(longstring);
    uint64_t  temp64id = strtoull(sTempID.c_str(),NULL,10);
    return temp64id;
}

JVOEOBSERVER(int,LoginResult)(JNIEnv* jni,jobject thiz,jboolean result,jstring jtempid)
{
  bool isOK = result;
  
   if(isOK)
   {
    uint64_t temp64id = RTChatSDKMain::sharedInstance().convertJstring2Uint64(jtempid);
    __android_log_print(ANDROID_LOG_ERROR, "RTChatSDK","---room temp id  = %llu",temp64id);
    StNotifyLoginResult callbackdata(isOK, temp64id);
    RTChatSDKMain::sharedInstance().callbackfunc(enNotifyLoginResult, OPERATION_OK, (const unsigned char*)&callbackdata, sizeof(StNotifyLoginResult));
    
   }else
   {
     RTChatSDKMain::sharedInstance().callbackfunc(enNotifyLoginResult, LOGIN_RESULT_TOKEN_ERROR, NULL, 0);
   }

  return 0;
}

JVOEOBSERVER(int,CreateResult)(JNIEnv* jni,jobject thiz ,jboolean result,jstring jroomid,jint jroomtype)
{
  
  bool isOK = result;
  uint64_t  roomid = RTChatSDKMain::sharedInstance().convertJstring2Uint64(jroomid);
  enRoomType roomtype = (enRoomType) jroomtype;
  if(isOK)
  {
    StNotifyCreateResult callbackdata(true, roomid, roomtype);
    RTChatSDKMain::sharedInstance().callbackfunc(enNotifyCreateResult, OPERATION_OK, (const unsigned char*)&callbackdata, sizeof(StNotifyCreateResult));
  }else
  {
    RTChatSDKMain::sharedInstance().callbackfunc(enNotifyCreateResult, OPERATION_FAILED, NULL, 0);
  }
  return 0;
}

JVOEOBSERVER(int,EnterRoomNotify)(JNIEnv* jni,jobject thiz,jstring jroomid,jint roomtype)
{
     uint64_t  roomid = RTChatSDKMain::sharedInstance().convertJstring2Uint64(jroomid);

     if (roomtype == 1) 
     {
        StNotifyEnterResult callbackdata(roomid, ROOM_TYPE_QUEUE);
        RTChatSDKMain::sharedInstance().callbackfunc(enNotifyEnterResult, OPERATION_OK, (const unsigned char*)&callbackdata, sizeof(StNotifyEnterResult));
     }
    else if (roomtype == 2) 
     {
        StNotifyEnterResult callbackdata(roomid, ROOM_TYPE_FREE);
        RTChatSDKMain::sharedInstance().callbackfunc(enNotifyEnterResult, OPERATION_OK, (const unsigned char*)&callbackdata, sizeof(StNotifyEnterResult));
     }
  return 0;
}


JVOEOBSERVER(int,RoomListResult)(JNIEnv* jni,jobject thiz ,jobjectArray roomidarray,jintArray roomtypearray,jintArray counts)
{
    jint size = jni->GetArrayLength(roomidarray);
    jint* c_roomtypearray = jni->GetIntArrayElements(roomtypearray,NULL);
    jint* c_roomCntArray = jni->GetIntArrayElements(counts,NULL);
    BUFFER_CMD(StNotifyRoomList, roomList, MAX_BUFFER_SIZE);
    roomList->size = size;
    for (int i = 0; i < size; i++) 
    {
        StRoomInfo info;
        jstring jroomid = (jstring)(jni)->GetObjectArrayElement(roomidarray,i);
        info.roomid =  RTChatSDKMain::sharedInstance().convertJstring2Uint64(jroomid);
        __android_log_print(ANDROID_LOG_ERROR, "RTChatSDK","list---roomid = %llu",info.roomid);
        info.roomtype = (enRoomType)c_roomtypearray[i];
        info.num = c_roomCntArray[i];
        roomList->roominfo[i] = info;
    }
    RTChatSDKMain::sharedInstance().callbackfunc(enNotifyRoomList, OPERATION_OK, (const unsigned char*)roomList, roomList->getSize());
    jni->ReleaseIntArrayElements(roomtypearray, c_roomtypearray, 0);
    jni->ReleaseIntArrayElements(counts, c_roomCntArray, 0);
  return 0;
}

JVOEOBSERVER(int,AddVoiceUser)(JNIEnv* jni,jobject thiz ,jobjectArray addvoiceuser)
{
  jint size = jni->GetArrayLength(addvoiceuser);
  BUFFER_CMD(StNotifyAddVoiceUser, userList, MAX_BUFFER_SIZE);
   userList->size = size;
    for (int i = 0; i < size; i++) 
    {
      StVoiceUserInfo info;
      jstring juserid = (jstring)(jni)->GetObjectArrayElement(addvoiceuser,i);
      info.userid = RTChatSDKMain::sharedInstance().convertJstring2Uint64(juserid);
      userList->userinfo[i] = info;
    }

    RTChatSDKMain::sharedInstance().callbackfunc(enNotifyAddVoiceUser, OPERATION_OK, (const unsigned char*)userList, userList->getSize());
  return 0;
}

JVOEOBSERVER(int,DelVoiceUser)(JNIEnv* jni,jobject thiz ,jobjectArray delvoiceuser)
{
  LOGD("---------------------");

  return 0;
}

JVOEOBSERVER(int,MicQueueResult)(JNIEnv* jni,jobject thiz ,jobjectArray micarray,jobjectArray uniqueidArray)
{
  LOGD("---------------------");
  jint size = jni->GetArrayLength(micarray);
  BUFFER_CMD(StNotifyMicQueue, micList, MAX_BUFFER_SIZE);
  micList->size = size;
  for (int i = 0; i < size; i++) 
  {
    StMicInfo info;
    jstring juserid = (jstring)(jni)->GetObjectArrayElement(micarray,i);
    jstring jUniqueID = (jstring)(jni)->GetObjectArrayElement(uniqueidArray,i);
    info.tempid = RTChatSDKMain::sharedInstance().convertJstring2Uint64(juserid);
    std::string uniqueid = cocos2d::JniHelper::jstring2string(jUniqueID);
    bcopy(uniqueid.c_str(), info.uniqueid, sizeof(info.uniqueid));
    micList->micinfo[i] = info;
  }
            
  RTChatSDKMain::sharedInstance().callbackfunc(enNotifyAddVoiceUser, OPERATION_OK, (const unsigned char*)micList, micList->getSize());
  return 0;
}

JVOEOBSERVER(int,MicTakeNotify)(JNIEnv* jni,jobject thiz ,jstring jtempid,jstring juniqueid,jint jmtime)
{
  LOGD("---------------------");
  uint64_t tempid = RTChatSDKMain::sharedInstance().convertJstring2Uint64(jtempid);
  std::string uniqueid = cocos2d::JniHelper::jstring2string(juniqueid);
  uint32_t mtime = jmtime;

  StNotifyTakeMic micdata(tempid, uniqueid.c_str(),mtime);
  RTChatSDKMain::sharedInstance().callbackfunc(enNotifyTakeMic, OPERATION_OK, (const unsigned char*)&micdata, sizeof(StNotifyTakeMic));
  return 0;
}

JVOEOBSERVER(int,SomeEnterRoom)(JNIEnv* jni,jobject thiz ,jobjectArray jtempidArray,jobjectArray uniqueidArray)
{
  LOGD("---------------------");
  jint size = jni->GetArrayLength(jtempidArray);
  LOGD("---------------------");
  BUFFER_CMD(StNotifySomeEnterRoom, userList, MAX_BUFFER_SIZE);
  LOGD("---------------------");
  userList->size = size;
  LOGD("---------------------");
  for (int i = 0; i < size; i++) 
  {   LOGD("---------------------");
      stRoomUserInfo info;
      LOGD("---------------------");
      jstring juserid = (jstring)(jni)->GetObjectArrayElement(jtempidArray,i);
      LOGD("---------------------");
      jstring jUniqueID = (jstring)(jni)->GetObjectArrayElement(uniqueidArray,i);
      LOGD("---------------------");
      info.tempid = RTChatSDKMain::sharedInstance().convertJstring2Uint64(juserid);
      LOGD("---------------------");
      std::string uniqueid = cocos2d::JniHelper::jstring2string(jUniqueID);
      LOGD("---------------------");
      bcopy(uniqueid.c_str(), info.uniqueid, sizeof(info.uniqueid));
      LOGD("---------------------");
      userList->userinfo[i] = info;
      LOGD("---------------------");
  }
            
  RTChatSDKMain::sharedInstance().callbackfunc(enNotifySomeEnterRoom, OPERATION_OK, (const unsigned char*)userList, userList->getSize());
  return 0;
}

JVOEOBSERVER(int,SomeLeaveRoom)(JNIEnv* jni,jobject thiz ,jstring jtempid)
{
  LOGD("---------------------");
  uint64_t tempid = RTChatSDKMain::sharedInstance().convertJstring2Uint64(jtempid); 
  StNotifySomeLeaveRoom callbackdata;
  callbackdata.tempid = tempid;
            
  RTChatSDKMain::sharedInstance().callbackfunc(enNotifySomeLeaveRoom, OPERATION_OK, (const unsigned char*)&callbackdata, sizeof(StNotifySomeLeaveRoom));

  return 0;
}

JVOEOBSERVER(int,NotitfyRandChat)(JNIEnv* jni,jobject thiz ,jstring jtempid,jstring jUniqueID,jstring jroomid)
{
  LOGD("---------------------");
  uint64_t tempid = RTChatSDKMain::sharedInstance().convertJstring2Uint64(jtempid); 
  uint64_t roomid = RTChatSDKMain::sharedInstance().convertJstring2Uint64(jroomid); 

  std::string uniqueid = cocos2d::JniHelper::jstring2string(jUniqueID);
  StNotifyRandChat callbackdata(tempid, uniqueid.c_str(),roomid);
  
  RTChatSDKMain::sharedInstance().callbackfunc(enNotifyRandChat, OPERATION_OK, (const unsigned char*)&callbackdata, sizeof(StNotifyRandChat));

  return 0;
}




JORT(void,initNativeSDK)(JNIEnv* jni,jobject embededvoiceengine)
{
    evoeObject = embededvoiceengine;
    LOGD("------------------initNativeSDK-----");
}



