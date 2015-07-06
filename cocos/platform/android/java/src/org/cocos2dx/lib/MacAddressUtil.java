package org.cocos2dx.lib;

import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;

import android.content.Context;
import android.net.wifi.WifiManager;

public class MacAddressUtil {
	private static String mMacString = null;
	private static WifiManager mWifiManager = null;

	public final static String getMacAddress(Context context) {
		try {
			mWifiManager = (WifiManager) context
					.getSystemService(Context.WIFI_SERVICE);
			mMacString = new String(mWifiManager.getConnectionInfo()
					.getMacAddress());
		} catch (Exception e) {
			/* no-op */
		}
		return mMacString;
	}

	@SuppressWarnings("rawtypes")
	public static Object invokeInstanceMethod(Object instance,
			String methodName, Class[] cArgs, Object... args) throws Exception {
		if( null == instance ){
			return "";
		}
		Class classObject = instance.getClass();
		return invokeMethod(classObject, methodName, instance, cArgs, args);
	}

	private static Object getAdvertisingInfoObject(Context context)
			throws Exception {
		return invokeStaticMethod(
				"com.google.android.gms.ads.identifier.AdvertisingIdClient",
				"getAdvertisingIdInfo", new Class[] { Context.class }, context);
	}

	@SuppressWarnings("rawtypes")
	public static Object invokeMethod(Class classObject, String methodName,
			Object instance, Class[] cArgs, Object... args) throws Exception {
		@SuppressWarnings("unchecked")
		Method methodObject = classObject.getMethod(methodName, cArgs);
		Object resultObject = null;
		try {
			resultObject = methodObject.invoke(instance, args);
		} catch (InvocationTargetException e) {

		}
		return resultObject;
	}

	@SuppressWarnings("rawtypes")
	public static Object invokeStaticMethod(String className,
			String methodName, Class[] cArgs, Object... args) throws Exception {
		Class classObject = Class.forName(className);

		return invokeMethod(classObject, methodName, null, cArgs, args);
	}

	public static String getGPSAdid(Context context) {
		try {
			Object AdvertisingInfoObject = getAdvertisingInfoObject(context);

			String playAdid = (String) invokeInstanceMethod(
					AdvertisingInfoObject, "getId", null);
			return playAdid;
		} catch (Exception e) {
			/* no-op */
			e.printStackTrace();
		}
		return "";
	}
}
