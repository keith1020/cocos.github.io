/****************************************************************************
Copyright (c) 2012-2013 cocos2d-x.org

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
package org.cocos2dx.plugin;

import java.util.HashMap;
import java.util.Hashtable;

import com.helpshift.Helpshift;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;

public class AnalyticsHelpShift implements InterfaceAds {

	Context mContext;
	AnalyticsHelpShift mAdapter;
	String mAppKey;
	String mAppSignature;
	String mDomainName;
	String mUserIdentifier;
	boolean isInited = false;
	String mToken;

	public AnalyticsHelpShift(Context context) {
		mContext = context;
		mAdapter = this;
	}

	@Override
	public void configDeveloperInfo(Hashtable<String, String> devInfo) {
		isInited = true;
		mAppKey = devInfo.get("AppID");
		mAppSignature = devInfo.get("AppSignature");
		mDomainName = devInfo.get("domainName");
		mUserIdentifier = devInfo.get("userIdentifier");
		Runnable runnable = new Runnable() {
			@Override
			public void run() {
				Helpshift.install(((Activity) mContext).getApplication(), mAppKey,
						mDomainName, mAppSignature);
				if( null != mUserIdentifier ){
					Helpshift.setUserIdentifier( mUserIdentifier );
				}
				Helpshift.registerDeviceToken(mContext, mToken);
			}
		};
		PluginWrapper.runOnMainThread(runnable);
		
	}
	
	@Override
	public void showAds(Hashtable<String, String> adsInfo, int pos) {
		try{
			HashMap<String, String> metaData = new HashMap<String, String>();
			metaData.put("viplevel", adsInfo.get("viplevel"));
			metaData.put("server", adsInfo.get("server"));
			
			HashMap<String, HashMap<String, String>> config =new HashMap<String, HashMap<String, String>>();
			config.put(Helpshift.HSCustomMetadataKey, metaData);
			if( null != adsInfo.get("FAQ") ){
				Helpshift.showFAQs((Activity) mContext, config );
				return;
			}else if( null != adsInfo.get("Conversation") ){
				Helpshift.showConversation((Activity) mContext, config);
				return;
			}
			Helpshift.showFAQs((Activity) mContext,config);
		}catch(Throwable e ){
			
		}
	}

	@Override
	public void hideAds(Hashtable<String, String> adsInfo) {
		// TODO Auto-generated method stub

	}

	@Override
	public void queryPoints() {
		// TODO Auto-generated method stub

	}

	@Override
	public void spendPoints(int points) {
		// TODO Auto-generated method stub

	}

	@Override
	public void setDebugMode(boolean debug) {
		// TODO Auto-generated method stub

	}

	@Override
	public String getSDKVersion() {
		// TODO Auto-generated method stub
		return null;
	}

	@Override
	public String getPluginVersion() {
		// TODO Auto-generated method stub
		return null;
	}
	
	public void registerDeviceToken(String deviceToken){
		if(isInited)
			Helpshift.registerDeviceToken(mContext, deviceToken);
		else
			mToken = deviceToken;
	}
	
	public void handlePush(Intent intent){
		Helpshift.handlePush(mContext,intent);
	}

}