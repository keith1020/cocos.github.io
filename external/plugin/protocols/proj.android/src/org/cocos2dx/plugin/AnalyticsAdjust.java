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
import java.util.Map.Entry;

import org.json.JSONException;
import org.json.JSONObject;

import android.content.Context;
import android.util.Log;

import com.adjust.sdk.Adjust;
import com.adjust.sdk.AdjustAttribution;
import com.adjust.sdk.AdjustConfig;
import com.adjust.sdk.AdjustEvent;
import com.adjust.sdk.LogLevel;
import com.adjust.sdk.OnAttributionChangedListener;

public class AnalyticsAdjust implements InterfaceAnalytics {

	Context mContext;
	AnalyticsAdjust mAdapter;
	boolean mDebugMode = false;
	

	public AnalyticsAdjust(Context context) {
		mContext = context;
		mAdapter = this;
	}
	
	@Override
	public void startSession(String appKey) {
		// TODO Auto-generated method stub
		String appToken = appKey;
		String environment = AdjustConfig.ENVIRONMENT_PRODUCTION;
		if( PluginWrapper.isDebug() || true == mDebugMode ){
			environment = AdjustConfig.ENVIRONMENT_SANDBOX;
		}
		
		AdjustConfig config = new AdjustConfig(mContext, appToken, environment);
		// change the log level
		config.setLogLevel(LogLevel.VERBOSE);
		// set attribution delegate
		config.setOnAttributionChangedListener(new OnAttributionChangedListener() {
			@Override
			public void onAttributionChanged(AdjustAttribution attribution) {
				Log.d("example", "attribution: " + attribution.toString());
			}
		});

		Adjust.onCreate(config);
	}

	@Override
	public void stopSession() {
		// TODO Auto-generated method stub

	}

	@Override
	public void setSessionContinueMillis(int millis) {
		// TODO Auto-generated method stub
 
	}

	@Override
	public void setCaptureUncaughtException(boolean isEnabled) {
		// TODO Auto-generated method stub

	}

	@Override
	public void setDebugMode(boolean isDebugMode) {
		// TODO Auto-generated method stub
		mDebugMode = isDebugMode;
	}

	@Override
	public void logError(String errorId, String message) {
		// TODO Auto-generated method stub
     
	} 

	@Override
	public void logEvent(String eventId) {
		// TODO Auto-generated method stub
		AdjustEvent ae = new AdjustEvent(eventId);
		Adjust.trackEvent(ae);
	}

	@Override
	public void logEvent(String eventId, Hashtable<String, String> paramMap) {
		// TODO Auto-generated method stub
		AdjustEvent ae = new AdjustEvent(eventId);
		for (Entry<String, String> entry : paramMap.entrySet()) {
			ae.addCallbackParameter(entry.getKey(), entry.getValue());
		}
		Adjust.trackEvent(ae);
	}

	@Override
	public void logTimedEventBegin(String eventId) {
		// TODO Auto-generated method stub

	}

	@Override
	public void logTimedEventEnd(String eventId) {
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
	public void trackRevenue( JSONObject o ){
		if (null == o) {
			return;
		}
		try {
			double m = o.getDouble( "money" );
			String r = o.getString( "receipt" );
			String t = o.getString( "token" );
			String c = o.getString( "currencyCode" );
			
			if(  null == r || null == t || null == c ){
				return;
			}
			AdjustEvent ae = new AdjustEvent(t);
			ae.setRevenue(m, c);
			Adjust.trackEvent(ae);
		} catch (JSONException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		catch( Exception e ){
			e.printStackTrace();
		}
	}

}
