//
//  MediaSample.cpp
//  RtcSameple
//
//  Created by raymon_wang on 14-7-24.
//  Copyright (c) 2014年 yunwei. All rights reserved.
//

#include "MediaSample.h"
#include "SdkPublic.h"
#include "RTChatSDKMain.h"
#include <webrtc/voice_engine/include/voe_file.h>

using namespace webrtc;

MediaSample::MediaSample() :
_voe(NULL),
_recvport(30000)
{
    _voe = VoiceEngine::Create();
    
    init();
}

MediaSample::~MediaSample()
{
    clearChannelData();
    
    VoiceEngine::Delete(_voe);
}

bool MediaSample::init()
{
    if (!_voe) {
        return false;
    }
    
    VoEBase* voe_base = VoEBase::GetInterface(_voe);
    if (voe_base) {
        voe_base->Init();
    }
    
    voe_base->RegisterVoiceEngineObserver(*this);
    
    return true;
}

void MediaSample::CallbackOnError(int channel, int errCode)
{
    sdklog("channel=%d, errCode=%d", channel, errCode);
}

void MediaSample::connectRoom(const std::string &ip, unsigned int port, uint64_t sdkID)
{
    _voiceServerPort = port;
    _voiceServerIp = ip;
    
//    VoEBase* voe_base = VoEBase::GetInterface(_voe);
//    int channel = voe_base->CreateChannel();
//    
//    VoEFile* voe_file = VoEFile::GetInterface(_voe);
//    voe_file->StartPlayingFileLocally(channel, "/var/mobile/Applications/A3B46D3F-0DF2-492A-AF52-4A4E6CFCE34F/RTChatTestAPP.app/audio_long16noise.pcm");
//    voe_base->StartPlayout(channel);
//    
//    
//    channel = voe_base->CreateChannel();
//    
//    voe_file->StartPlayingFileLocally(channel, "/var/mobile/Applications/A3B46D3F-0DF2-492A-AF52-4A4E6CFCE34F/RTChatTestAPP.app/audio_long16.pcm");
//    voe_base->StartPlayout(channel);
//    
//    return;
    
    int channel = onCreateChannel(sdkID, data_out);
    if (channel == -1) {
        //创建通道失败，直接返回
        return;
    }
    
    setEncodeTypeToChannel(channel, 102);
    
//    VoEBase* voe_base = VoEBase::GetInterface(_voe);
//    if (voe_base) {
//        voe_base->StartPlayout(channel);
//        voe_base->StartReceive(channel);
//    }
    
//    VoEHardware* hardware = VoEHardware::GetInterface(_voe);
//    hardware->SetLoudspeakerStatus(true);
    
    VoEVolumeControl* volumnControl = VoEVolumeControl::GetInterface(_voe);
    volumnControl->SetSpeakerVolume(255);
    volumnControl->SetMicVolume(255);
    volumnControl->SetChannelOutputVolumeScaling(channel, 10);
}

void MediaSample::leaveCurrentRoom()
{
    setReceiveMute(false);
    setMuteMic(true);
    clearChannelData();
}

//设置输出方向MIC是否静音
void MediaSample::setMuteMic(bool isMicMute)
{
    int channel = getAudioSendOutChannel();
    
    if (!_voe) {
        return;
    }
    
    VoEBase* voe_base = VoEBase::GetInterface(_voe);
    if (isMicMute) {
        if (voe_base) {
            voe_base->StopSend(channel);
        }
    }
    else {
        if (voe_base) {
            voe_base->StartSend(channel);
        }
    }
}

//设置全部通道输入方向是否接收语音
void MediaSample::setReceiveMute(bool isReceive)
{
    for (auto it = _channelMap.begin(); it != _channelMap.end(); ++it) {
        int channel = it->first;
        if (channel >= 0) {
            setChannelReceiveMute(channel, isReceive);
        }
    }
}

//设置1路通道输入方向是否接收语音
void MediaSample::setChannelReceiveMute(int channel, bool isReceive)
{
    if (!_voe) {
        return;
    }
    
    VoEBase* voe_base = VoEBase::GetInterface(_voe);
    if (voe_base) {
        if (isReceive) {
            voe_base->StartReceive(channel);
            voe_base->StartPlayout(channel);
        }
        else {
            voe_base->StopReceive(channel);
            voe_base->StopPlayout(channel);
        }
    }
}

