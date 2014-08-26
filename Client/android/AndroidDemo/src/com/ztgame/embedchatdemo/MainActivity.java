package com.ztgame.embedchatdemo;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Timer;
import java.util.TimerTask;

import com.ztgame.embededvoice.VoiceChannelEngine;
import com.ztgame.embededvoice.VoiceChannelEngineObserver;

import Cmd.Game.cmdNotifyCreateResult;
import Cmd.Game.cmdNotifyLoginResult;
import Cmd.Game.cmdNotifyRoomList;
import Cmd.Game.cmdNotifyTakeMic;
import Cmd.Public.enRoomReason;
import Cmd.Public.enRoomType;
import Cmd.Public.stMicInfo;
import Cmd.Public.stRoomInfo;
import Cmd.Public.stRoomUserInfo;
import Cmd.Public.stVoiceUserInfo;
import android.net.wifi.WifiInfo;
import android.net.wifi.WifiManager;
import android.os.Bundle;
import android.app.Activity;
import android.content.Context;
import android.view.Menu;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.AdapterView;
import android.widget.AdapterView.OnItemClickListener;
import android.widget.Button;
import android.widget.LinearLayout;
import android.widget.ListView;
import android.widget.SimpleAdapter;
import android.widget.TextView;




public class MainActivity extends Activity implements VoiceChannelEngineObserver {

	private ArrayList<HashMap<String, String>> roomlist = new ArrayList<HashMap<String, String>>();
	private ArrayList<stRoomInfo> roomInfoList = new ArrayList<stRoomInfo>();
	public  SimpleAdapter listItemAdapter;
	
	public ListView roomListView;
	private  VoiceChannelEngine ztvoe = null;
	
	private TextView theRoomTv;
	private TextView whoSpTv;
	private LinearLayout micArea;
	
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        ztvoe =  VoiceChannelEngine.Create(this, VoiceChannelEngine.enInterfaceType.INTERFACE_JAVA);
        ztvoe.setObserver(this);
        WifiManager wifi = (WifiManager)getSystemService(Context.WIFI_SERVICE);
		WifiInfo info = wifi.getConnectionInfo();
	    ztvoe.Init("test", "test2", info.getMacAddress());
        
        
        roomListView =  (ListView)findViewById(R.id.listView1);
        listItemAdapter = new SimpleAdapter(this,roomlist,//����Դ 
	            R.layout.listitem,//ListItem��XMLʵ��
	            //��̬������ImageItem��Ӧ������        
	            new String[] {"roomidtext", "roomtypetext"}, 
	            //ImageItem��XML�ļ������һ��ImageView,����TextView ID
	            new int[] {R.id.roomid,R.id.roomtype}
	        );
        
        
        roomListView.setAdapter(listItemAdapter);
        roomListView.setOnItemClickListener(new OnItemClickListener(){

			@Override
			public void onItemClick(AdapterView<?> arg0, View arg1, int arg2,
					long arg3) {
				
				stRoomInfo roomitem = roomInfoList.get(arg2);
				ztvoe.ReqEnterRoom(roomitem.getRoomid());
				
			}
        	
        });
        
        Button createFreeBtn = (Button)findViewById(R.id.Btncreatefreeroom);
        createFreeBtn.setOnClickListener(new OnClickListener(){

			@Override
			public void onClick(View arg0) {
				// TODO Auto-generated method stub
				VoiceChannelEngine.GetIntance().ReqCreateRoom(enRoomType.ROOM_TYPE_FREE,enRoomReason.ROOM_REASON_NORMAL);
			}
        	
        });
        
        Button createQueueBtn = (Button)findViewById(R.id.Btncreatequeueroom);
        createQueueBtn.setOnClickListener(new OnClickListener(){

			@Override
			public void onClick(View arg0) {
				// TODO Auto-generated method stub
				VoiceChannelEngine.GetIntance().ReqCreateRoom(enRoomType.ROOM_TYPE_QUEUE,enRoomReason.ROOM_REASON_NORMAL);
			}
        	
        });
        
        Button takemic = (Button)findViewById(R.id.takeMic);
        takemic.setOnClickListener(new OnClickListener(){

			@Override
			public void onClick(View arg0) {
				// TODO Auto-generated method stub
				ztvoe.ReqMicToken();
			}
        	
        });
        
        Button leaveMic = (Button)findViewById(R.id.leaveMic);
        leaveMic.setOnClickListener(new OnClickListener(){

			@Override
			public void onClick(View arg0) {
				// TODO Auto-generated method stub
				ztvoe.ReqLeaveMicToken();
			}
        	
        });
        
