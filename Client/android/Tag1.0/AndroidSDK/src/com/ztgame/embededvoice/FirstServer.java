package com.ztgame.embededvoice;

import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.net.URI;
import java.util.Arrays;
import java.util.List;
import java.util.Timer;
import java.util.TimerTask;

import org.apache.http.message.BasicNameValuePair;

import Cmd.Game.cmdNotifyLogicInfo;
import Cmd.Game.cmdRequestLogicInfo;
import Cmd.Game.cmdRequestLogin;
import Cmd.Game.cmdNotifyLogicInfo.enLogicResult;
import Cmd.Public.enCmdId;
import android.content.Context;
import android.util.Log;

import com.codebutler.android_websockets.WebSocketClient;


public class FirstServer 
{
   //private final String serveraddr = "ws://180.168.126.249:16008";
   private final String serveraddr = "ws://rtchat.ztgame.com:18000";
   private final String TAG = "FirstServerStatus";
   private	List<BasicNameValuePair> extraHeaders = Arrays.asList(
		    new BasicNameValuePair("Cookie", "session=abcd")
		);
   private FirstServerObserver mObserver;
   private Context mContext;
   
   private String appid;
   private String appkey;
   
   private boolean isGeted = false;
   private int reconCnt = 0;
   private final int maxreconCnt = 30;
   private boolean disConnectByUser = false;

   public FirstServer(Context ctx,FirstServerObserver observer,String Appid,String AppKey)
   {
	   setmObserver(observer);
	   setmContext(ctx);
	   
	   appid = Appid;
	   appkey = AppKey;
   }
   
   public boolean ConnectServer()
   {
		TimerTask task = new TimerTask(){

			@Override
			public void run() 
			{   
				firstWebSocket.connect();
			}
			
		};
		
		if(reconCnt > maxreconCnt)
		{
			return false;
		}
		else{
			int delay = 2000;
			if(reconCnt ==0)
				delay = 1;
			reconCnt++;
			Timer timer = new Timer(true);
			timer.schedule(task, delay);
		}	
	   return false;
   }
   
   public boolean DisConnect()
   {
	   disConnectByUser = true;
	   firstWebSocket.disconnect();
	   return false;
   }
   
   private WebSocketClient firstWebSocket = new WebSocketClient(URI.create(serveraddr),new WebSocketClient.Listener() {
	
		@Override
		public void onMessage(byte[] data) {
			Log.d(TAG, "first--onMessage--->[]" + data);
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
				
				if(iCmdid == enCmdId.enNotifyLogicInfo_VALUE)
				{
					cmdNotifyLogicInfo reqLogicResult = cmdNotifyLogicInfo.parseFrom(msgbytes);
					enLogicResult logicResult = reqLogicResult.getResult();
					if(logicResult == enLogicResult.LOGIC_RESULT_OK)
					{
						if(mObserver != null)
						{
							
							mObserver.OnNotifyLogicInfo(reqLogicResult.getToken(), reqLogicResult.getGateip(), reqLogicResult.getGateport());
						}
					}else
					{
						Log.e(TAG, "Req Logic fail "+ logicResult.ordinal());
					}
					
					DisConnect();
				}
				
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
		
		@Override
		public void onMessage(String message) {
			// TODO Auto-generated method stub
			Log.d(TAG, "first--onMessage--->" + message);
		}
		
		@Override
		public void onError(Exception error) {
			// TODO Auto-generated method stub
			Log.d(TAG, "first--onError--->" + error);
			if(disConnectByUser)
			{
				disConnectByUser = false;
				
			}else{
				ConnectServer();
			}
		}
		
		@Override
		public void onDisconnect(int code, String reason) {
			// TODO Auto-generated method stub
			Log.d(TAG, "first----onDisconnect--->" + reason);
			
		}
		
		@Override
		public void onConnect() 
		{
			Log.d(TAG, "first--onConnect--------------" + serveraddr);
			assert(appid != null && appkey !=null);
			reconCnt = 0;
			cmdRequestLogicInfo reqLogicmd = cmdRequestLogicInfo.newBuilder().setAppid(appid).setKey(appkey).build();
			byte[] data = reqLogicmd.toByteArray();
			ByteArrayOutputStream bout=new ByteArrayOutputStream();
			DataOutputStream dos=new DataOutputStream(bout);
			try {
			
				byte[] cmdid = ByteUtil.little_intToByte(enCmdId.enRequestLogicInfo_VALUE, 2);
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
			firstWebSocket.send(buf);
			
		}
	},extraHeaders);

	
	public void SetObserver(FirstServerObserver ob)
	{
		setmObserver(ob);
	}


	public FirstServerObserver getmObserver() {
		return mObserver;
	}


	public void setmObserver(FirstServerObserver mObserver) {
		this.mObserver = mObserver;
	}


	public Context getmContext() {
		return mContext;
	}


	public void setmContext(Context mContext) {
		this.mContext = mContext;
	}


	public WebSocketClient getFirstWebSocket() {
		return firstWebSocket;
	}


	public void setFirstWebSocket(WebSocketClient firstWebSocket) {
		this.firstWebSocket = firstWebSocket;
	}

}
