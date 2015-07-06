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

import java.io.IOException;

import android.annotation.SuppressLint;
import android.content.Context;
import android.content.SharedPreferences;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager.NameNotFoundException;
import android.os.AsyncTask;
import android.util.Log;

import com.google.android.gms.common.ConnectionResult;
import com.google.android.gms.common.GooglePlayServicesUtil;
import com.google.android.gms.gcm.GoogleCloudMessaging;

public class PushNotifacationRemote implements
		PushNotifacation.PushNotifacationRemote {
	protected GoogleCloudMessaging mGoogleCloudMessaging;
	protected Context mContext;
	protected String mRegistrationId;
	protected String mSenderID = "177468992129";
	protected PushNotifacation mAdapter;

	public static final String PROPERTY_REG_ID = "registration_id";
	private static final String PROPERTY_APP_VERSION = "appVersion";

	protected void tokenReceived() {
		UserWrapper.onActionResult(mAdapter, UserWrapper.kUserType_1, "");
		Log.v("GCM", "token = " + mRegistrationId);
	}

	private SharedPreferences getGcmPreferences() {
		if (null == mContext) {
			return null;
		}
		try {
			return mContext.getSharedPreferences(PushNotifacation.REMOTE_TAG,
					Context.MODE_PRIVATE);
		} catch (Exception e) {

		}
		return null;
	}

	private int getAppVersion() {
		try {
			PackageInfo packageInfo = mContext.getPackageManager()
					.getPackageInfo(mContext.getPackageName(), 0);
			return packageInfo.versionCode;
		} catch (NameNotFoundException e) {
			// should never happen
		}
		return 0;
	}

	private void storeRegistrationId(String regId) {
		try {
			final SharedPreferences prefs = getGcmPreferences();
			if (null == prefs) {
				return;
			}
			int appVersion = getAppVersion();
			SharedPreferences.Editor editor = prefs.edit();
			editor.putString(PROPERTY_REG_ID, regId);
			editor.putInt(PROPERTY_APP_VERSION, appVersion);
			editor.commit();
		} catch (Exception e) {

		}
	}

	@SuppressLint("NewApi")
	private String getRegistrationId() {
		try {
			final SharedPreferences prefs = getGcmPreferences();
			String registrationId = prefs.getString(PROPERTY_REG_ID, "");
			if (registrationId.isEmpty()) {
				return "";
			}
			int registeredVersion = prefs.getInt(PROPERTY_APP_VERSION,
					Integer.MIN_VALUE);
			int currentVersion = getAppVersion();
			if (registeredVersion != currentVersion) {
				return "";
			}
			return registrationId;
		} catch (Exception e) {

		}
		return "";
	}

	@SuppressLint("NewApi")
	public void init(Context c, PushNotifacation adapter) {
		mContext = c;
		mAdapter = adapter;
		PluginWrapper.runOnMainThread(new Runnable() {
			public void run() {
				if (checkPlayServices()) {
					mGoogleCloudMessaging = GoogleCloudMessaging
							.getInstance(mContext.getApplicationContext());
					mRegistrationId = getRegistrationId();
					if (mRegistrationId.isEmpty()) {
						registerInBackground();
					} else {
						tokenReceived();
					}
				}
			}
		});
	}

	private boolean checkPlayServices() {
		try{
			int resultCode = GooglePlayServicesUtil
					.isGooglePlayServicesAvailable(mContext);
			if (resultCode != ConnectionResult.SUCCESS) {
				if (GooglePlayServicesUtil.isUserRecoverableError(resultCode)) {
					return false;
				} else {

				}
				return false;
			}
			return true;
		}catch( Exception e ){
			return false;
		}
	}

	private void registerInBackground() {
		try {
			if (mGoogleCloudMessaging == null) {
				mGoogleCloudMessaging = GoogleCloudMessaging
						.getInstance(mContext.getApplicationContext());
			}
			new AsyncTask<Void, Void, String>() {
				@Override
				protected String doInBackground(Void... params) {
					try {
						mRegistrationId = mGoogleCloudMessaging
								.register(mSenderID);
					} catch (IOException e) {
					}
					catch( Exception e2 ){
					}
					return mRegistrationId;
				}

				@SuppressLint("NewApi")
				@Override
				protected void onPostExecute(String msg) {
					PluginWrapper.runOnMainThread(new Runnable() {
						public void run() {
							if (false == mRegistrationId.isEmpty()) {
								storeRegistrationId(mRegistrationId);
								tokenReceived();
							}
						}
					});
				}
			}.execute(null, null, null);
		} catch (Exception ex) {
			ex.printStackTrace();
		}catch( Throwable ignor){
			ignor.printStackTrace();
		}
	}

	@Override
	public String getDeviceToken() {
		if( null == mRegistrationId ){
			return "";
		}
		return mRegistrationId;
	}
}