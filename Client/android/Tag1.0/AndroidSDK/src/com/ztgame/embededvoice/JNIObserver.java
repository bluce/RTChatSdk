package com.ztgame.embededvoice;

import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;

import android.util.Log;
import Cmd.Game.cmdNotifyAddVoiceUser;
import Cmd.Game.cmdNotifyCreateResult;
import Cmd.Game.cmdNotifyDelVoiceUser;
import Cmd.Game.cmdNotifyLoginResult;
import Cmd.Game.cmdNotifyLoginResult.enLoginResult;
import Cmd.Game.cmdNotifyTakeMic;
import Cmd.Public.enRoomType;
import Cmd.Public.stMicInfo;
import Cmd.Public.stRoomInfo;
import Cmd.Public.stRoomUserInfo;
import Cmd.Public.stVoiceUserInfo;

public class JNIObserver implements VoiceChannelEngineObserver {

	private final String TAG = "JNIObserver";
	@Override
	public void OnRoomListNotify(List<stRoomInfo> roomlist) {
		// TODO Auto-generated method stub
		String roomsID[] = new String[roomlist.size()];
		int roomsType[] = new int[roomlist.size()];
		int counts[] = new int[roomlist.size()];
		int j = 0;
		for (Iterator<stRoomInfo> i = roomlist.iterator(); i.hasNext();)
		{  
			stRoomInfo userinfoRef = i.next(); 
			roomsID[j] = String.valueOf(userinfoRef.getRoomid());
			roomsType[j] = userinfoRef.getRoomtype().ordinal();
			counts[j] = userinfoRef.getNum();
			j++;
		} 
		
		RoomListResult(roomsID,roomsType,counts);
			
	}

	@Override
	public void OnMicQueueNotify(List<stMicInfo> micinfolists) {
		// TODO Auto-generated method stub
		
		String tempIDS[] = new String[micinfolists.size()];
		String uniqueIDS[] = new String[micinfolists.size()];
	
		int j = 0;
		for (Iterator<stMicInfo> i = micinfolists.iterator(); i.hasNext();)
		{  
			stMicInfo userinfoRef = i.next(); 
			tempIDS[j] = String.valueOf(userinfoRef.getTempid());
			uniqueIDS[j] = userinfoRef.getUniqueid();
			j++;
		} 
		MicQueueResult(tempIDS,uniqueIDS);
	}

	@Override
	public void OnMicTakeNotify(cmdNotifyTakeMic micinfo) {
		// TODO Auto-generated method stub
		if(micinfo != null)
			MicTakeNotify(String.valueOf(micinfo.getTempid()),micinfo.getUniqueid());
	}

	@Override
	public void OnEnterRoomNotify(long roomid, enRoomType roomtype) 
	{
		// TODO Auto-generated method stub
		Log.d(TAG, "OnEnterRoomNotify  -------1");
		if(roomid > 0)
		{
			Log.d(TAG, "OnEnterRoomNotify  -------1");
			EnterRoomNotify(String.valueOf(roomid),roomtype.ordinal());
		}
		
	}
	


	@Override
	public void OnLoginResult(cmdNotifyLoginResult loginresult) {
		// TODO Auto-generated method stub
		boolean isok = false;
		long temid = 0;
		if(loginresult.getResult() == enLoginResult.LOGIN_RESULT_OK)
		{
			isok = true;
			temid = loginresult.getTempid();
		}
		
		LoginResult(isok,String.valueOf(temid));
	}

	@Override
	public void OnCreateResult(cmdNotifyCreateResult createresult) {
		// TODO Auto-generated method stub
		boolean isOK = createresult.getIsok();
		long roomid = createresult.getRoomid();
		int roomtype = createresult.getType().ordinal();
		Log.d(TAG, "OnCreateResult  -------1");
		CreateResult(isOK,String.valueOf(roomid),roomtype);
	}

	@Override
	public void OnAddVoiceUser(List<stVoiceUserInfo> voiceuser) {
		// TODO Auto-generated method stub
		stVoiceUserInfo[] userinfos = new stVoiceUserInfo[voiceuser.size()];
		int j = 0;
		for (Iterator<stVoiceUserInfo> i = voiceuser.iterator(); i.hasNext();)
		{  
			stVoiceUserInfo userinfoRef = i.next(); 
			userinfos[j] = userinfoRef;
			j++;
		} 
		
	}

