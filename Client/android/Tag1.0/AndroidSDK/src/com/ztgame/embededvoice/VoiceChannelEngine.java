package com.ztgame.embededvoice;

import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.net.URI;
import java.util.Arrays;
import java.util.Iterator;
import java.util.List;
import java.util.Timer;
import java.util.TimerTask;

import org.apache.http.message.BasicNameValuePair;
import org.webrtc.webrtcdemo.NativeWebRtcContextRegistry;

import com.codebutler.android_websockets.WebSocketClient;
import com.google.protobuf.InvalidProtocolBufferException;

import Cmd.Game.cmdNotifyAddVoiceUser;
import Cmd.Game.cmdNotifyAssignResult;
import Cmd.Game.cmdNotifyAssignResult.enAssignResult;
import Cmd.Game.cmdNotifyCreateResult;
import Cmd.Game.cmdNotifyDelVoiceUser;
import Cmd.Game.cmdNotifyEnterResult;
import Cmd.Game.cmdNotifyEnterResult.enEnterResult;
import Cmd.Game.cmdNotifyLoginResult;
import Cmd.Game.cmdNotifyRandChat;
import Cmd.Game.cmdNotifyRandPlay;
import Cmd.Game.cmdNotifyRoomList;
import Cmd.Game.cmdNotifySomeEnterRoom;
import Cmd.Game.cmdNotifySomeEnterRoom.enEnterReason;
import Cmd.Game.cmdNotifySomeLeaveRoom;
import Cmd.Game.cmdNotifyTakeMic;
import Cmd.Game.cmdNotifyUpdatePower;
import Cmd.Game.cmdNotifyUpdatePowerResult;
import Cmd.Game.cmdNotifyUpdatePowerResult.enUpdateResult;
import Cmd.Game.cmdRequestAssignMic;
import Cmd.Game.cmdRequestEnterRoom;
import Cmd.Game.cmdRequestLogin;
import Cmd.Game.cmdRequestRandPlay;
import Cmd.Game.cmdRequestUpdatePower;
import Cmd.Game.cmdReturnRandChat;
import Cmd.Public.enCmdId;
import Cmd.Public.enPowerType;
import Cmd.Public.enRoomType;
import Cmd.Public.stMicInfo;
import Cmd.Game.cmdNotifyMicQueue;
import Cmd.Game.cmdRequestCreateRoom;
import Cmd.Public.stRoomUserInfo;
import Cmd.Public.stVoiceUserInfo;
import android.app.Activity;
import android.os.Handler;
import android.os.Message;
import android.util.Log;

public class VoiceChannelEngine implements FirstServerObserver
{
	
	private enum enMsgType{
		
		MSG_VOICE_SIGNAL,
		MSG_USER,
		MSG_VOICE_ENGINE,
	}
	
	public enum enInterfaceType
	{	
		INTERFACE_JAVA,
		INTERFACE_NATIVE,
	}
	
	private final String TAG = "VoiceChannelEngine";
	private enInterfaceType iftype = enInterfaceType.INTERFACE_JAVA;
	private Activity mContext;
    private VoiceHandler mHandle;

    private final int reConnectMaxCnt  = 30;
    private int reConnectCnt  = 0;
    private final int maxHeartBitTime = 72;
	
    private NativeWebRtcContextRegistry contextRegistry = null;

    
    private static VoiceChannelEngine instance;
 
    
    
    private long myuserid = 0;
    private String theUniUserId;
    private String theAppid;
    private String theAppKey;
    
    private VoiceRoom theRoom;
    private VoiceRecord theRecordingEngine;
    
    private boolean DisposeByUser = false;
    private FirstServer firstServer;
    
    private enum LoginState
	{	
		UNConnected,
		GetingLogcicInfo,
		Connecting,
		ReConnecting,
		Connected,
		Logining,
		Logined
	}
	
    private LoginState LoginStatus = LoginState.UNConnected;
    
    public cmdNotifyRoomList currentrRoomlist;
    
    private Timer heartbitTimer =  new Timer(true);
    private TimerTask heatBitMisstask = new TimerTask()
	{

		@Override
		public void run() 
		{
			activateSDK();
		}
		
	};
	