        Button reqList = (Button)findViewById(R.id.reqroomlist);
        reqList.setOnClickListener(new OnClickListener(){

			@Override
			public void onClick(View arg0) {
				// TODO Auto-generated method stub
				ztvoe.ReqRoomList();
			}
        	
        });
        
        Button leaveroom = (Button)findViewById(R.id.leaveroom);
        leaveroom.setOnClickListener(new OnClickListener(){

			@Override
			public void onClick(View arg0) {
				// TODO Auto-generated method stub
				ztvoe.ReqLeaveRoom(0);
			}
        	
        });
        
       
        theRoomTv = (TextView)findViewById(R.id.theroomnumb);
        whoSpTv = (TextView)findViewById(R.id.whospeaking);
        
        micArea = (LinearLayout)findViewById(R.id.micCtrArea);
        micArea.setVisibility(View.INVISIBLE);
        
        /*
        Timer timer = new Timer(true);
        timer.schedule(new TimerTask(){

			@Override
			public void run() {
				
				ztvoe.ReqRoomList();
			}}, 1000*2, 1000*5);
			*/
    }


    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.main, menu);
        return true;
    }


	@Override
	public void finish() {
		// TODO Auto-generated method stub
		super.finish();
		VoiceChannelEngine.GetIntance().DisposeVoiceEngine();
	}


	@Override
	public void OnRoomListNotify(final List<stRoomInfo> roomlistref) {
		// TODO Auto-generated method stub
	
		
		
		this.runOnUiThread(new Runnable(){

			@Override
			public void run() {
				
				roomlist.clear();
				roomInfoList.clear();
				// new String[] {"enterbtn","roomidtext", "roomtypetext"}, 
				
				for (Iterator<stRoomInfo> i = roomlistref.iterator(); i.hasNext();)
				{  
					HashMap<String, String> roominfo = new HashMap<String, String>();
					stRoomInfo romminfoRef = i.next(); 
					String roomid = String.valueOf(romminfoRef.getRoomid());
					String roomType = "����ģʽ����";
					if(romminfoRef.getRoomtype() == enRoomType.ROOM_TYPE_QUEUE)
					{
						roomType = "����ģʽ����";
					}
					
					roominfo.put("roomidtext",roomid);
					roominfo.put("roomtypetext",roomType);
					roomInfoList.add(romminfoRef);
					roomlist.add(roominfo);
				}
				
				listItemAdapter.notifyDataSetChanged();
			}});
			
	}


	@Override
	public void OnMicQueueNotify(List<stMicInfo> micinfolists) {
		// TODO Auto-generated method stub
		
	}


	@Override
	public void OnMicTakeNotify(final cmdNotifyTakeMic micinfo) {
		// TODO Auto-generated method stub
		this.runOnUiThread(new Runnable(){

			@Override
			public void run() {
				// TODO Auto-generated method stub  
				whoSpTv.setText(micinfo.getTempid() + "����˵��");
			}});
	}
	
	public void OnEnterRoomNotify(final long roomid,final enRoomType roomtype) {
		// TODO Auto-generated method stub
		this.runOnUiThread(new Runnable(){

			@Override
			public void run() {
				// TODO Auto-generated method stub
				theRoomTv.setText("���ڷ���" + roomid);
				if(roomtype == enRoomType.ROOM_TYPE_QUEUE)
				{
					micArea.setVisibility(View.VISIBLE);
				}else{
					
					micArea.setVisibility(View.INVISIBLE);
				}
			}});
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
	public void OnAddVoiceUser(List<stVoiceUserInfo> voiceuser) {
		// TODO Auto-generated method stub
		
	}


	@Override
	public void OnDelVoiceUser(List<stVoiceUserInfo> voiceuser) {
		// TODO Auto-generated method stub
		
	}


	public void OnRoomUsersEnter(List<stRoomUserInfo> roomUsersList) {
		// TODO Auto-generated method stub
		String tempIDs[] = new String[roomUsersList.size()];
		String uniqueIDS[] = new String[roomUsersList.size()];
		int j = 0;
		for (Iterator<stRoomUserInfo> i = roomUsersList.iterator(); i.hasNext();)
		{  
			stRoomUserInfo userinfoRef = i.next(); 
			tempIDs[j] = String.valueOf(userinfoRef.getTempid());
			uniqueIDS[j] = userinfoRef.getUniqueid();
			j++;
		} 
		
	}

	public void OnRoomUserLeave(long leaveRoomUser) {
		// TODO Auto-generated method stub
		
	}

	public void OnRandChatInvite(long tempid, long roomid, String uniqueid) {
		// TODO Auto-generated method stub
		
	}
    
    
    
}
