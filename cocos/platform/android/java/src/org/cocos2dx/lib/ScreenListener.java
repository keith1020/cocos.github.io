package org.cocos2dx.lib;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.os.PowerManager;

public class ScreenListener {
	private Context mContext;
	private ScreenBroadcastReceiver mScreenReceiver;
	private ScreenStateListener mScreenStateListener;

	public ScreenListener(Context context) {
		mContext = context;
		mScreenReceiver = new ScreenBroadcastReceiver();
	}

	private class ScreenBroadcastReceiver extends BroadcastReceiver {
		private String action = null;

		@Override
		public void onReceive(Context context, Intent intent) {
			action = intent.getAction();
			if (Intent.ACTION_SCREEN_ON.equals(action)) {
				mScreenStateListener.onScreenOn();
			} else if (Intent.ACTION_SCREEN_OFF.equals(action)) {
				mScreenStateListener.onScreenOff();
			} else if (Intent.ACTION_USER_PRESENT.equals(action)) {
				mScreenStateListener.onUserPresent();
			}
		}
	}

	public void begin(ScreenStateListener listener) {
		mScreenStateListener = listener;
		registerListener();
		getScreenState();
	}

	private void getScreenState() {
		PowerManager manager = (PowerManager) mContext
				.getSystemService(Context.POWER_SERVICE);
		if (manager.isScreenOn()) {
			if (mScreenStateListener != null) {
				mScreenStateListener.onScreenOn();
			}
		} else {
			if (mScreenStateListener != null) {
				mScreenStateListener.onScreenOff();
			}
		}
	}

	public void unregisterListener() {
		if (null == mContext || null == mScreenReceiver) {
			return;
		}
		try {
			mContext.unregisterReceiver(mScreenReceiver);
		} catch (Exception e) {

		}
	}

	private void registerListener() {
		if (null == mContext || null == mScreenReceiver) {
			return;
		}
		try {
			unregisterListener();
			IntentFilter filter = new IntentFilter();
			filter.addAction(Intent.ACTION_SCREEN_ON);
			filter.addAction(Intent.ACTION_SCREEN_OFF);
			filter.addAction(Intent.ACTION_USER_PRESENT);
			mContext.registerReceiver(mScreenReceiver, filter);
		} catch (Exception e) {

		}
	}

	public interface ScreenStateListener {
		public void onScreenOn();

		public void onScreenOff();

		public void onUserPresent();
	}
}