int MediaSample::onCreateChannel(uint64_t id, MediaSample::DataDirection direction)
{
    std::string cname;
    if (direction == data_in) {
        cname = avar("r%llu@s%llu", id, RTChatSDKMain::sharedInstance().get_sdkTempID());
    }
    else {
        cname = avar("s%llu", id);
    }
    
    VoEBase* voe_base = VoEBase::GetInterface(_voe);
    if (voe_base) {
        int channel = voe_base->CreateChannel();
        
        VoENetwork* network = VoENetwork::GetInterface(_voe);
        VoiceChannelTransport* voiceTransport = new VoiceChannelTransport(network, channel);
        
        if (voiceTransport->SetSendDestination(_voiceServerIp.c_str(), _voiceServerPort) == -1) {
            sdklog("SetSendDestination 错误\n");
        }
        
        VoERTP_RTCP* rtcp = VoERTP_RTCP::GetInterface(_voe);
        rtcp->SetRTCP_CNAME(channel, cname.c_str());
        rtcp->RegisterRTCPObserver(channel, *this);
        rtcp->InsertExtraRTPPacket(channel, 20, false, "activate", 8);
        
        _channelMap[channel] = new ChannelInfo(channel, voiceTransport, direction, cname.c_str());
        
        if (direction == data_in) {
            sdklog("发送数据之前，channel=%d, cname=%s, recvport=%d\n", channel, cname.c_str(), _recvport);
            if (voiceTransport->SetLocalReceiver(_recvport) == -1) {
                sdklog("SetLocalReceiver失败\n");
            }
            if (voe_base->StartReceive(channel)) {
                sdklog("StartReceive失败\n");
            }
            if (voe_base->StartPlayout(channel) == -1) {
                sdklog("StartPlayout失败\n");
            }
            if (voe_base->StartSend(channel) == -1) {
                sdklog("StartSend失败, channel=%d\n", channel);
            }
            _recvport += 10;
            
            setVoiceOutChannelCode(0);
        }
        else {
            if (voiceTransport->SetLocalReceiver(_recvport) == -1) {
                sdklog("SetLocalReceiver失败\n");
            }
            if (voe_base->StartReceive(channel)) {
                sdklog("StartReceive失败\n");
            }
            if (voe_base->StartPlayout(channel) == -1) {
                sdklog("StartPlayout失败\n");
            }
            if (voe_base->StartSend(channel) == -1) {
                sdklog("StartSend失败, channel=%d\n", channel);
            }
            _recvport += 10;
        }
        
        return channel;
    }
    
    return -1;
}

//关闭删除数据通道
void MediaSample::onDeleteChannel(uint64_t id, MediaSample::DataDirection direction)
{
    std::string cname;
    if (direction == data_in) {
        cname = avar("r%llu@s%llu", id, RTChatSDKMain::sharedInstance().get_sdkTempID());
    }
    else {
        cname = avar("s%llu", id);
    }
    
    int channel = -1;
    for (auto it = _channelMap.begin(); it != _channelMap.end(); ++it) {
        const ChannelInfo* info = it->second;
        if (!strncasecmp(info->name, cname.c_str(), sizeof(info->name))) {
            channel = info->channelID;
            sdklog("移除%d号通道, name=%s", channel, info->name);
            setChannelReceiveMute(channel, false);
            VoEBase* voe_base = VoEBase::GetInterface(_voe);
            if (voe_base) {
                voe_base->DeleteChannel(channel);
            }
            delete it->second;
            _channelMap.erase(it);
            break;
        }
    }
}

void MediaSample::closeVoiceEngine()
{
    leaveCurrentRoom();
    _voiceServerIp = "";
    _voiceServerPort = 0;
    _recvport = 30000;
}

//获取语音数据发送通道
int MediaSample::getAudioSendOutChannel()
{
    int ret = 0;
    for (auto it = _channelMap.begin(); it != _channelMap.end(); ++it) {
        const ChannelInfo* info = it->second;
        if (info->direction == data_out) {
            ret = info->channelID;
        }
    }
    return ret;
}

//清除通道数据
void MediaSample::clearChannelData()
{
    for (auto it = _channelMap.begin(); it != _channelMap.end(); ++it) {
        if (it->second) {
            delete it->second;
        }
    }
    
    _channelMap.clear();
}

//设置发送编码格式为ilbc
void MediaSample::setEncodeTypeToChannel(int channel, int codeType)
{
    VoECodec* codec = VoECodec::GetInterface(_voe);
    CodecInst inst;
    
    int num = codec->NumOfCodecs();
    for (int i = 0; i < num; i++) {
//        sdklog("%s=%d\n", inst.plname, inst.pltype);
        codec->GetCodec(i, inst);
        if (inst.pltype == codeType) {
            if (codec->SetSendCodec(channel, inst) == -1) {
                sdklog("设置编码格式为%d失败\n", inst.pltype);
            }
            break;
        }
    }
}

//把发送通道编码设置为codeType
void MediaSample::setVoiceOutChannelCode(int codeType)
{
    int channel = getAudioSendOutChannel();
    if (channel >= 0) {
        setEncodeTypeToChannel(channel, codeType);
    }
}

void MediaSample::OnApplicationDataReceived(int channel, unsigned char subType,
                                       unsigned int name, const unsigned char* data,
                                       unsigned short dataLengthInBytes)
{
    sdklog("收到关闭通道%d rtcp指令\n", channel);
    VoEBase* voe_base = VoEBase::GetInterface(_voe);
    if (voe_base) {
        voe_base->StopSend(channel);
    }
}




