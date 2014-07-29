//
//  MediaSample.h
//  RtcSameple
//
//  Created by raymon_wang on 14-7-24.
//  Copyright (c) 2014年 yunwei. All rights reserved.
//

#ifndef __RtcSameple__MediaSample__
#define __RtcSameple__MediaSample__

#include <iostream>
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
    
    void init();
    
    virtual void CallbackOnError(int channel, int errCode);
    
    virtual int ReceivedRTPPacket(int channel,
                                  const void* data,
                                  unsigned int length);
    
protected:
    webrtc::VoiceEngine*            _voe;
    webrtc::VoEBase*                _voe_base;
    VoiceChannelTransport*          _voiceSendTransport;
    VoiceChannelTransport*          _voiceRecvTransport;
};

#endif /* defined(__RtcSameple__MediaSample__) */
