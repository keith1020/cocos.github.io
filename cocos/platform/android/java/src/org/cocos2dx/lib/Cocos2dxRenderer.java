/****************************************************************************
Copyright (c) 2010-2011 cocos2d-x.org

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
package org.cocos2dx.lib;

import java.util.ArrayList;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import android.opengl.GLSurfaceView;

import org.cocos2dx.lib.Cocos2dxHelper;

public class Cocos2dxRenderer implements GLSurfaceView.Renderer {
	// ===========================================================
	// Constants
	// ===========================================================
	
	private boolean mIsPaused  = false;
	private int mDelayResumeCount = 0;
	private static int DELAY_RESUME_COUNT = 3;
	
	private final static long NANOSECONDSPERSECOND = 1000000000L;
	private final static long NANOSECONDSPERMICROSECOND = 1000000;

	class TouchPosition {
		public int mIndex = -1;
		public float mTouchPositionX = 0;
		public float mTouchPositionY = 0;
		public boolean mIsMoving = false;
	}

	private ArrayList<TouchPosition> mTouchPositions = new ArrayList<TouchPosition>();

	private static long sAnimationInterval = (long) (1.0 / 60 * Cocos2dxRenderer.NANOSECONDSPERSECOND);

	// ===========================================================
	// Fields
	// ===========================================================

	private long mLastTickInNanoSeconds;
	private int mScreenWidth;
	private int mScreenHeight;
	private boolean mNativeInitCompleted = false;

	// ===========================================================
	// Constructors
	// ===========================================================

	// ===========================================================
	// Getter & Setter
	// ===========================================================

	public static void setAnimationInterval(final double animationInterval) {
		Cocos2dxRenderer.sAnimationInterval = (long) (animationInterval * Cocos2dxRenderer.NANOSECONDSPERSECOND);
	}

	public void setScreenWidthAndHeight(final int surfaceWidth,
			final int surfaceHeight) {
		this.mScreenWidth = surfaceWidth;
		this.mScreenHeight = surfaceHeight;
	}

	// ===========================================================
	// Methods for/from SuperClass/Interfaces
	// ===========================================================

	@Override
	public void onSurfaceCreated(final GL10 GL10, final EGLConfig EGLConfig) {
		Cocos2dxRenderer.nativeInit(this.mScreenWidth, this.mScreenHeight);
		this.mLastTickInNanoSeconds = System.nanoTime();
		mNativeInitCompleted = true;
	}

	@Override
	public void onSurfaceChanged(final GL10 GL10, final int width,
			final int height) {
		Cocos2dxRenderer.nativeOnSurfaceChanged(width, height);
	}

	@Override
	public void onDrawFrame(final GL10 gl) {
		/*
		 * No need to use algorithm in default(60 FPS) situation, since
		 * onDrawFrame() was called by system 60 times per second by default.
		 */
		if( true == mIsPaused ){
			return;
		}
		if( mDelayResumeCount <= DELAY_RESUME_COUNT ){
			mDelayResumeCount = mDelayResumeCount + 1;
			if( mDelayResumeCount == DELAY_RESUME_COUNT ){
				Cocos2dxRenderer.nativeOnResume();
			}
			return;
		}
		if (sAnimationInterval <= 1.0 / 60 * Cocos2dxRenderer.NANOSECONDSPERSECOND) {
			Cocos2dxRenderer.nativeRender();
		} else {
			final long now = System.nanoTime();
			final long interval = now - this.mLastTickInNanoSeconds;

			if (interval < Cocos2dxRenderer.sAnimationInterval) {
				try {
					Thread.sleep((Cocos2dxRenderer.sAnimationInterval - interval)
							/ Cocos2dxRenderer.NANOSECONDSPERMICROSECOND);
				} catch (final Exception e) {
				}
			}
			/*
			 * Render time MUST be counted in, or the FPS will slower than
			 * appointed.
			 */
			this.mLastTickInNanoSeconds = System.nanoTime();
			Cocos2dxRenderer.nativeRender();
		}
	}

	// ===========================================================
	// Methods
	// ===========================================================

	private static native void nativeTouchesBegin(final int id, final float x,
			final float y);

	private static native void nativeTouchesEnd(final int id, final float x,
			final float y);

	private static native void nativeTouchesMove(final int[] ids,
			final float[] xs, final float[] ys);

	private static native void nativeTouchesCancel(final int[] ids,
			final float[] xs, final float[] ys);

	private static native boolean nativeKeyDown(final int keyCode);

	private static native void nativeRender();

	private static native void nativeInit(final int width, final int height);

	private static native void nativeOnSurfaceChanged(final int width,
			final int height);

	private static native void nativeOnPause();

	private static native void nativeOnResume();

	private void removeTouchPosition(final int id) {
		for (TouchPosition tp : mTouchPositions) {
			if (tp.mIndex == id) {
				mTouchPositions.remove(tp);
				return;
			}
		}
	}

	private boolean isTouchMoving(final int id, final float x, final float y) {
		for (TouchPosition tp : mTouchPositions) {
			if (tp.mIndex == id) {
				if (tp.mIsMoving) {
					return true;
				}
				float t = (tp.mTouchPositionX - x) * (tp.mTouchPositionX - x)
						+ (tp.mTouchPositionY - y) * (tp.mTouchPositionY - y);
				////澶т�� 25 涓����绱�锛���ゅ��涓虹Щ���
				if( t >= 25 * 25 ){
					tp.mIsMoving = true;
					return true;
				}
			}
		}
		return false;
	}

	public void handleActionDown(final int id, final float x, final float y) {
		Cocos2dxRenderer.nativeTouchesBegin(id, x, y);
		TouchPosition tp = new TouchPosition();
		tp.mIndex = id;
		tp.mTouchPositionX = x;
		tp.mTouchPositionY = y;
		tp.mIsMoving = false;
		mTouchPositions.add(tp);
	} 

	public void handleActionUp(final int id, final float x, final float y) {
		Cocos2dxRenderer.nativeTouchesEnd(id, x, y);
		removeTouchPosition(id);
	}

	public void handleActionCancel(final int[] ids, final float[] xs,
			final float[] ys) {
		Cocos2dxRenderer.nativeTouchesCancel(ids, xs, ys);
		for (int id : ids) {
			removeTouchPosition(id);
		}
	}

	public void handleActionMove(final int[] ids, final float[] xs,
			final float[] ys) {
		int length = ids.length;
		boolean isMoving = false;
		for( int i = 0 ; i < length; i ++ ){
			if( isTouchMoving( ids[ i ], xs[ i ], ys[ i ]) ){
				isMoving = true;
			}
		}
		if( true == isMoving ){
			Cocos2dxRenderer.nativeTouchesMove( ids, xs, ys );
		}
	}

	public void handleKeyDown(final int keyCode) {
		Cocos2dxRenderer.nativeKeyDown(keyCode);
	}

	public void handleOnPause() {
		/**
		 * onPause may be invoked before onSurfaceCreated, and engine will be
		 * initialized correctly after onSurfaceCreated is invoked. Can not
		 * invoke any native method before onSurfaceCreated is invoked
		 */
		if (!mNativeInitCompleted)
			return;

		Cocos2dxHelper.onEnterBackground();
		Cocos2dxRenderer.nativeOnPause();
		mIsPaused = true;
		mDelayResumeCount = 0;
	}

	public void handleOnResume() {
		mIsPaused = false;
		mDelayResumeCount = 0;
	}

	public void handleOnUserPresent() {
		Cocos2dxHelper.onEnterForeground();
	}

	private static native void nativeInsertText(final String text);

	private static native void nativeDeleteBackward();

	private static native String nativeGetContentText();

	public void handleInsertText(final String text) {
		Cocos2dxRenderer.nativeInsertText(text);
	}

	public void handleDeleteBackward() {
		Cocos2dxRenderer.nativeDeleteBackward();
	}

	public String getContentText() {
		return Cocos2dxRenderer.nativeGetContentText();
	}

	// ===========================================================
	// Inner and Anonymous Classes
	// ===========================================================
}
