package com.ztgame.embededvoice;

import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;

import org.webrtc.webrtcdemo.MediaEngine;
import org.webrtc.webrtcdemo.VoiceRxChannel;

import android.content.Context;
import android.util.Log;

import Cmd.Public.enRoomType;
import Cmd.Public.stMicInfo;
import Cmd.Public.stVoiceUserInfo;
import Cmd.Game.*;

public class VoiceRoom {

	private long roomId;
	private MediaEngine  voiceMediaEngine;
	private long myUserId;
	private ArrayList<stMicInfo> userMicInfo = new ArrayList<stMicInfo>();
	private ArrayList<VoiceRxChannel> freeRoomUsers = new ArrayList<VoiceRxChannel>();
	private enRoomType roomType;
	
	private String roomip;
	private int roomInputport;
	private int roomOutport;
	
	private boolean isSpeaking = false;
	
	private Context ctx;
	
	public enum RoomState
	{	
		uninited,
		creating,
		created,
		speaking,
	}
	
	public RoomState State = RoomState.uninited;
		

	public static VoiceRoom CreateRoom(enRoomType roomtype,long userid,Context context)
	{
		VoiceRoom room = new VoiceRoom(roomtype,userid,context);
		return room;
	}
	
	public VoiceRoom(enRoomType roomtype,long userid,Context context)
	{
		ctx = context;
		myUserId = userid;
		roomType = roomtype;
		State = RoomState.creating;
	}
		

	
	public void SetRoomId(long roomID)
	{
		roomId = roomID;
	}
	
	public boolean SetupMainVoiceChannel(String serverip,int receiveport,int sendport)
    {
		roomip = serverip;
		roomInputport = receiveport;
		roomOutport = sendport;
		
		if(voiceMediaEngine != null)
			 return false;
		
		voiceMediaEngine = new MediaEngine(ctx);
    	//mediaEngine.setAudioCodec(7);
		voiceMediaEngine.setAudioRxPort(roomInputport);
		voiceMediaEngine.setAudioTxPort(roomOutport);
		voiceMediaEngine.setRemoteIp(roomip);
    	
        
		voiceMediaEngine.setTrace(true);
		voiceMediaEngine.setDebuging(true);
		voiceMediaEngine.setAudio(true);
		voiceMediaEngine.setAudioCodec(voiceMediaEngine.getiLBCIndex());
    	
    	//mediaEngine.setAudioCodec(7);
		
		voiceMediaEngine.setSpeaker(true);
		voiceMediaEngine.setEc(true);
		voiceMediaEngine.setAgc(true);
		voiceMediaEngine.setNs(true);
		voiceMediaEngine.userid = myUserId;
		voiceMediaEngine.activeChannel();
		
		State = RoomState.created;
		if(roomType == enRoomType.ROOM_TYPE_FREE)
		{
			StartChat();
			
		}else
		{
			voiceMediaEngine.startListen();
		}
    	return true;
    }
	
	public boolean AddFreeRoomUser(long peerid)
	{
		Log.e("yibintest", "add user -------------------------peerid = " + peerid);
		VoiceRxChannel freeMicUser = voiceMediaEngine.addReceiveChl(peerid);
		freeRoomUsers.add(freeMicUser);
		if(freeRoomUsers.size() >= 2)
		{
			voiceMediaEngine.setAudioCodec(7);
		}
		return true;
	}
	
	public boolean DelFreeRoomUser(long peerid)
	{
		Log.e("yibintest", "delete user -------------------------peerid = " + peerid);
		for (Iterator<VoiceRxChannel> i = freeRoomUsers.iterator(); i.hasNext();)
		{  
			VoiceRxChannel freeChl = i.next();
			if(freeChl.peerUserId == peerid)
			{
				freeChl.stop();
				freeChl.dispose();
			}
		}  
		return true;
	}
	
	public void StartChat()
	{
		voiceMediaEngine.start();
	}
	
	public void StopChat()
	{
		voiceMediaEngine.stop();
	}
	
	public void MuteMic(boolean isMute)
	{
		voiceMediaEngine.muteMic(isMute);
	}
	
	public void UpdateMicQueue(List<stMicInfo> micInfoList)
	{
		userMicInfo.clear();
		userMicInfo.addAll(micInfoList);
	}
	
	public void UpateCurrentMic(cmdNotifyTakeMic micinfo)
	{
		if(roomType == enRoomType.ROOM_TYPE_QUEUE)
		{
			
		}else{
			
			
		}
		
		if(micinfo.getTempid() == myUserId)
			{   Log.e("yibintest", "Get Mic -------------------------" + micinfo.getTempid() + "myUserId = " + myUserId);
				voiceMediaEngine.startSay();
				isSpeaking = true;
			}else
			{
				Log.e("yibintest", "Leave Mic -------------------------" + micinfo.getTempid() + "myUserId = " + myUserId);
				if(isSpeaking)
				{
					voiceMediaEngine.stopSay();
					isSpeaking = false;
				}
			}
	}
	
	
	public void Dispose()
	{
		for (Iterator<VoiceRxChannel> i = freeRoomUsers.iterator(); i.hasNext();)
		{  
			VoiceRxChannel freeChl = i.next();
			freeChl.stop();
			freeChl.dispose();
		}  
		if(voiceMediaEngine != null)
			voiceMediaEngine.dispose();
	}
	
	
	
}
