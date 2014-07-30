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
_voe(NULL),
_channel(-1)
{
    _voe = VoiceEngine::Create();
    
    init();
}

MediaSample::~MediaSample()
{
    
}

bool MediaSample::init()
{
    if (!_voe) {
        return false;
    }
    
    VoEBase* voe_base = VoEBase::GetInterface(_voe);
    if (voe_base) {
        voe_base->Init();
        _channel = voe_base->CreateChannel();
        
        VoENetwork* network = VoENetwork::GetInterface(_voe);
        _voiceTransport = new VoiceChannelTransport(network, _channel);
    }
    
    VoEHardware* hardware = VoEHardware::GetInterface(_voe);
    hardware->SetLoudspeakerStatus(true);

//    voe_base->SetNetEQPlayoutMode(0, kNetEqDefault);
//    VoEVolumeControl* volumnControl = VoEVolumeControl::GetInterface(_voe);
//    unsigned int volumn;
//    volumnControl->GetSpeakerVolume(volumn);
//    volumnControl->SetSpeakerVolume(volumn*10);
//    volumnControl->SetChannelOutputVolumeScaling(0, 5);
    
    voe_base->RegisterVoiceEngineObserver(*this);
    
    return true;
}

void MediaSample::CallbackOnError(int channel, int errCode)
{
    printf("channel=%d, errCode=%d", channel, errCode);
}

void MediaSample::connectRoom(const std::string &ip, unsigned int port)
{
    if (_voiceTransport) {
        _voiceTransport->SetSendDestination(ip.c_str(), port);
        _voiceTransport->SetLocalReceiver(20000);
    }
    
    VoEBase* voe_base = VoEBase::GetInterface(_voe);
    if (voe_base) {
        voe_base->StartPlayout(_channel);
        voe_base->StartReceive(_channel);
        voe_base->StartSend(_channel);
    }
}

void MediaSample::leaveCurrentRoom()
{
    VoEBase* voe_base = VoEBase::GetInterface(_voe);
    if (voe_base) {
        voe_base->StopPlayout(_channel);
        voe_base->StartReceive(_channel);
        voe_base->StartSend(_channel);
    }
}

void MediaSample::setMuteMic(bool isMicMute)
{
    if (!_voe) {
        return;
    }
    
    VoEBase* voe_base = VoEBase::GetInterface(_voe);
    if (isMicMute) {
        if (voe_base) {
            voe_base->StopSend(_channel);
        }
    }
    else {
        if (voe_base) {
            voe_base->StartSend(_channel);
        }
    }
}

//设置发送编码格式为ilbc
void MediaSample::setEncodeToIlbc()
{
    VoECodec* codec = VoECodec::GetInterface(_voe);
    CodecInst inst;
    
    int num = codec->NumOfCodecs();
    for (int i = 0; i < num; i++) {
        codec->GetCodec(i, inst);
        printf("%s\n", inst.plname);
        if (inst.pltype == 102) {
            codec->SetSendCodec(_channel, inst);
            break;
        }
    }
}




