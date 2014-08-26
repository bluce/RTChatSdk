/*
 *  Copyright (c) 2013 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

package org.webrtc.webrtcdemo;

import android.app.AlertDialog;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.IntentFilter;
import android.media.AudioManager;
import android.os.Environment;
import android.util.Log;
import android.widget.Toast;

import java.io.File;
import java.util.HashMap;

import org.webrtc.webrtcdemo.VoiceEngine.EcModes;

public class MediaEngine {
  
  private static final String LOG_DIR = "embedvoice";
  private static final int volumeLevel = 255;
  // Checks for and communicate failures to user (logcat and popup).
  private void check(boolean value, String message) {
    if (value) {
      return;
    }
    Log.e("VoiceEngine-CHECK", message);
    //Toast.makeText(context, message,Toast.LENGTH_SHORT).show();
  }


  // Shared Audio/Video members.
  private final Context context;
  private String remoteIp;
  private boolean enableTrace;

    // Audio
  private VoiceEngine voe;
  private int audioChannel;
  private boolean audioEnabled;
  private boolean voeRunning;
  private int audioCodecIndex;
  private int audioTxPort;
  private int audioRxPort;
  
  public long userid = 0;
  //客户端混音的多通道
  private HashMap<Long,VoiceRxChannel> rxChannels = new HashMap<Long,VoiceRxChannel>();

  private boolean speakerEnabled;
  private boolean headsetPluggedIn;
  private boolean enableAgc = true;
  private boolean enableNs = true;
  private boolean enableAecm = true;

  private BroadcastReceiver headsetListener;

  private boolean audioRtpDump;
  private boolean apmRecord;


  public MediaEngine(Context context) {
    this.context = context;
    voe = new VoiceEngine();
    check(voe.init() == 0, "Failed voe Init");
    audioChannel = voe.createChannel();
    check(audioChannel >= 0, "Failed voe CreateChannel");
    
    
    check(voe.setSpeakerVolume(volumeLevel) == 0,
        "Failed setSpeakerVolume");
  
    check(voe.setAecmMode(VoiceEngine.AecmModes.SPEAKERPHONE, false) == 0,
        "VoE set Aecm speakerphone mode failed");
    
    check(voe.setEcStatus(true,EcModes.CONFERENCE) == 0,
            "VoE set Ec mode CONFERENCE mode failed");
    
    
    // Set audio mode to communication
    AudioManager audioManager =
        ((AudioManager) context.getSystemService(Context.AUDIO_SERVICE));
    // Listen to headset being plugged in/out.
    IntentFilter receiverFilter = new IntentFilter(Intent.ACTION_HEADSET_PLUG);
    headsetListener = new BroadcastReceiver() {
        @Override
        public void onReceive(Context context, Intent intent) {
          if (intent.getAction().compareTo(Intent.ACTION_HEADSET_PLUG) == 0) {
            headsetPluggedIn = intent.getIntExtra("state", 0) == 1;
            updateAudioOutput();
          }
        }
      };
      
   
    context.registerReceiver(headsetListener, receiverFilter);
  }

  public void dispose() {
    stop();
    check(!voeRunning && !voeRunning, "Engines must be stopped before dispose");
    context.unregisterReceiver(headsetListener);
    check(voe.deleteChannel(audioChannel) == 0, "VoE delete channel failed");
    voe.dispose();
  }

  public void start() {
    if (audioEnabled) {
      startVoE();
    }
    /*
    if (receiveVideo || sendVideo) {
      startViE();
    }
    */
  }
  

  public void stop() {
    stopVoe();
    //stopVie();
  }
  
  public VoiceRxChannel addReceiveChl(long peerid)
  {
	  VoiceRxChannel rxChl = new VoiceRxChannel(peerid,voe,userid);
	  rxChl.Start(audioTxPort,remoteIp);
	  rxChannels.put(Long.valueOf(peerid),rxChl);
	  return rxChl;
  }
  
  public void deletReceiveChl(long peerid)
  {
	  VoiceRxChannel rxChl = rxChannels.get(Long.valueOf(peerid));
	  rxChl.stop();
	  rxChl.dispose();
	  rxChannels.remove(rxChl);
  }

  public boolean isRunning() {
    return voeRunning;
  }

  public void setRemoteIp(String remoteIp) {
    this.remoteIp = remoteIp;
    UpdateSendDestination();
  }

  public String remoteIp() { return remoteIp; }

  public void setTrace(boolean enable) {
    if (enable) {
      voe.setTraceFile("/sdcard/trace.txt", false);
      voe.setTraceFilter(VoiceEngine.TraceLevel.TRACE_ALL);
      return;
    }
   voe.setTraceFilter(VoiceEngine.TraceLevel.TRACE_NONE);
  }

  private String getDebugDirectory() {
    // Should create a folder in /scard/|LOG_DIR|
    return Environment.getExternalStorageDirectory().toString() + "/" +
        LOG_DIR;
  }

  private boolean createDebugDirectory() {
    File webrtc_dir = new File(getDebugDirectory());
    if (!webrtc_dir.exists()) {
      return webrtc_dir.mkdir();
    }
    return webrtc_dir.isDirectory();
  }
  
  public void activeChannel()
  {  
	  String cname = "s"+userid;
	  Log.e("MediaEngine","aactive Main Channel Cname" + cname);
	  check(voe.setRTCPCNAME(audioChannel, cname) == 0, "VoE start send failed");
	  check(voe.startSend(audioChannel) == 0, "VoE start send failed");
  }

  public void startVoE() {
    check(!voeRunning, "VoE already started");
    if(voeRunning) 
    	return;
    check(voe.startListen(audioChannel) == 0, "Failed StartListen");
    check(voe.startPlayout(audioChannel) == 0, "VoE start playout failed");
    check(voe.startSend(audioChannel) == 0, "VoE start send failed");
    
    //check(voe.insertExtraRTPPacket(audioChannel, "activate") == 0, "VoE start send failed");
    voeRunning = true;
  }
  
  public void startListen()
  {
	  check(voe.startListen(audioChannel) == 0, "Failed StartListen");
	  check(voe.startPlayout(audioChannel) == 0, "VoE start playout failed");
  }
  
  public void stopListen()
  {
	  check(voe.stopListen(audioChannel) == 0, "Failed StartListen");
	  check(voe.stopPlayout(audioChannel) == 0, "VoE start playout failed");
  }
  
  
  public void startSay()
  {
	  check(voe.startSend(audioChannel) == 0, "VoE start send failed");
  }
  
  public void stopSay()
  {
	  check(voe.stopSend(audioChannel) == 0, "VoE stop send failed");
  }

  private void stopVoe() {
	if(voeRunning == false) 
		return;
    check(voeRunning, "VoE not started");
    check(voe.stopSend(audioChannel) == 0, "VoE stop send failed");
    check(voe.stopPlayout(audioChannel) == 0, "VoE stop playout failed");
    check(voe.stopListen(audioChannel) == 0, "VoE stop listen failed");
    voeRunning = false;
  }

  public void setAudio(boolean audioEnabled) {
    this.audioEnabled = audioEnabled;
  }

  public boolean audioEnabled() { return audioEnabled; }

  public int audioCodecIndex() { return audioCodecIndex; }

  public void setAudioCodec(int codecNumber) {
    audioCodecIndex = codecNumber;
    CodecInst codec = voe.getCodec(codecNumber);
    check(voe.setSendCodec(audioChannel, codec) == 0, "Failed setSendCodec");
    codec.dispose();
   
  }
  
  
  public void setCName(String cname,int channel)
  {
	  voe.setRTCPCNAME(channel, cname);
  }
  
  public void insertExtraActivePacketOnChannel(int channel)
  {
	  voe.insertExtraRTPPacket(channel, "activate");
  }
  
  public void muteMic(boolean isMuteMic)
  {
	  voe.muteMic(audioChannel, isMuteMic);
  }

  public String[] audioCodecsAsString() {
    String[] retVal = new String[voe.numOfCodecs()];
    for (int i = 0; i < voe.numOfCodecs(); ++i) {
      CodecInst codec = voe.getCodec(i);
      retVal[i] = codec.toString();
      codec.dispose();
    }
    return retVal;
  }

  private CodecInst[] defaultAudioCodecs() {
    CodecInst[] retVal = new CodecInst[voe.numOfCodecs()];
     for (int i = 0; i < voe.numOfCodecs(); ++i) {
      retVal[i] = voe.getCodec(i);
    }
    return retVal;
  }

  public int getIsacIndex() {
    CodecInst[] codecs = defaultAudioCodecs();
    for (int i = 0; i < codecs.length; ++i) {
      if (codecs[i].name().contains("ISAC")) {
        return i;
      }
    }
    return 0;
  }
  
  public int getiLBCIndex() {
	    CodecInst[] codecs = defaultAudioCodecs();
	    for (int i = 0; i < codecs.length; ++i) {
	      if (codecs[i].name().contains("ILBC")) {
	        return i;
	      }
	    }
	    return 0;
	  }
  
  public int getAMRIndex() {
	    CodecInst[] codecs = defaultAudioCodecs();
	    for (int i = 0; i < codecs.length; ++i) {
	      if (codecs[i].name().contains("AMR")) {
	        return i;
	      }
	    }
	    return 0;
	  }

  public void setAudioTxPort(int audioTxPort) {
    this.audioTxPort = audioTxPort;
    UpdateSendDestination();
  }

  public int audioTxPort() { return audioTxPort; }

  public void setAudioRxPort(int audioRxPort) {
    check(voe.setLocalReceiver(audioChannel, audioRxPort) == 0,
        "Failed setLocalReceiver");
    this.audioRxPort = audioRxPort;
  }

  public int audioRxPort() { return audioRxPort; }

  public boolean agcEnabled() { return enableAgc; }

  public void setAgc(boolean enable) {
    enableAgc = enable;
    VoiceEngine.AgcConfig agc_config =
        new VoiceEngine.AgcConfig(3, 9, true);
    check(voe.setAgcConfig(agc_config) == 0, "VoE set AGC Config failed");
    check(voe.setAgcStatus(enableAgc, VoiceEngine.AgcModes.FIXED_DIGITAL) == 0,
        "VoE set AGC Status failed");
  }

  public boolean nsEnabled() { return enableNs; }

  public void setNs(boolean enable) {
    enableNs = enable;
    check(voe.setNsStatus(enableNs,
            VoiceEngine.NsModes.CONFERENCE) == 0,
        "VoE set NS Status failed");
  }

  public boolean aecmEnabled() { return enableAecm; }

  public void setEc(boolean enable) {
    enableAecm = enable;
    check(voe.setEcStatus(enable, VoiceEngine.EcModes.AECM) == 0,
        "voe setEcStatus");
  }

  public boolean speakerEnabled() {
    return speakerEnabled;
  }

  public void setSpeaker(boolean enable) {
    speakerEnabled = enable;
    updateAudioOutput();
  }

  // Debug helpers.
  public boolean apmRecord() { return apmRecord; }

  public boolean audioRtpDump() { return audioRtpDump; }

  public void setDebuging(boolean enable) {
    apmRecord = enable;
    if (!enable) {
      check(voe.stopDebugRecording() == 0, "Failed stopping debug");
      return;
    }
    if (!createDebugDirectory()) {
      check(false, "Unable to create debug directory.");
      return;
    }
    String debugDirectory = getDebugDirectory();
    //check(voe.startDebugRecording(debugDirectory +  String.format("/apm_%d.dat",
    //            System.currentTimeMillis())) == 0,
    //    "Failed starting debug");
  }

  public void setIncomingVoeRtpDump(boolean enable) {
    audioRtpDump = enable;
    if (!enable) {
      check(voe.stopRtpDump(audioChannel,
              VoiceEngine.RtpDirections.INCOMING) == 0,
          "voe stopping rtp dump");
      return;
    }
    String debugDirectory = getDebugDirectory();
    //check(voe.startRtpDump(audioChannel, debugDirectory +
    //        String.format("/voe_%d.rtp", System.currentTimeMillis()),
    //        VoiceEngine.RtpDirections.INCOMING) == 0,
    //    "voe starting rtp dump");
  }

  private void updateAudioOutput() {
    boolean useSpeaker = !headsetPluggedIn && speakerEnabled;
    AudioManager audioManager =
        ((AudioManager) context.getSystemService(Context.AUDIO_SERVICE));
    audioManager.setSpeakerphoneOn(useSpeaker);
  }


  private void UpdateSendDestination() 
  {
    if (remoteIp == null) {
      return;
    }
    if (audioTxPort != 0) {
      check(voe.setSendDestination(audioChannel, audioTxPort,
              remoteIp) == 0, "VoE set send destination failed");
    }
   
  }

  // Collates current state into a multiline string.
  public String sendReceiveState() {
    int packetLoss = 0;
    return null;
  }

  MediaEngineObserver observer;
  public void setObserver(MediaEngineObserver observer) 
  {
    this.observer = observer;
  }
  
}
