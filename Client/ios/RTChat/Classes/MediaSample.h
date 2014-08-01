//
//  MediaSample.h
//  RtcSameple
//
//  Created by raymon_wang on 14-7-24.
//  Copyright (c) 2014年 yunwei. All rights reserved.
//

#ifndef __RtcSameple__MediaSample__
#define __RtcSameple__MediaSample__

#include <map>
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
    enum DataDirection {
        data_in = 1,
        data_out = 0
    };
    struct ChannelInfo {
        ChannelInfo() {
            bzero(this, sizeof(*this));
        }
        ChannelInfo(int id, VoiceChannelTransport* pVoiceChanel, DataDirection inDirection, const char* inName) {
            channelID = id;
            voiceChannel = pVoiceChanel;
            direction = inDirection;
            strncpy(name, inName, sizeof(name)-1);
        }
        ~ChannelInfo() {
            delete voiceChannel;
            voiceChannel = NULL;
        }
        int channelID;
        VoiceChannelTransport* voiceChannel;
        DataDirection direction;
        char          name[32];
    };
    typedef std::map<int, ChannelInfo> ChannelMap;
    
public:
    MediaSample();
    
    virtual ~MediaSample();
    
    bool init();
    
    void connectRoom(const std::string& ip, unsigned int port);
    
    void leaveCurrentRoom();
    
    void setMuteMic(bool isMicMute);
    
    void closeVoiceEngine();
    
    //创建数据通道
    int onCreateChannel(const char* cname, DataDirection direction);
    
protected:
    //设置发送编码格式为ilbc
    void setEncodeToIlbc(int channel);
    
    //获取语音数据发送通道
    int getAudioSendOutChannel();
    
    virtual void CallbackOnError(int channel, int errCode);
    
private:
    ChannelMap                      _channelMap;
    webrtc::VoiceEngine*            _voe;
    std::string                     _voiceServerIp;
    unsigned int                    _voiceServerPort;
};

#endif /* defined(__RtcSameple__MediaSample__) */
