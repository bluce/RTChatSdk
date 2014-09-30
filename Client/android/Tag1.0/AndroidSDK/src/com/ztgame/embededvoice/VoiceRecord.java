package com.ztgame.embededvoice;

import java.io.File;
import java.io.IOException;
import java.io.UnsupportedEncodingException;
import java.lang.reflect.Array;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;
import java.util.Arrays;
import java.util.Comparator;
import java.util.List;
import java.util.Timer;
import java.util.TimerTask;




import org.webrtc.webrtcdemo.MediaEngine;

import android.content.Context;
import android.os.Environment;
import android.util.Log;

public class VoiceRecord 
{
   private MediaEngine voe;
   private Context mCtx;
   private static final String VOICE_DIR = "stvoice/";
   //private final String VOICEURL = "http://122.11.47.94:10000/wangpan.php";
   private final String VOICEURL = "http://uploadchat.ztgame.com:10000/wangpan.php";
   private final String TAG = "VoiceRecord";
   
   private String couldfileUrl;
   private Timer recordingTimer;
   
   private VoiceChannelEngine moniter;
   
  public VoiceRecord(Context context,VoiceChannelEngine core)
  {
	  mCtx = context;
	  voe = new MediaEngine(mCtx);
	  voe.setTrace(true);
	  moniter = core;
	  createVoiceDirectory();
  }
  
  private String getVoiceDirectory() {
	    // Should create a folder in /scard/|LOG_DIR|
	    return Environment.getExternalStorageDirectory().toString() + "/" +
	    VOICE_DIR;
	  }

  private boolean createVoiceDirectory() {
	    File record_dir = new File(getVoiceDirectory());
	    if (!record_dir.exists()) {
	      return record_dir.mkdir();
	    }
	    return record_dir.isDirectory();
	  }
  
  public boolean StartRecording()
  {
	  
	  Log.d(TAG, "StartRecording----------------");
	  String voicefile = getVoiceDirectory() + "temp";
	  File recordfile = new File(voicefile);
	  if(recordfile.exists())
		  recordfile.delete();

	  voe.startRecordingAsFile(voicefile);
	  //启动限时Timer
	  recordingTimer = new Timer(false);
	  TimerTask stoprecordingTask = new TimerTask() {
		
		@Override
		public void run() {
			recordingTimer = null;
			StopRecording();
		}
	  };
	  
	  recordingTimer.schedule(stoprecordingTask, 30*1000);
	  return true;
  }
    
  
  public boolean StopRecording()
  {
	  Log.d(TAG, "StopRecording----------------");
	  if(recordingTimer != null)
	  {
		  recordingTimer.cancel();
		  recordingTimer = null;
	  }
	  
	  voe.stopRcordingAsFile();
	 // voe.stopRecordingPlayingout();
	  //读取上传
	  final String voicefile = getVoiceDirectory() + "temp";
	  File recordfile = new File(voicefile);
	  long filesize = 0; 
	  if(!recordfile.exists())
		 return false;
	  else
		  filesize = recordfile.length();
	  
	  final int duration = (int) (filesize/1500);
	  //上传文件开始
	  if(moniter.observer != null)
	  {
		  moniter.observer.OnStopVoiceRecording(filesize);
	  }
	  
     Thread postthred = new Thread(new Runnable() {
		
		@Override
		public void run() 
		{
			
			couldfileUrl = new UploadVoice().post(VOICEURL, null, voicefile);
			Log.d(TAG, "upload file successs" + couldfileUrl);
			  if(moniter.observer != null)
			  {
				  moniter.observer.OnUploadVoiceEnd(couldfileUrl,duration);
				  //for test 漂流瓶
				  moniter.ReqSendRandomRecording(couldfileUrl);
			  }
			/*测试下载文件123
			final String voicefile = getVoiceDirectory() + "testdownload1";
			boolean downloadret = new UploadVoice().getVoiceFile(cloudVoiceFileUrl, voicefile);
			*/
		}
	});	  
	  
     postthred.start();
     
	  return true;
  }
  
  private List<File> getFile(File file)
  {
		File[] fileArray =file.listFiles();
		List<File> mFileList = null;
		for (File f : fileArray) {
			if(f.isFile()){
				mFileList.add(f);
			}else{
				getFile(f);
			}
		}
		return mFileList;
  }
  
  //文件夹中按时间排序最新的文件读取  
  static class CompratorByLastModified implements Comparator 
  {
      public int compare(Object o1, Object o2) {
          File file1 = (File)o1;
          File file2 = (File)o2;
          long diff = file1.lastModified() - file2.lastModified();
          if (diff > 0)
             return 1;
          else if (diff == 0)
             return 0;
          else
             return -1;
      }
     
      public boolean equals(Object obj){
          return true;  //简单做法
      }
   }
  
  private void checkfilestorge()
  {
	  final String voicefile_dir = getVoiceDirectory();
	  File voiceDir = new File(voicefile_dir);
	  File[] voicefiles = voiceDir.listFiles();
	  
	  if(voicefiles.length > 10)
	  {
		  Arrays.sort(voicefiles,new CompratorByLastModified());
		  for (int i = 0; i< (voicefiles.length - 5); i++)
		  {
			  File deletfile = voicefiles[i];
			  if(deletfile.getName().compareTo("temp") != 0)
				   deletfile.delete();
		  }
	  }
  }
  
	/**
	 * 将字符串转成MD5值
	 * 
	 * @param string
	 * @return
	 */
	private String stringToMD5(String string) {
		byte[] hash;

		try {
			hash = MessageDigest.getInstance("MD5").digest(string.getBytes("UTF-8"));
		} catch (NoSuchAlgorithmException e) {
			e.printStackTrace();
			return null;
		} catch (UnsupportedEncodingException e) {
			e.printStackTrace();
			return null;
		}

		StringBuilder hex = new StringBuilder(hash.length * 2);
		for (byte b : hash) {
			if ((b & 0xFF) < 0x10)
				hex.append("0");
			hex.append(Integer.toHexString(b & 0xFF));
		}

		return hex.toString();
	}
  
  public boolean StartPlayVoice(final String url)
  {
	  
	  String formatefileUrl = stringToMD5(url);
	  final String voicefile = getVoiceDirectory() + formatefileUrl;//"test";//
	 
	  //判断文件是否在磁盘，不在去下载
	  File recordfile = new File(voicefile);
	  if(!recordfile.exists())
	  {
		  if(moniter.observer != null)
		  {
			  moniter.observer.OnDlVoiceBegin();
		  }
		  Thread downthred = new Thread(new Runnable() {
				
				@Override
				public void run() 
				{	
					//检查文件保存10个语音文件
					checkfilestorge();
					boolean downloadret = new UploadVoice().getVoiceFile(url, voicefile);
					
					if(downloadret)
					{
						Log.d(TAG, "下载后播放");
						File recordfile = new File(voicefile); 
						long filesize = recordfile.getTotalSpace();
						moniter.observer.OnDlVoiceEnd(downloadret,filesize);
						voe.startPlayFile(voicefile);
					}
					
					
					
				}
			});	  
			  
		  downthred.start();
	  }else{
		  voe.startPlayFile(voicefile);
	  }
		 
	  return true;
  }
  
  public boolean StopPlayVoice(String url)
  {
	  //voe.stopPlayingFileLocally(audiochannel);
	  voe.stopPlayFile(url);
	  return true;
  }
  
  public boolean Dispose()
  {
	  voe.dispose();
	  return true;
  }

}