	private	List<BasicNameValuePair> extraHeaders = Arrays.asList(
			    new BasicNameValuePair("Cookie", "session=abcd")
			);

	private WebSocketClient signalChannelSocket;
	
    
    public static VoiceChannelEngine Create(Activity context,enInterfaceType type)
    {
    	if(instance == null)
    	{
    		instance = new VoiceChannelEngine(context);
    		instance.iftype = type;
    		if(instance.iftype == enInterfaceType.INTERFACE_NATIVE)
    		{
    			JNIObserver jniobserver = new JNIObserver();
    			instance.setObserver(jniobserver);
    			instance.initNativeSDK(instance);
    		}
    	}
    	return instance;
    }
    
    
    public static VoiceChannelEngine GetIntance()
    {
    	return instance;
    }
    
    
    
    public VoiceChannelEngine(Activity context) {
		super();
		this.mContext = context;
		contextRegistry = new NativeWebRtcContextRegistry();
		contextRegistry.register(context);
		mHandle = new VoiceHandler();
	}
    
    public VoiceChannelEngineObserver observer;
    public void setObserver(VoiceChannelEngineObserver observer) 
    {
        this.observer = observer;
    }
    
    	
	private void processMsg(int msgid,byte[] msgData)
	{
		Log.d(TAG, "onMessage--[]- msgid = >" + msgid);
		try
		{
			switch(msgid)
			{
				case enCmdId.enNotifyLoginResult_VALUE:
					cmdNotifyLoginResult loginResult = cmdNotifyLoginResult.parseFrom(msgData);
					myuserid = loginResult.getTempid();
					Log.d(TAG, "My voice userid = " + myuserid);
					if(loginResult.getResult() == cmdNotifyLoginResult.enLoginResult.LOGIN_RESULT_OK)
					{
						LoginStatus = LoginState.Logined;
						if(observer != null)
						{
							observer.OnLoginResult(loginResult);
						}
						ReqRoomList();
					}
				break;
				
				case enCmdId.enNotifyRoomList_VALUE:
					cmdNotifyRoomList roomlistMsg = cmdNotifyRoomList.parseFrom(msgData);
					currentrRoomlist = roomlistMsg;
					if(observer != null)
					{
						observer.OnRoomListNotify(roomlistMsg.getInfoList());
					}
					
				break;
				
				case enCmdId.enNotifyCreateResult_VALUE:
				{
					cmdNotifyCreateResult createRoomResult = cmdNotifyCreateResult.parseFrom(msgData);
					boolean isOK = createRoomResult.getIsok();
					if(isOK)
					{
						long roomid = createRoomResult.getRoomid();
						String roomIp = createRoomResult.getIp();
						int port = createRoomResult.getPort();
						enRoomType roomtype = createRoomResult.getType();
						if(theRoom == null)
						{
							theRoom = VoiceRoom.CreateRoom(roomtype,myuserid,mContext);
						}
						
						theRoom.SetupMainVoiceChannel(roomIp, 10086,port);
						theRoom.SetRoomId(roomid);
						
						if(observer != null)
						{  
							observer.OnEnterRoomNotify(roomid,roomtype);
						}
								
					}else{
						
						theRoom.Dispose();
						theRoom = null;
						if(observer != null)
						{  
							//observer.OnEnterRoomNotify(roomid,roomtype);
						}
					}
					
					
				}
				break;
				case enCmdId.enNotifyEnterResult_VALUE:
					cmdNotifyEnterResult enterRoomResult = cmdNotifyEnterResult.parseFrom(msgData);
					enEnterResult result  = enterRoomResult.getResult();
					if(result == enEnterResult.ENTER_RESULT_OK)
					{
						long roomid = enterRoomResult.getRoomid();
						String roomIp = enterRoomResult.getIp();
						int port = enterRoomResult.getPort();
						enRoomType roomtype = enterRoomResult.getType();
						if(theRoom == null)
						{
							theRoom = VoiceRoom.CreateRoom(roomtype,myuserid,mContext);
						}
						
						theRoom.SetupMainVoiceChannel(roomIp, 10086,port);
						theRoom.SetRoomId(roomid);
						
						if(observer != null)
						{   
							observer.OnEnterRoomNotify(roomid,roomtype);
						}
								
					}else{
						
						
					}
					
				break;
				
				case enCmdId.enNotifyAddVoiceUser_VALUE:
					cmdNotifyAddVoiceUser  newvoiceUser = cmdNotifyAddVoiceUser.parseFrom(msgData);
					List<stVoiceUserInfo> userinfoList = newvoiceUser.getInfoList();
					for (Iterator<stVoiceUserInfo> i = userinfoList.iterator(); i.hasNext();)
					{  
						stVoiceUserInfo userinfoRef = i.next(); 
						if(theRoom != null)
						{
							theRoom.AddFreeRoomUser(userinfoRef.getId());
						}
					}  
					if(observer != null)
					{
						observer.OnAddVoiceUser(userinfoList);
					}
				break;
				case enCmdId.enNotifyDelVoiceUser_VALUE:
					cmdNotifyDelVoiceUser delnotify = cmdNotifyDelVoiceUser.parseFrom(msgData);
					{
						List<stVoiceUserInfo> delInfoList = delnotify.getInfoList();
						for (Iterator<stVoiceUserInfo> i = delInfoList.iterator(); i.hasNext();)
						{  
							stVoiceUserInfo userinfoRef = i.next(); 
							if(theRoom != null)
							{
								theRoom.DelFreeRoomUser(userinfoRef.getId());
							
							}
						}  
						
						if(observer != null)
						{
							observer.OnDelVoiceUser(delInfoList);
						}
					}
					break;
				case enCmdId.enNotifyMicQueue_VALUE:
					cmdNotifyMicQueue micqueue = cmdNotifyMicQueue.parseFrom(msgData);
					List<stMicInfo> micInfoList = micqueue.getInfoList();
					if(theRoom != null)
					{
						theRoom.UpdateMicQueue(micInfoList);
						if(observer != null)
						{
							observer.OnMicQueueNotify(micInfoList);
						}
					}
				break;
				case enCmdId.enNotifyTakeMic_VALUE:
					cmdNotifyTakeMic micinfo = cmdNotifyTakeMic.parseFrom(msgData);
					if(theRoom != null)
					{
						
						theRoom.UpateCurrentMic(micinfo);
						if(observer != null)
						{
							observer.OnMicTakeNotify(micinfo);
						}
					}
				break;
				case enCmdId.enNotifySomeEnterRoom_VALUE:
					cmdNotifySomeEnterRoom enterRoomCmd = cmdNotifySomeEnterRoom.parseFrom(msgData);
					if(theRoom != null)
					{
						
						List<stRoomUserInfo> roomUserList = enterRoomCmd.getInfoList();
						if(observer != null)
						{   boolean isNewAll = false;
						    if(enterRoomCmd.getReason() == enEnterReason.ENTER_REASON_NEW)
						    	 isNewAll = true;
							observer.OnRoomUsersEnter(roomUserList,isNewAll);
						}
					}
				break;
				case enCmdId.enNotifySomeLeaveRoom_VALUE:
					cmdNotifySomeLeaveRoom leaveRoomCmd = cmdNotifySomeLeaveRoom.parseFrom(msgData);
					if(theRoom != null)
					{
						
						long leaveroomUser = leaveRoomCmd.getTempid();
						if(observer != null)
						{
							observer.OnRoomUserLeave(leaveroomUser);
						}
					}
				break;
				case enCmdId.enNotifyRandChat_VALUE:
					
					cmdNotifyRandChat randChatInviteCmd = cmdNotifyRandChat.parseFrom(msgData);
					long peerTempid = randChatInviteCmd.getTempid();
					long randRoomid = randChatInviteCmd.getRoomid();
					String peerUniqueId = randChatInviteCmd.getUniqueid();
					if(observer != null)
					{
						observer.OnRandChatInvite(peerTempid,randRoomid,peerUniqueId);
					}
					
				break;
				case enCmdId.enNotifyRandPlay_VALUE:
					cmdNotifyRandPlay recordingIncomingCmd = cmdNotifyRandPlay.parseFrom(msgData);
					String url = recordingIncomingCmd.getUrl();
					if(observer != null)
					{
						observer.OnRandomRecording(url);
					}
				break;
				case enCmdId.enNotifyAssignResult_VALUE:
				    cmdNotifyAssignResult powerAssignedCmd = cmdNotifyAssignResult.parseFrom(msgData);
				    enAssignResult assignedRet = powerAssignedCmd.getResult();
					if(observer != null)
					{
						observer.OnNotifyAssignRet(assignedRet.ordinal());
					}
				break;
				case enCmdId.enNotifyUpdatePowerResult_VALUE:
					cmdNotifyUpdatePowerResult upPowerRetCmd = cmdNotifyUpdatePowerResult.parseFrom(msgData);
					enUpdateResult upRet =  upPowerRetCmd.getResult();
					if(observer != null)
					{
						observer.OnResultUpPwder(upRet.ordinal());
					}
				break;
				case enCmdId.enNotifyUpdatePower_VALUE:
					cmdNotifyUpdatePower updatePowerCmd = cmdNotifyUpdatePower.parseFrom(msgData);
					long updateTempid = updatePowerCmd.getTempid();
					enPowerType power =  updatePowerCmd.getPower();
					if(observer != null)
					{
						observer.OnNotifyUpdatePower(updateTempid,power.ordinal());
					}
				break;
				default:
			    break;
			}
			
		}catch (InvalidProtocolBufferException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
	}
	
	private boolean sendData(byte[] msgbuf)
	{
		Log.d(TAG, "sendData------LoginStatus = " + LoginStatus);
		if(signalChannelSocket!=null && LoginStatus == LoginState.Logined)
		{
			signalChannelSocket.send(msgbuf);
			return true;
		}else if(signalChannelSocket!=null && LoginStatus == LoginState.UNConnected)
		{
		   activateSDK();
		   return false;
		}else{
			
		  return false;
		}
	}
	
	private boolean sendMsg(int msgid,byte[] msgData)
	{
		ByteArrayOutputStream bout=new ByteArrayOutputStream();
		DataOutputStream dos=new DataOutputStream(bout);
		try {
		
			byte[] cmdid = ByteUtil.little_intToByte(msgid, 2);
			dos.write(cmdid);
			byte[] length = ByteUtil.little_intToByte(msgData.length, 4);
			dos.write(length);
			dos.write(msgData);
			dos.close();
			bout.close();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
		byte[] buf=bout.toByteArray();//获取内存缓冲区中的数据
		sendData(buf);
		return true;
	}
	
	private boolean sendMsg(int msgid)
	{
		ByteArrayOutputStream bout=new ByteArrayOutputStream();
		DataOutputStream dos=new DataOutputStream(bout);
		try {
		
			byte[] cmdid = ByteUtil.little_intToByte(msgid, 2);
			dos.write(cmdid);
			byte[] length = ByteUtil.little_intToByte(0, 4);
			dos.write(length);
			dos.close();
			bout.close();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
		byte[] buf=bout.toByteArray();//获取内存缓冲区中的数据
		sendData(buf);
		return true;
	}
	
	 public void Init(String appid,String appkey,String UniUserId)
	 {
	    	theUniUserId = UniUserId;
	    	theAppid = appid;
	    	theAppKey = appkey;
	    	firstServer = new FirstServer(mContext, this,appid, appkey);
	    	firstServer.ConnectServer();
	 }
	
	   private class VoiceHandler extends Handler
	    {
	    	  @Override 
	          public void handleMessage(Message msg) { 
	              // TODO Auto-generated method stub 
	              super.handleMessage(msg); 
	              switch(msg.what) 
	              { 
	              	  case 1://在收到消息时，对界面进行更新 
	              	  {
	              		  int cmdid = msg.arg1;
	              		  byte[] data = (byte[])msg.obj;
	              		  processMsg(cmdid, data);
	              	  }
	                  break;
	              	  case 2://用户请求
	              	  {
	              		  
	              	  }
	              	  break;
	              	  case 3://webrtc 引擎通知
	              	  {
	              		  
	              	  }
	              	  break;
	              	  case 4://心跳通知
	             	  {
	             		heartbitTimer.cancel();
	      				//启动心跳检测timer
	             		heartbitTimer.purge();
	             		heartbitTimer = null;
	             		heartbitTimer = new Timer(true); 
	      				TimerTask task = new TimerTask()
	      				{

	      					@Override
	      					public void run() 
	      					{
	      						reActivateSDK();
	      					}
	      					
	      				};
	      				heartbitTimer.schedule(task, maxHeartBitTime *1000);
	             	  }
	             	  break;
	              	  default:
	              	  break;
	              } 
	          } 
	    }
	    
	
	public boolean reActivateSDK()
	{
		if(LoginStatus == LoginState.ReConnecting)
			  return false;
		else
			LoginStatus = LoginState.ReConnecting;
		
		
		if(signalChannelSocket != null)
		{
			signalChannelSocket.disconnect();
		}
		
		TimerTask task = new TimerTask(){

			@Override
			public void run() 
			{   
				Log.d(TAG, "--------------reActivateSDK");
				activateSDK();
			}
			
		};
		
		if(reConnectCnt > reConnectMaxCnt)
		{
			return false;
		}
		else{
			int delay = 1000;
			if(reConnectCnt > 15)
				delay = 2000;
			reConnectCnt++;
			Timer timer = new Timer(true);
			timer.schedule(task, delay);
		}	
	
		
		return true;
	}
	
	public boolean activateSDK()
	{
			
		DisposeByUser = false;
    	if(signalChannelSocket != null)
    	{
    		LoginStatus = LoginState.Connecting;
    		signalChannelSocket.connect();
    	}
    	
		return true;
	}
	
	public boolean deActivateSDK()
	{
		DisposeVoiceEngine();
		return true;
	}
	
	public boolean loginServer(String uniuserID)
	{
		if(uniuserID != null)
			theUniUserId = uniuserID;
		if(theUniUserId == null || signalChannelSocket == null || LoginStatus != LoginState.Connected)
			 return false;
		
		cmdRequestLogin logincmd = cmdRequestLogin.newBuilder().setUniqueid(theUniUserId).setToken("1111").setTempid(myuserid).build();
		byte[] data = logincmd.toByteArray();
		//sendMsg(enCmdId.enRequestLogin_VALUE,data);
		
		ByteArrayOutputStream bout=new ByteArrayOutputStream();
		DataOutputStream dos=new DataOutputStream(bout);
		try {
		
			byte[] cmdid = ByteUtil.little_intToByte(enCmdId.enRequestLogin_VALUE, 2);
			dos.write(cmdid);
			byte[] length = ByteUtil.little_intToByte(data.length, 4);
			dos.write(length);
			dos.write(data);
			dos.close();
			bout.close();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
		byte[] buf=bout.toByteArray();//获取内存缓冲区中的数据
		
		if(signalChannelSocket!=null && LoginStatus == LoginState.Connected)
		{
			signalChannelSocket.send(buf);
			return true;
		}
		LoginStatus = LoginState.Logining;
		return false;
	}
	
	
	public boolean ReqRoomList()
	{
		sendMsg(enCmdId.enRequestRoomList_VALUE);
		return true;
	}
	
	public boolean ReqCreateRoom(Cmd.Public.enRoomType type,Cmd.Public.enRoomReason roomreson)
	{
		if(theRoom != null ||(theRoom != null && theRoom.State == VoiceRoom.RoomState.creating))
			return false;
		
		theRoom = VoiceRoom.CreateRoom(type,myuserid,mContext);
		cmdRequestCreateRoom createrRoomcmd = cmdRequestCreateRoom.newBuilder().setType(type).setReason(roomreson).build();
		byte[] data = createrRoomcmd.toByteArray();
		sendMsg(enCmdId.enRequestCreateRoom_VALUE,data);
		return true;
	}
	
	public boolean ReqCreateRoomByNative(int type,int roomreason)
	{
		if(theRoom != null ||(theRoom != null && theRoom.State == VoiceRoom.RoomState.creating))
			return false;
		
		
		Cmd.Public.enRoomType roomtype = Cmd.Public.enRoomType.valueOf(type);
		theRoom = VoiceRoom.CreateRoom(roomtype,myuserid,mContext);
		Cmd.Public.enRoomReason reason = Cmd.Public.enRoomReason.valueOf(roomreason);
		cmdRequestCreateRoom createrRoomcmd = cmdRequestCreateRoom.newBuilder().setType(roomtype).setReason(reason).build();
		byte[] data = createrRoomcmd.toByteArray();
		sendMsg(enCmdId.enRequestCreateRoom_VALUE,data);
		return true;
	}
	
	public boolean ReqEnterRoom(long roomid)
	{
		if(theRoom != null || (theRoom != null && theRoom.State == VoiceRoom.RoomState.creating))
			return false;
		
		theRoom = VoiceRoom.CreateRoom(Cmd.Public.enRoomType.ROOM_TYPE_FREE,myuserid,mContext);
		cmdRequestEnterRoom enterRoomCmd = cmdRequestEnterRoom.newBuilder().setRoomid(roomid).build();
		byte[] data = enterRoomCmd.toByteArray();
		sendMsg(enCmdId.enRequestEnterRoom_VALUE,data);
		return true;
	}
	
	public boolean ReqLeaveRoom(long roomid)
	{
		if(theRoom == null)
			return false;
		sendMsg(enCmdId.enRequestLeaveRoom_VALUE);
		disposeCurrentVoiceChannel();
		return true;
	}
	
	public boolean ReqMicToken()
	{   
		if(theRoom == null || (theRoom != null && theRoom.State != VoiceRoom.RoomState.created))
			return false;
		sendMsg(enCmdId.enRequestJoinMicQueue_VALUE);
		return true;
	}
	
	public boolean ReqLeaveMicToken()
	{
		if(theRoom == null ||  (theRoom != null && theRoom.State != VoiceRoom.RoomState.created))
			return false;
		sendMsg(enCmdId.enRequestLeaveMicQueue_VALUE);
		return true;
	}
	
	public boolean ReqAcceptRandChat(boolean isAccecpt,long peerid)
	{
		if(theRoom != null)
			return false;
		cmdReturnRandChat replycmd = cmdReturnRandChat.newBuilder().setIsok(isAccecpt).setTempid(peerid).build();
		byte[] data = replycmd.toByteArray();
		sendMsg(enCmdId.enReturnRandChat_VALUE,data);
		return true;
	}
	
	public boolean ReqMuteSelf(boolean isMute)
	{
		if(theRoom == null ||  (theRoom != null && theRoom.State != VoiceRoom.RoomState.created))
			return false;
		Log.d(TAG, "c  ReqMuteSelf-----" + isMute);
		theRoom.MuteMic(isMute);     
		return true;
	}
	
	public boolean DisposeVoiceEngine()
	{  
		DisposeByUser = true;
		if(signalChannelSocket != null && signalChannelSocket.isConnected())
		{
			signalChannelSocket.disconnect();
		}
		LoginStatus = LoginState.UNConnected;
		disposeCurrentVoiceChannel();
		return true;
	}
	
	public boolean disposeCurrentVoiceChannel()
	{
		if(theRoom != null)
		{
			theRoom.Dispose();
			theRoom = null;
		}
		return true;
	}
	
	public boolean ReqStartRecording()
	{
		if(theRecordingEngine  == null)
		{
			theRecordingEngine = new VoiceRecord(mContext,this);
		}
		
		theRecordingEngine.StartRecording();
		return true;
	}
	
	public boolean ReqStopRecording()
	{
		if(theRecordingEngine  == null)
		{
			return false;
		}
		
		theRecordingEngine.StopRecording();
		return true;
	}
	
	public boolean ReqStartPlayVoice(String voiceid)
	{
		if(theRecordingEngine  == null)
		{
			theRecordingEngine = new VoiceRecord(mContext,this);
		}
		
		theRecordingEngine.StartPlayVoice(voiceid);
		return true;
	}
	
	public boolean ReqStopPlayVoice()
	{
		theRecordingEngine.StopPlayVoice("");
		return true;
	}
	
	public boolean ReqSendRandomRecording(String cloundfileurl)
	{
		cmdRequestRandPlay randomRcoringCmd = cmdRequestRandPlay.newBuilder().setUrl(cloundfileurl).build();
		byte[] data = randomRcoringCmd.toByteArray();
		sendMsg(enCmdId.enRequestRandPlay_VALUE,data);
		return true;
	}
	
	public boolean ReqUpdatePower(long tempid,int power)
	{
		if(theRoom == null)
			return false;
		
		enPowerType epower = enPowerType.valueOf(power);
		cmdRequestUpdatePower reqUpPwder = cmdRequestUpdatePower.newBuilder().setTempid(tempid).setPower(epower).build();
		byte[] data = reqUpPwder.toByteArray();
		sendMsg(enCmdId.enRequestUpdatePower_VALUE,data);
		return true;
	}
	
	public boolean ReqAssignMic(long tempid)
	{
		if(theRoom == null)
			return false;
		
		cmdRequestAssignMic reqAssignMic = cmdRequestAssignMic.newBuilder().setTempid(tempid).build();
		byte[] data = reqAssignMic.toByteArray();
		sendMsg(enCmdId.enRequestAssignMic_VALUE,data);
		return true;
	}
	
	
	public native void initNativeSDK(VoiceChannelEngine vce);


	@Override
	public void OnNotifyLogicInfo(String token, final String gateip, int gateport) 
	{
		Log.d(TAG, "OnNotifyLogicInfo--------" + "ip:"+ gateip + ":" + gateport);
		URI wsuri = URI.create("ws://"+ gateip + ":" + gateport);
		signalChannelSocket = new WebSocketClient(wsuri,new WebSocketClient.Listener() {
			
			@Override
			public void onMessage(byte[] data) {
				// TODO Auto-generated method stub
				if(data.length == 2)
				{
					signalChannelSocket.send(data);
					//123Log.d(TAG, "send heart bit--------" + signalChannelSocket.toString());
					Message hearbitsignal = Message.obtain(mHandle,4);
					mHandle.sendMessage(hearbitsignal);
					return;
				}
				Log.d(TAG, "onMessage--->------>" + data);
				ByteArrayInputStream binput = new ByteArrayInputStream(data);
				DataInputStream  dios = new DataInputStream(binput);
				byte[] cmdid= new byte[2];
				byte[] cmdlength=new byte[4];
				byte[] msgbytes=null;
				try {
					dios.read(cmdid, 0, 2);
					dios.read(cmdlength, 0, 4);
					
					int iCmdid = ByteUtil.little_bytesToInt(cmdid);
					int iCmdLength = ByteUtil.little_bytesToInt(cmdlength);
					msgbytes = new byte[iCmdLength];
					dios.read(msgbytes);
					
					//processMsg(iCmdid, msgbytes);
					
					Message voicesignal = Message.obtain(mHandle,1);
					voicesignal.arg1 = iCmdid;
					voicesignal.obj = msgbytes;
					mHandle.sendMessage(voicesignal);
					
					
				} catch (IOException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
				
			}
			
			@Override
			public void onMessage(String message) {
				// TODO Auto-generated method stub
				Log.d(TAG, "onMessage--->" + message);
			}
			
			@Override
			public void onError(Exception error) {
				// TODO Auto-generated method stub
				Log.d(TAG, "onError--->" + error);
				//activateSDK();
				if(signalChannelSocket!=null)
				{
					signalChannelSocket.disconnect();
				}
				reActivateSDK();
			}
			
			@Override
			public void onDisconnect(int code, String reason) {
				// TODO Auto-generated method stub
				Log.d(TAG, "onDisconnect--->" + reason);
				if(!DisposeByUser)
				{
					//非用户关闭则自动重连
					if(signalChannelSocket!=null)
					{
						signalChannelSocket.disconnect();
					}
					
				}
			}
			
			@Override
			public void onConnect() 
			{
				// TODO Auto-generated method stub
				Log.d(TAG, "onConnect--------------gateip = " + gateip);
				LoginStatus = LoginState.Connected;
				reConnectCnt = 0;
				loginServer(null);
				//启动心跳检测timer,方式第一次心跳收不到
				TimerTask task = new TimerTask()
				{

					@Override
					public void run() 
					{
						reActivateSDK();
					}
					
				};
				heartbitTimer.schedule(task, maxHeartBitTime *1000);
							
			}
		},extraHeaders);
		
		activateSDK();
		
	}
	
}
