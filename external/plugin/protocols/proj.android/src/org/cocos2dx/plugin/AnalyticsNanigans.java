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

import org.json.JSONObject;

import com.nanigans.android.sdk.NanigansEventManager;
import com.nanigans.android.sdk.NanigansEventParameter;

import android.content.Context;

public class AnalyticsNanigans implements InterfaceAnalytics {

	Context mContext;
	AnalyticsNanigans mAdapter;
	boolean mDebugMode = false;
	String mUserID;

	public AnalyticsNanigans(Context context) {
		mContext = context;
		mAdapter = this;
	}

	public void initNanigans(JSONObject o) {
		if (null == o) {
			return;
		}
		try {
			String nanigansID = o.getString("Nanigans");
			String facebookID = o.getString("Facebook");
			String userID = o.getString("UserID");
			if (null == nanigansID || null == facebookID || null == userID) {
				return;
			}
			mUserID = userID;
			NanigansEventManager.getInstance().onActivityCreate(mContext,
					facebookID, Integer.parseInt(nanigansID));
			NanigansEventManager.getInstance().setUserId(userID);
		} catch (Exception e) {

		}
	}

	@Override
	public void startSession(String appKey) {

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
	}

	@Override
	public void logEvent(String eventId, Hashtable<String, String> paramMap) {
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

	public void trackRevenue(JSONObject o) {
		if (null == o) {
			return;
		}
		try {
			String eventName = o.getString("eventName");
			String productIdx = o.getString("productIdx");
			String productPrice = o.getString("productPrice");
			if (null == eventName || null == productIdx || null == productPrice) {
				return;
			}
			NanigansEventManager.getInstance().trackPurchase(
					Float.parseFloat(productPrice), productIdx, 1.0,
					new NanigansEventParameter("EventName", eventName));
		} catch (Exception e) {

		}
	}

	public void trackEvent(JSONObject o) {
		if (null == o) {
			return;
		}
		try {
			String eventName = o.getString("eventName");
			if (null == eventName) {
				return;
			}
			NanigansEventManager.getInstance().trackNanigansEvent(
					NanigansEventManager.TYPE.USER, eventName,
					new NanigansEventParameter("EventName", eventName));
		} catch (Exception e) {

		}
	}

}
