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
#include <webrtc/voice_engine/include/voe_audio_processing.h>
#include "RTChatBuffStream.h"

using namespace webrtc;
using namespace webrtc::test;

namespace rtchatsdk {

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
    
    //离开当前房间
    void leaveCurrentRoom();
    
    //设置输出方向MIC是否静音
    void setMuteMic(bool isMicMute);
    
    //设置全部通道输入方向是否接收语音
    void setReceiveMute(bool isReceive);
    
    //设置是否发送语音数据
    void setWetherSendVoiceData(bool isSend);
    
    //设置1路通道输入方向是否接收语音
    void setChannelReceiveMute(int channel, bool isReceive);
    
    void closeVoiceEngine();
    
    //创建数据通道
    int onCreateChannel(uint64_t id, DataDirection direction);
    
    //关闭删除数据通道
    void onDeleteChannel(uint64_t id, DataDirection direction);
    
    //开始录制麦克风数据
    bool startRecordVoice(OutStream* outstream);
    
    //停止录制麦克风数据
    bool stopRecordVoice();
    
    //开始播放本地录制的音频
    bool startPlayLocalStream(InStream* instream);
    
    //停止播放录制的音频
    bool stopPlayLocalStream();
    
    //获得录音数据时长
    uint64_t getRecordStreamTimeLength(OutStream* outstream);
    
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
    int                             _playChannel;
};
    
}

#endif /* defined(__RtcSameple__MediaSample__) */
