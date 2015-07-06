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

import java.lang.reflect.Field;
import java.lang.reflect.InvocationTargetException;
import java.util.ArrayList;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.content.pm.ApplicationInfo;
import android.content.pm.PackageManager;
import android.content.pm.PackageManager.NameNotFoundException;
import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.os.Handler;
import android.util.Log;

public class PluginWrapper {

	protected static Context sContext = null;
	protected static Bundle mSavedInstanceState = null;
	protected static GLSurfaceView sGLSurfaceView = null;
	protected static Handler sMainThreadHandler = null;
	protected static Handler sGLThreadHandler = null;
	protected static ArrayList<Object> mPluginObjects = new ArrayList<Object>();
	private static final String TAG = "PluginWrapper";

	public static void init(Context context) {
		sContext = context;
		if (null == sMainThreadHandler) {
			sMainThreadHandler = new Handler();
		}
	}

	public static void setGLSurfaceView(GLSurfaceView value) {
		sGLSurfaceView = value;
	}

	protected static void initFromNativeActivity(Activity act) {
		sContext = act;
		// @warning These lines will cause crash.
		// if (null == sGLThreadHandler) {
		// sGLThreadHandler = new Handler();
		// }
	}

	public static void onPause() {
		callReflecMethod("onPause");
	}

	public static void onResume() {
		callReflecMethod("onResume");
	}

	public static void onStop() {
		callReflecMethod("onStop");
	}

	public static void onDestroy() {
		callReflecMethod("onDestroy");
	}
	public static void onStart() {
		callReflecMethod("onStart");
	}
	
	public static void handlePush(Intent intent){
		for (int i = 0; i < mPluginObjects.size(); i++) {
			Object o = mPluginObjects.get(i);
			java.lang.reflect.Method method;
			try {
				method = o.getClass().getMethod("handlePush",Intent.class);
				if (null != method) {
					method.invoke(o,new Object[]{intent});
				}
			} catch (SecurityException e) {
				// ...
			} catch (NoSuchMethodException e) {
				// ...
			} catch (IllegalAccessException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			} catch (IllegalArgumentException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			} catch (InvocationTargetException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
	}

	private static void callReflecMethod(String functionName) {
		for (int i = 0; i < mPluginObjects.size(); i++) {
			Object o = mPluginObjects.get(i);
			java.lang.reflect.Method method;
			try {
				method = o.getClass().getMethod(functionName);
				if (null != method) {
					method.invoke(o);
				}
			} catch (SecurityException e) {
				// ...
			} catch (NoSuchMethodException e) {
				// ...
			} catch (IllegalAccessException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			} catch (IllegalArgumentException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			} catch (InvocationTargetException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
	}

	public static void onNewIntent(Intent intent) {
		for (int i = 0; i < mPluginObjects.size(); i++) {
			Object o = mPluginObjects.get(i);
			java.lang.reflect.Method method;
			try {
				method = o.getClass().getMethod("onNewIntent", Intent.class);
				if (null != method) {
					method.invoke(o, intent);
				}
			} catch (SecurityException e) {
				// ...
			} catch (NoSuchMethodException e) {
				// ...
			} catch (IllegalAccessException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			} catch (IllegalArgumentException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			} catch (InvocationTargetException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
	}

	public static void onActivityResult(int requestCode, int responseCode,
			Intent intent) {
		for (int i = 0; i < mPluginObjects.size(); i++) {
			Object o = mPluginObjects.get(i);
			java.lang.reflect.Method method;
			try {
				method = o.getClass().getMethod("onActivityResult", int.class,
						int.class, Intent.class);
				if (null != method) {
					method.invoke(o, requestCode, responseCode, intent);
				}
			} catch (SecurityException e) {
				// ...
			} catch (NoSuchMethodException e) {
				// ...
			} catch (IllegalAccessException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			} catch (IllegalArgumentException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			} catch (InvocationTargetException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
	}

	protected static Object initPlugin(String classFullName) {
		Log.i(TAG, "class name : ----" + classFullName + "----");
		Class<?> c = null;
		try {
			String fullName = classFullName.replace('/', '.');
			c = Class.forName(fullName);
		} catch (ClassNotFoundException e) {
			Log.e(TAG, "Class " + classFullName + " not found.");
			e.printStackTrace();
			return null;
		}

		try {
			Context ctx = getContext();
			if (ctx != null) {
				Object o = c.getDeclaredConstructor(Context.class).newInstance(
						ctx);
				mPluginObjects.add(o);
				return o;
			} else {
				Log.e(TAG, "Plugin " + classFullName + " wasn't initialized.");
			}
		} catch (Exception e) {
			e.printStackTrace();
		}
		return null;
	}

	protected static int getPluginType(Object obj) {
		int nRet = -1;
		try {
			Field filedID = obj.getClass().getField("PluginType");
			Integer nObj = (Integer) filedID.get(obj);
			nRet = nObj.intValue();
		} catch (Exception e) {
			e.printStackTrace();
		}

		return nRet;
	}

	public static Context getContext() {
		return sContext;
	}
	
	public static void runOnGLThread(Runnable r) {
		if (null != sGLSurfaceView) {
			sGLSurfaceView.queueEvent(r);
		} else if (null != sGLThreadHandler) {
			sGLThreadHandler.post(r);
		} else {
			Log.i(TAG, "call back invoked on main thread");
			r.run();
		}
	}
	public static void setSavedInstanceState( Bundle savedInstanceState ){
		mSavedInstanceState = savedInstanceState;
	}
	public static Bundle getSavedInstanceState(){
		return mSavedInstanceState;
	}

	public static void runOnMainThread(Runnable r) {
		if (null != sMainThreadHandler) {
			sMainThreadHandler.post(r);
		} else if (null != sContext && sContext instanceof Activity) {
			Activity act = (Activity) sContext;
			act.runOnUiThread(r);
		}
	}
	public static boolean isDebug() {
		  PackageManager mgr = sContext.getPackageManager();
		  try {
		   ApplicationInfo info = mgr.getApplicationInfo(
				   sContext.getPackageName(), 0);
		   Log.d(TAG, "ApplicationInfo.FLAG_DEBUGGABLE: "
		     + ApplicationInfo.FLAG_DEBUGGABLE);
		   Log.d(TAG, "applicationInfo.flags: " + info.flags);
		   Log.d(TAG, "info.flags & ApplicationInfo.FLAG_DEBUGGABLE: "
		     + (info.flags & ApplicationInfo.FLAG_DEBUGGABLE));
		   Log.d(TAG,
		     "debug: "
		       + ((info.flags & ApplicationInfo.FLAG_DEBUGGABLE) == ApplicationInfo.FLAG_DEBUGGABLE));
		   return (info.flags & ApplicationInfo.FLAG_DEBUGGABLE) == ApplicationInfo.FLAG_DEBUGGABLE;
		  }
		  catch (NameNotFoundException e) {
		   Log.e(TAG, e.getMessage(), e);
		  }
		  return false;
		 }
}
