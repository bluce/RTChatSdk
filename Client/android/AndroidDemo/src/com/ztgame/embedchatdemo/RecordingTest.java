package com.ztgame.embedchatdemo;

import java.util.List;

import com.ztgame.embededvoice.VoiceChannelEngine;
import com.ztgame.embededvoice.VoiceChannelEngineObserver;

import Cmd.Game.cmdNotifyCreateResult;
import Cmd.Game.cmdNotifyLoginResult;
import Cmd.Game.cmdNotifyTakeMic;
import Cmd.Public.enRoomType;
import Cmd.Public.stMicInfo;
import Cmd.Public.stRoomInfo;
import Cmd.Public.stRoomUserInfo;
import Cmd.Public.stVoiceUserInfo;
import android.app.Activity;
import android.os.Bundle;
import android.view.MotionEvent;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.View.OnTouchListener;
import android.widget.Button;
import android.widget.TextView;

public class RecordingTest extends Activity implements VoiceChannelEngineObserver
{
	public  VoiceChannelEngine ztvoe = null;
	private String cloudFileUrl = null; 
	private TextView cloudfileUrlTv = null;
	private Button playtn = null;
	
	protected void onCreate(Bundle savedInstanceState) 
	{
		super.onCreate(savedInstanceState);
		setContentView(R.layout.recordtest);
		ztvoe = VoiceChannelEngine.GetIntance();
		cloudfileUrlTv = (TextView)findViewById(R.id.urltv);
		  Button startrecording = (Button)findViewById(R.id.startrecording);
		  startrecording.setOnTouchListener(new OnTouchListener() {
				
				@Override
				public boolean onTouch(View arg0, MotionEvent arg1) 
				{
					if(arg1.getAction() == MotionEvent.ACTION_DOWN)
					{
						ztvoe.ReqStartRecording();
					}else if(arg1.getAction() == MotionEvent.ACTION_UP)
					{
						ztvoe.ReqStopRecording();
					}
					return false;
				}
			});
	        
		  playtn = (Button)findViewById(R.id.playrecording);
		  playtn.setText("²¥·ÅÂ¼Òô");
		  playtn.setOnClickListener(new OnClickListener(){

				@Override
				public void onClick(View arg0) {
					// TODO Auto-generated method stub
					//ztvoe.ReqLeaveMicToken();
					ztvoe.ReqStartPlayVoice(cloudFileUrl);
					//ztvoe.ReqStopPlayVoice();
				}
	        	
	        });
	        
		  playtn.setEnabled(false);
    }
	
	

	@Override
	protected void onResume() {
		// TODO Auto-generated method stub
		super.onResume();
		ztvoe.setObserver(this);
	}



	@Override
	public void OnLoginResult(cmdNotifyLoginResult loginresult) {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void OnCreateResult(cmdNotifyCreateResult createresult) {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void OnEnterRoomNotify(long roomid, enRoomType roomtype) {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void OnAddVoiceUser(List<stVoiceUserInfo> voiceuser) {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void OnDelVoiceUser(List<stVoiceUserInfo> voiceuser) {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void OnRoomListNotify(List<stRoomInfo> roomlist) {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void OnMicQueueNotify(List<stMicInfo> micinfolists) {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void OnMicTakeNotify(cmdNotifyTakeMic micinfo) {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void OnRoomUsersEnter(List<stRoomUserInfo> roomUsersList,
			boolean isAll) {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void OnRoomUserLeave(long leaveRoomUser) {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void OnRandChatInvite(long tempid, long roomid, String uniqueid) {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void OnStopVoiceRecording(long size) {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void OnUploadVoiceEnd(String cloudvoiceurl) {
		// TODO Auto-generated method stub
//		cloudFileUrl = cloudvoiceurl;
//		runOnUiThread(new Runnable() {
//			
//			@Override
//			public void run() {
//				// TODO Auto-generated method stub
//				cloudfileUrlTv.setText(cloudFileUrl);
//				playtn.setEnabled(true);
//			}
//		});
		
	}

	



	@Override
	public void OnDlVoiceBegin() {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void OnDlVoiceEnd(boolean isOk, long size) {
		// TODO Auto-generated method stub
		
	}



	@Override
	public void OnRandomRecording(String url) {
		// TODO Auto-generated method stub
		cloudFileUrl = url;
		runOnUiThread(new Runnable() {
			
			@Override
			public void run() {
				// TODO Auto-generated method stub
				cloudfileUrlTv.setText(cloudFileUrl);
				playtn.setEnabled(true);
			}
		});
	}



	@Override
	public void OnNotifyUpdatePower(long tempid, int power) {
		// TODO Auto-generated method stub
		
	}



	@Override
	public void OnNotifyAssignRet(int ret) {
		// TODO Auto-generated method stub
		
	}
	
	public void OnResultUpPwder(int result) {
		// TODO Auto-generated method stub
		
	}
}
