//
//  MediaSample.cpp
//  RtcSameple
//
//  Created by raymon_wang on 14-7-24.
//  Copyright (c) 2014年 yunwei. All rights reserved.
//

#include "MediaSample.h"

using namespace webrtc;

MediaSample::MediaSample() :
_voe(NULL)
{
    _voe = VoiceEngine::Create();
    
    init();
}

MediaSample::~MediaSample()
{
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

void MediaSample::connectRoom(const std::string &ip, unsigned int port)
{
    _voiceServerPort = port;
    _voiceServerIp = ip;
    
    int channel = onCreateChannel("111", data_in);
    
    setEncodeToIlbc(channel);
    
    VoEBase* voe_base = VoEBase::GetInterface(_voe);
    if (voe_base) {
        voe_base->StartPlayout(channel);
        voe_base->StartReceive(channel);
        voe_base->StartSend(channel);
    }
    
    VoEHardware* hardware = VoEHardware::GetInterface(_voe);
    hardware->SetLoudspeakerStatus(true);
    
    VoEVolumeControl* volumnControl = VoEVolumeControl::GetInterface(_voe);
    volumnControl->SetSpeakerVolume(255);
    volumnControl->SetMicVolume(255);
    volumnControl->SetChannelOutputVolumeScaling(channel, 10);
    
    onCreateChannel("222", data_out);
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

int MediaSample::onCreateChannel(const char *cname, MediaSample::DataDirection direction)
{
    VoEBase* voe_base = VoEBase::GetInterface(_voe);
    if (voe_base) {
        int channel = voe_base->CreateChannel();
        
        VoENetwork* network = VoENetwork::GetInterface(_voe);
        VoiceChannelTransport* voiceTransport = new VoiceChannelTransport(network, channel);
        
        voiceTransport->SetSendDestination(_voiceServerIp.c_str(), _voiceServerPort);
//        voiceTransport->SetLocalReceiver(20000);
        
        VoERTP_RTCP* rtcp = VoERTP_RTCP::GetInterface(_voe);
        rtcp->SetRTCP_CNAME(channel, cname);
        
        voiceTransport->sendActivateTransport(_voiceServerPort);
        
        _channelMap[channel] = ChannelInfo(channel, voiceTransport, direction, cname);
        
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
        const ChannelInfo& info = it->second;
        if (info.direction == data_out) {
            ret = info.channelID;
        }
    }
    return ret;
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




