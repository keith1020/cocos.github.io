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

import java.util.Hashtable;

import com.chartboost.sdk.Chartboost;
import com.chartboost.sdk.Libraries.CBLogging.Level;

import android.app.Activity;
import android.content.Context;

public class AdsChartboost implements InterfaceAds {

	Context mContext;
	AdsChartboost mAdapter;
	String mAppKey;
	String mAppSignature;
	
	public AdsChartboost(Context context) {
		mContext = context;
		mAdapter = this;
	}
	@Override
	public void configDeveloperInfo(Hashtable<String, String> devInfo) {
		// TODO Auto-generated method stub
		mAppKey = devInfo.get("AppID");
		mAppSignature = devInfo.get("AppSignature");
	    Runnable runnable = new Runnable() {
			@Override
			public void run() {
				Chartboost.startWithAppId((Activity) mContext, mAppKey, mAppSignature);
				Chartboost.setLoggingLevel(Level.ALL);
				Chartboost.onCreate((Activity) mContext);
				Chartboost.onStart((Activity) mContext);
			}
		};
		PluginWrapper.runOnMainThread(runnable);
	}
	public void onStart() {
	    Chartboost.onStart((Activity) mContext);
	}

	public void onResume() {
	    Chartboost.onResume((Activity) mContext);
	}
	public void onPause() {
		Chartboost.onPause((Activity) mContext);
	}
	       
	@Override
	public void showAds(Hashtable<String, String> adsInfo, int pos) {
		// TODO Auto-generated method stub
		
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


}
