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

class MediaSample : public webrtc::VoiceEngineObserver, public webrtc::VoERTCPObserver {
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
    typedef std::map<int, ChannelInfo*> ChannelMap;
    
public:
    MediaSample();
    
    virtual ~MediaSample();
    
    bool init();
    
    void connectRoom(const std::string& ip, unsigned int port, uint64_t sdkID);
    
    void leaveCurrentRoom();
    
    void setMuteMic(bool isMicMute);
    
    void closeVoiceEngine();
    
    //创建数据通道
    int onCreateChannel(uint64_t id, DataDirection direction);
    
protected:
    //设置发送编码格式为codeType
    void setEncodeTypeToChannel(int channel, int codeType);
    
    //把发送通道编码设置为codeType
    void setVoiceOutChannelCode(int codeType);
    
    //获取语音数据发送通道
    int getAudioSendOutChannel();
    
    //清除通道数据
    void clearChannelData();
    
    //VoiceEngineObserver回调
    virtual void CallbackOnError(int channel, int errCode);
    
    //VoERTCPObserver回调
    virtual void OnApplicationDataReceived(int channel, unsigned char subType,
                                           unsigned int name, const unsigned char* data,
                                           unsigned short dataLengthInBytes);
    
private:
    ChannelMap                      _channelMap;
    webrtc::VoiceEngine*            _voe;
    std::string                     _voiceServerIp;
    unsigned int                    _voiceServerPort;
    unsigned int                    _recvport;
};

#endif /* defined(__RtcSameple__MediaSample__) */
