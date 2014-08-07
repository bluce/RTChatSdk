//
//  MediaSample.cpp
//  RtcSameple
//
//  Created by raymon_wang on 14-7-24.
//  Copyright (c) 2014年 yunwei. All rights reserved.
//

#include "MediaSample.h"
#include "SdkPublic.h"

using namespace webrtc;

MediaSample::MediaSample() :
_voe(NULL)
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
    printf("channel=%d, errCode=%d", channel, errCode);
}

void MediaSample::connectRoom(const std::string &ip, unsigned int port, uint64_t sdkID)
{
    _voiceServerPort = port;
    _voiceServerIp = ip;
    
    int channel = onCreateChannel(sdkID, data_out);
    if (channel == -1) {
        //创建通道失败，直接返回
        return;
    }
    
    setEncodeToIlbc(channel);
    
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
//    VoEBase* voe_base = VoEBase::GetInterface(_voe);
//    if (voe_base) {
//        voe_base->StopPlayout(_channel);
//        voe_base->StopReceive(_channel);
//        voe_base->StopSend(_channel);
//    }
}

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

int MediaSample::onCreateChannel(uint64_t id, MediaSample::DataDirection direction)
{
    static int recvport = 20000;
    std::string cname;
    if (direction == data_in) {
        cname = avar("r%llu", id);
    }
    else {
        cname = avar("s%llu", id);
    }
    
    VoEBase* voe_base = VoEBase::GetInterface(_voe);
    if (voe_base) {
        int channel = voe_base->CreateChannel();
        
        VoENetwork* network = VoENetwork::GetInterface(_voe);
        VoiceChannelTransport* voiceTransport = new VoiceChannelTransport(network, channel);
        
        voiceTransport->SetSendDestination(_voiceServerIp.c_str(), _voiceServerPort);
        voiceTransport->SetLocalReceiver(recvport++);
        
        VoERTP_RTCP* rtcp = VoERTP_RTCP::GetInterface(_voe);
        rtcp->SetRTCP_CNAME(channel, cname.c_str());
        rtcp->RegisterRTCPObserver(channel, *this);
        rtcp->InsertExtraRTPPacket(channel, 0, false, "activate", 8);
        
        _channelMap[channel] = new ChannelInfo(channel, voiceTransport, direction, cname.c_str());
        
        if (direction == data_in) {
            voe_base->StartReceive(channel);
            voe_base->StartPlayout(channel);
        }
        else {
            voe_base->StartSend(channel);
        }
        
        return channel;
    }
    
    return -1;
}

void MediaSample::closeVoiceEngine()
{
    
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
void MediaSample::setEncodeToIlbc(int channel)
{
    VoECodec* codec = VoECodec::GetInterface(_voe);
    CodecInst inst;
    
    int num = codec->NumOfCodecs();
    for (int i = 0; i < num; i++) {
        codec->GetCodec(i, inst);
        if (inst.pltype == 102) {
            codec->SetSendCodec(channel, inst);
            break;
        }
    }
}

void MediaSample::OnApplicationDataReceived(int channel, unsigned char subType,
                                       unsigned int name, const unsigned char* data,
                                       unsigned short dataLengthInBytes)
{
    int i =0 ;
}