	@Override
	public void OnDelVoiceUser(List<stVoiceUserInfo> voiceuser) {
		// TODO Auto-generated method stub
		stVoiceUserInfo[] userinfos = new stVoiceUserInfo[voiceuser.size()];
		int j = 0;
		for (Iterator<stVoiceUserInfo> i = voiceuser.iterator(); i.hasNext();)
		{  
			stVoiceUserInfo userinfoRef = i.next(); 
			userinfos[j] = userinfoRef;
			j++;
		} 
	}
	

	@Override
	public void OnRoomUsersEnter(List<stRoomUserInfo> roomUsersList,boolean isAll) {
		// TODO Auto-generated method stub
		String tempIDs[] = new String[roomUsersList.size()];
		String uniqueIDS[] = new String[roomUsersList.size()];
		int power[] = new int[roomUsersList.size()];
		int j = 0;
		for (Iterator<stRoomUserInfo> i = roomUsersList.iterator(); i.hasNext();)
		{  
			stRoomUserInfo userinfoRef = i.next(); 
			tempIDs[j] = String.valueOf(userinfoRef.getTempid());
			uniqueIDS[j] = userinfoRef.getUniqueid();
			power[j] = userinfoRef.getPower().ordinal();
			j++;
		} 
		SomeEnterRoom(tempIDs,uniqueIDS,power,isAll);
	}

	@Override
	public void OnRoomUserLeave(long leaveRoomUser) {
		// TODO Auto-generated method stub
		SomeLeaveRoom(String.valueOf(leaveRoomUser));
	}

	@Override
	public void OnRandChatInvite(long tempid, long roomid, String uniqueid) {
		// TODO Auto-generated method stub
		NotitfyRandChat(String.valueOf(tempid),uniqueid,String.valueOf(roomid));
	}
	
	@Override
	public void OnStopVoiceRecording(long size) {
		// TODO Auto-generated method stub
		RecordingStop(size);
	}

	@Override
	public void OnUploadVoiceEnd(String cloudvoiceurl,int duration) {
		// TODO Auto-generated method stub
		RecordingUploadEnd(cloudvoiceurl,cloudvoiceurl.length(),duration);
	}

	@Override
	public void OnDlVoiceBegin() {
		// TODO Auto-generated method stub
		RecordingVoiceDownloadBegin();
	}

	@Override
	public void OnDlVoiceEnd(boolean isOk, long size) {
		// TODO Auto-generated method stub
		RecordingVoiceDownloadEnd(isOk,size);
	}

	@Override
	public void OnRandomRecording(String url) {
		// TODO Auto-generated method stub
		RecordingVoiceIncoming(url);
	}

	@Override
	public void OnNotifyUpdatePower(long tempid, int power) {
		// TODO Auto-generated method stub
		NotifyPowerUpdate(String.valueOf(tempid),power);
	}

	@Override
	public void OnNotifyAssignRet(int ret) {
		// TODO Auto-generated method stub
		ResultAssignRet(ret);
	}
	
	@Override
	public void OnResultUpPwder(int result) {
		// TODO Auto-generated method stub
		//ResultUpdatePowerRet(result);
	}
   
	public native int LoginResult(boolean isOK,String tempid);
	public native int CreateResult(boolean isOK,String roomid,int roomtype);
	public native int SomeEnterRoom(String[] tempids,String[] uniqueids,int[] powers,boolean isAll);
	public native int SomeLeaveRoom(String leaveuser);
	public native int RoomListResult(String[] roomsid,int[] roomstype,int[] count);
	public native int MicQueueResult(String[] tempid,String[] uniqueid);
	public native int MicTakeNotify(String tempid,String uniqueid);
	public native int EnterRoomNotify(String roomid,int roomtype);
	public native int NotitfyRandChat(String tempid,String uniqueid,String roomid);
    public native int RecordingStop(long size);
    public native int RecordingUploadEnd(String cloudvoiceurl,int size,long duration);
    public native int RecordingVoiceDownloadBegin();
    public native int RecordingVoiceDownloadEnd(boolean isOk,long size);
    public native int RecordingVoiceIncoming(String url);
    public native int NotifyPowerUpdate(String tempid,int power);
    public native int ResultAssignRet(int ret);
    public native int ResultUpdatePowerRet(int ret);


}
