/****************************************************************************
Copyright (c) 2008-2010 Ricardo Quesada
Copyright (c) 2010-2012 cocos2d-x.org
Copyright (c) 2011      Zynga Inc.
Copyright (c) 2013-2014 Chukong Technologies Inc.
 
http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/
package org.cocos2dx.cpp;

import java.util.ArrayList;

import org.cocos2dx.lib.Cocos2dxActivity;

import com.ztgame.embededvoice.JNIObserver;
import com.ztgame.embededvoice.VoiceChannelEngine;

import android.os.Bundle;

public class AppActivity extends Cocos2dxActivity 
{

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		super.onCreate(savedInstanceState);
		VoiceChannelEngine.Create(this, VoiceChannelEngine.enInterfaceType.INTERFACE_NATIVE);
		
		//test jni
		 int a[] = new int[] { 1, 2, 3, 4, 5 };
		 ///String [] str = { "we", "are", "friends" };
		 //str[0] = "123";str[1] = "567";str[2] = "567";
		 String[] strArray = {"china", "company"};
		 
		 
		
		
//		 ArrayList<String> arrayList = new ArrayList<String>();
//         arrayList.add("hello");
//         arrayList.add("jni");
//         arrayList.add("i am java");
//         
		 //JNIObserver observer = new JNIObserver();
//		 observer.TestResult(arrayList);
		 //observer.RoomListResult(strArray, a);
		 //long id = 12345678901;
		 //String id = new String("12345678");
		 //observer.LoginResult(true,id);
	}
	
}
