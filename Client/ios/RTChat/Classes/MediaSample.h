//
//  MediaSample.h
//  RtcSameple
//
//  Created by raymon_wang on 14-7-24.
//  Copyright (c) 2014年 yunwei. All rights reserved.
//

#ifndef __RtcSameple__MediaSample__
#define __RtcSameple__MediaSample__

#include <webrtc/test/channel_transport/include/channel_transport.h>
#include <webrtc/voice_engine/include/voe_base.h>
#include <webrtc/voice_engine/include/voe_volume_control.h>
#include <webrtc/voice_engine/include/voe_hardware.h>
#include <webrtc/voice_engine/include/voe_rtp_rtcp.h>
#include <webrtc/voice_engine/include/voe_codec.h>
#include <webrtc/voice_engine/include/voe_network.h>

using namespace webrtc;
using namespace webrtc::test;

class MediaSample : public webrtc::VoiceEngineObserver {
public:
    MediaSample();
    
    virtual ~MediaSample();
    
    bool init();
    
    void connectRoom(const std::string& ip, unsigned int port);
    
    void leaveCurrentRoom();
    
    void setMuteMic(bool isMicMute);
    
    void closeVoiceEngine();
    
protected:
    //设置发送编码格式为ilbc
    void setEncodeToIlbc();
    
    virtual void CallbackOnError(int channel, int errCode);
    
private:
    webrtc::VoiceEngine*            _voe;
    VoiceChannelTransport*          _voiceTransport;
    int                             _channel;
};

#endif /* defined(__RtcSameple__MediaSample__) */
