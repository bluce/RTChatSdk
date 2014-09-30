package org.webrtc.webrtcdemo;

import android.util.Log;

public class VoiceRxChannel
{
  private int wtcIndex;
  private String cname;
  public long peerUserId;
  private VoiceEngine voe;
  private boolean isRunning = false;
  
  private final String TAG  ="VoiceRxChannel"; 
  
  private static int rxPort = 30000;
 
  public VoiceRxChannel(long userid,VoiceEngine ve,long myid)
  {
	  peerUserId = userid;
	  cname = "r"+peerUserId+"@s"+myid;
	  voe = ve;
	  wtcIndex = voe.createChannel();
  }
  
  public boolean Start(int port,String ipaddr)
  {
	  rxPort = rxPort + 10;
	  Log.d(TAG, "开启接受通道  peerid" + cname + "wtcInde =" + wtcIndex);
	  voe.setSendDestination(wtcIndex, port, ipaddr);
	  voe.setLocalReceiver(wtcIndex, rxPort);
	  voe.setRTCPCNAME(wtcIndex, cname);
	  voe.insertExtraRTPPacket(wtcIndex, "activate");
	  
	  
	  voe.startListen(wtcIndex);
	  voe.startPlayout(wtcIndex);
	  voe.startSend(wtcIndex);
	
	 
	  isRunning = true;
	  return true;
  }
  
 
  
  public boolean stop()
  {
	  voe.stopListen(wtcIndex);
	  voe.stopPlayout(wtcIndex);
	  isRunning = false;
	  return true;
  }
  
  
  public boolean dispose()
  {
	  voe.deleteChannel(wtcIndex);
	  return true;
  }
  
  public boolean isRunning()
  {
	  return false;
  }
}
