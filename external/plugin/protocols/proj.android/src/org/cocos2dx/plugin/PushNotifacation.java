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
import java.util.ArrayList;
import java.util.Calendar;
import java.util.Collections;
import java.util.Comparator;
import java.util.Hashtable;

import org.json.JSONArray;
import org.json.JSONObject;

import android.annotation.SuppressLint;
import android.app.Activity;
import android.app.AlarmManager;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.content.pm.ApplicationInfo;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.content.pm.PackageManager.NameNotFoundException;
import android.os.AsyncTask;
import android.os.Bundle;
import android.util.Log;

@SuppressLint("NewApi")
public class PushNotifacation implements InterfaceUser {
	
	public interface PushNotifacationRemote
	{
		public void init( Context c , PushNotifacation adapter );
		public String getDeviceToken();
	}

	static final String TAG = "__PushNotifacation__";
	static final String TAG_JSON = "__PushNotifacation_JSON__";
	static final String REMOTE_TAG = "__REMOTE_PushNotifacation__";

	static final String JSON_TAG_HASHCODE = "hashCode";
	static final String JSON_TAG_NAME = "name";
	static final String JSON_TAG_WHEN = "when";
	static final String JSON_TAG_PRIORITY = "priority";

	public static final String EXTRA_MESSAGE = "message";

	protected PushNotifacation mAdapter;
	protected Context mContext = null;
	protected AlarmManager mAlarmManager = null;

	protected PushNotifacationRemote mPushNotifacationRemote = null;

	public PushNotifacation(Context context) {
		mContext = context;
		mAdapter = this;
		Bundle bundle = PluginWrapper.getSavedInstanceState();
		if (null != bundle
				&& false == bundle.getBoolean("showNotifacation", false)) {
			clearRemoteNotifacation();
		}
	}

	@Override
	public void configDeveloperInfo(Hashtable<String, String> cpInfo) {
		// TODO Auto-generated method stub
		mAlarmManager = (AlarmManager) mContext
				.getSystemService(Context.ALARM_SERVICE);
		if (null == mAlarmManager) {
			return;
		}
		Class<?> remoteClazz=null;
        try{
        	remoteClazz=Class.forName("org.cocos2dx.plugin.PushNotifacationRemote");
        }catch (Exception e) {
            e.printStackTrace();
        }
        if( null != remoteClazz ){
        	try {
            	mPushNotifacationRemote=(PushNotifacationRemote)remoteClazz.newInstance();
            } catch (InstantiationException e) {
                // TODO Auto-generated catch block
                e.printStackTrace();
            } catch (IllegalAccessException e) {
                // TODO Auto-generated catch block
                e.printStackTrace();
            }
        }
        if( null != mPushNotifacationRemote ){
        	mPushNotifacationRemote.init(mContext, this);
        }
	}

	public String getDeviceToken() {
		if( null != mPushNotifacationRemote ){
			return mPushNotifacationRemote.getDeviceToken();
		}
		return "";
	}

	@Override
	public void login() {
		// TODO Auto-generated method stub

	}

	@Override
	public void logout() {
		// TODO Auto-generated method stub

	}

	@Override
	public boolean isLogined() {
		// TODO Auto-generated method stub
		return false;
	}

	@Override
	public String getSessionID() {
		// TODO Auto-generated method stub
		return null;
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

	public void createPushNotifacation(JSONObject params) {

		if (null == mAlarmManager) {
			return;
		}
		try {
			NotificationManager notifyMgr = ((NotificationManager) mContext
					.getSystemService(Context.NOTIFICATION_SERVICE));
			if (null != notifyMgr) {
				String name = params.getString("name");
				String delay = params.getString("delay");
				String sound = params.getString("sound");
				String badge = params.getString("badge");
				String repeat = params.getString("repeat");
				String body = params.getString("body");
				String priority = params.getString("priority");
				String title = "";
				if (params.has("title")) {
					title = params.getString("title");
				}
				int delaySecond = 0;
				try {
					delaySecond = (int) Float.parseFloat(delay);
				} catch (Exception e) {

				}
				Intent alarmIntent = getAlarmReceiverClazz();
				if (null == alarmIntent) {
					return;
				}

				Bundle bundle = new Bundle();
				bundle.putString("name", name);
				bundle.putString("delay", delay);
				bundle.putString("sound", sound);
				bundle.putString("badge", badge);
				bundle.putString("repeat", repeat);
				bundle.putString("body", body);
				bundle.putString("title", title);
				bundle.putString("priority", priority);
				alarmIntent.putExtras(bundle);
				
				PendingIntent pendingIntent = PendingIntent.getBroadcast(
						mContext, stringHashCode(name), alarmIntent, 0);

				Calendar alarmStartTime = Calendar.getInstance();
				alarmStartTime.add(Calendar.SECOND, delaySecond);
				long when = alarmStartTime.getTimeInMillis();
				boolean isRepeat = Boolean.parseBoolean(repeat);
				int priorityValue = 0;
				try {
					priorityValue = (int) Double.parseDouble(priority);
				} catch (Exception e) {
					try {
						priorityValue = Integer.parseInt(priority);
					} catch (Exception e2) {

					}
				}
				if (true == isRepeat) {
					mAlarmManager.setRepeating(AlarmManager.RTC_WAKEUP, when,
							AlarmManager.INTERVAL_DAY, pendingIntent);
				} else {
					mAlarmManager.set(AlarmManager.RTC_WAKEUP, when,
							pendingIntent);
				}
				addPush(name, when, priorityValue);
			}
		} catch (Exception e) {

		}
	}

	protected int stringHashCode(String notifationName) {
		int hashCode = notifationName.hashCode();
		if (hashCode <= 0) {
			return -1 * hashCode;
		}
		return hashCode;
	}

	protected String pushDataToString(String notifationName, long when,
			int priorityValue) {
		try {
			int hashCode = stringHashCode(notifationName);
			JSONObject o = new JSONObject();
			o.put(PushNotifacation.JSON_TAG_HASHCODE, hashCode);
			o.put(PushNotifacation.JSON_TAG_WHEN, when);
			o.put(PushNotifacation.JSON_TAG_PRIORITY, priorityValue);
			o.put(PushNotifacation.JSON_TAG_NAME, notifationName);
			return o.toString();
		} catch (Exception e) {

		}
		return "";
	}

	protected void addPush(String notifationName, long when, int priorityValue) {
		int hashCode = stringHashCode(notifationName);
		SharedPreferences sharedPref = ((Activity) mContext)
				.getPreferences(Context.MODE_PRIVATE);
		String s = sharedPref.getString(PushNotifacation.TAG, "");
		JSONObject jsonObj = null;
		try {
			JSONArray array = null;
			try {
				jsonObj = new JSONObject(s);
			} catch (Exception e2) {
				jsonObj = new JSONObject();
			}
			try {
				if (jsonObj.has(PushNotifacation.TAG_JSON)) {
					array = jsonObj.getJSONArray(PushNotifacation.TAG_JSON);
				} else {
					array = new JSONArray();
				}
			} catch (Exception e2) {
				array = new JSONArray();
			}
			if (null != array) {
				for (int i = 0; i < array.length(); i++) {
					JSONObject o = array.getJSONObject(i);
					if (null == o) {
						continue;
					}
					if (o.get(PushNotifacation.JSON_TAG_HASHCODE).equals(
							hashCode)) {
						return;
					}
				}
			} else {
				array = new JSONArray();
			}
			JSONObject insertObj = new JSONObject(pushDataToString(
					notifationName, when, priorityValue));
			array.put(insertObj);
			SharedPreferences.Editor editor = sharedPref.edit();
			JSONObject o = new JSONObject();
			o.put(PushNotifacation.TAG_JSON, array);
			s = o.toString();
			editor.putString(PushNotifacation.TAG, s);
			editor.commit();
		} catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}

	protected void removePush(int hashCode) {
		SharedPreferences sharedPref = ((Activity) mContext)
				.getPreferences(Context.MODE_PRIVATE);
		String s = sharedPref.getString(PushNotifacation.TAG, "");
		JSONObject jsonObj = null;
		try {
			try {
				jsonObj = new JSONObject(s);
			} catch (Exception e2) {
				jsonObj = new JSONObject();
			}
			JSONArray array = null;
			try {
				if (jsonObj.has(PushNotifacation.TAG_JSON)) {
					array = jsonObj.getJSONArray(PushNotifacation.TAG_JSON);
				} else {
					array = new JSONArray(PushNotifacation.TAG_JSON);
				}
			} catch (Exception e2) {
				array = new JSONArray(PushNotifacation.TAG_JSON);
			}
			JSONArray newArray = new JSONArray();
			if (null != array) {
				for (int i = 0; i < array.length(); i++) {
					JSONObject o = array.getJSONObject(i);
					if (null == o) {
						continue;
					}
					if (false == o.get(PushNotifacation.JSON_TAG_HASHCODE)
							.equals(hashCode)) {
						newArray.put(o);
					}
				}
			}
			SharedPreferences.Editor editor = sharedPref.edit();
			JSONObject o = new JSONObject();
			o.put(PushNotifacation.TAG_JSON, newArray);
			s = o.toString();
			editor.putString(PushNotifacation.TAG, s);
			editor.commit();
		} catch (Exception e) {

		}
	}
	protected void removePush(String notifationName) {
		int hashCode = stringHashCode(notifationName);
		removePush(hashCode);
	}

	protected ArrayList<Integer> getAllPushes() {
		ArrayList<Integer> ret = new ArrayList<Integer>();
		SharedPreferences sharedPref = ((Activity) mContext)
				.getPreferences(Context.MODE_PRIVATE);
		String s = sharedPref.getString(PushNotifacation.TAG, "");
		JSONObject jsonObj = null;
		try {
			jsonObj = new JSONObject(s);
			JSONArray array = jsonObj.getJSONArray(PushNotifacation.TAG_JSON);
			if (null != array) {
				for (int i = 0; i < array.length(); i++) {
					JSONObject o = array.getJSONObject(i);
					if (null == o) {
						continue;
					}
					ret.add(o.getInt(PushNotifacation.JSON_TAG_HASHCODE));
				}
			}
		} catch (Exception e) {

		}
		return ret;
	}

	public String getAlarmReceiver() {
		ApplicationInfo ai = null;
		try {
			ai = mContext.getPackageManager().getApplicationInfo(
					mContext.getPackageName(), PackageManager.GET_META_DATA);
		} catch (PackageManager.NameNotFoundException e) {
			return null;
		}
		if (ai == null || ai.metaData == null) {
			return null;
		}
		String s = ai.metaData.getString("PushNotifacationReceiver");
		return s;
	}

	public Intent getAlarmReceiverClazz() {
		Intent alarmIntent = null;
		String receiverClazzName = getAlarmReceiver();
		try {
			alarmIntent = new Intent(mContext, Class.forName(receiverClazzName));
		} catch (ClassNotFoundException e) {
			e.printStackTrace();
		}
		return alarmIntent;
	}

	public void removePushNotifacation(int hashCode) {
		if (null == mAlarmManager) {
			return;
		}
		Intent alarmIntent = getAlarmReceiverClazz();
		if (null == alarmIntent) {
			return;
		}
		PendingIntent pendingIntent = PendingIntent.getBroadcast(mContext,
				hashCode, alarmIntent, PendingIntent.FLAG_NO_CREATE);
		if (null != pendingIntent) {
			mAlarmManager.cancel(pendingIntent);
		}
		removePush(hashCode);
	}

	public void removePushNotifacation(String notifationName) {
		if (null == mAlarmManager) {
			return;
		}
		Intent alarmIntent = getAlarmReceiverClazz();
		if (null == alarmIntent) {
			return;
		}
		PendingIntent pendingIntent = PendingIntent.getBroadcast(mContext,
				stringHashCode(notifationName), alarmIntent,
				PendingIntent.FLAG_NO_CREATE);
		if (null != pendingIntent) {
			mAlarmManager.cancel(pendingIntent);
		}
		removePush(notifationName);
	}

	public void refreshLocalNotifacationBadgeNumber() {

	}

	public void removeTimecloseNotifacation(int second) {
		if (0 == second) {
			return;
		}
		class ByWeightComparator implements Comparator<JSONObject> {
			@Override
			public int compare(JSONObject lhs, JSONObject rhs) {
				try {
					Integer priority1 = lhs.getInt(PushNotifacation.JSON_TAG_PRIORITY);
					Integer priority2 = rhs.getInt(PushNotifacation.JSON_TAG_PRIORITY);
					return priority1.compareTo( priority2 );
				} catch (Exception e) {

				}
				return 0;
			}
		}
		SharedPreferences sharedPref = ((Activity) mContext)
				.getPreferences(Context.MODE_PRIVATE);
		String s = sharedPref.getString(PushNotifacation.TAG, "");
		JSONObject jsonObj = null;
		try {
			jsonObj = new JSONObject(s);
			JSONArray array = jsonObj.getJSONArray(PushNotifacation.TAG_JSON);
			ArrayList<JSONObject> jsonArray = new ArrayList<JSONObject>();
			if (null != array) {
				for (int i = 0; i < array.length(); i++) {
					JSONObject o = array.getJSONObject(i);
					if (null == o) {
						continue;
					}
					jsonArray.add(o);
				}
			}
			Collections.sort(jsonArray, new ByWeightComparator());
			ArrayList<JSONObject> jsonArrayNeedRemove = new ArrayList<JSONObject>();
			for (int i = 0; i < jsonArray.size(); i++) {
				for (int j = i + 1; j < jsonArray.size(); j++) {
					JSONObject a = jsonArray.get(i);
					JSONObject b = jsonArray.get(j);
					int delay1 = a.getInt(PushNotifacation.JSON_TAG_WHEN);
					int delay2 = b.getInt(PushNotifacation.JSON_TAG_WHEN);
					// /!删除时间非常近的
					if (Math.abs(delay1 - delay2) <= second * 1000) {
						boolean needInsert = true;
						for (int k = 0; k < jsonArrayNeedRemove.size(); k++) {
							JSONObject o = jsonArrayNeedRemove.get(k);
							if (o.getInt(PushNotifacation.JSON_TAG_HASHCODE) == b
									.getInt(PushNotifacation.JSON_TAG_HASHCODE)) {
								needInsert = false;
							}
						}
						if (true == needInsert) {
							jsonArrayNeedRemove.add(b);
						}
					}
				}
			}
			for (int i = 0; i < jsonArrayNeedRemove.size(); i++) {
				removePushNotifacation(jsonArrayNeedRemove.get(i).getInt(
						PushNotifacation.JSON_TAG_HASHCODE));
			}

		} catch (Exception e) {

		}
	}

	public void removeAllNotifacation() {
		if (null == mAlarmManager) {
			return;
		}
		ArrayList<Integer> allPushed = getAllPushes();
		if (null == allPushed || allPushed.size() == 0) {
			return;
		}
		for (int id : allPushed) {
			Intent alarmIntent = getAlarmReceiverClazz();
			if (null == alarmIntent) {
				return;
			}
			PendingIntent pendingIntent = PendingIntent.getBroadcast(mContext,
					id, alarmIntent, PendingIntent.FLAG_NO_CREATE);
			if (null != pendingIntent) {
				mAlarmManager.cancel(pendingIntent);
			}
		}
		SharedPreferences sharedPref = ((Activity) mContext)
				.getPreferences(Context.MODE_PRIVATE);
		SharedPreferences.Editor editor = sharedPref.edit();
		editor.putString(PushNotifacation.TAG, "");
		editor.commit();
	}

	public String getLaunchNotification() {
		SharedPreferences sharedPref = mContext.getSharedPreferences(
				"remotenotifacation", Context.MODE_PRIVATE);
		;
		String s = sharedPref.getString("remotenotifacation", "");
		return s;
	}

	public String getCurrentNotification() {
		SharedPreferences sharedPref = mContext.getSharedPreferences(
				"remotenotifacation", Context.MODE_PRIVATE);
		;
		String s = sharedPref.getString("remotenotifacation", "");
		return s;
	}

	public void clearRemoteNotifacation() {
		SharedPreferences sharedPref = mContext.getSharedPreferences(
				"remotenotifacation", Context.MODE_PRIVATE);
		;
		SharedPreferences.Editor editor = sharedPref.edit();
		editor.putString("remotenotifacation", "");
		editor.commit();
	}
}