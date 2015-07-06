/*
 * Copyright (C) 2013 Google Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package org.cocos2dx.plugin;

import java.math.BigDecimal;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Currency;
import java.util.Hashtable;
import java.util.Iterator;
import java.util.List;

import org.cocos2dx.libAnalyticsCommon.R;
import org.json.JSONException;
import org.json.JSONObject;

import android.annotation.SuppressLint;
import android.app.Activity;
import android.app.AlertDialog;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.net.Uri;
import android.os.Bundle;
import android.os.Handler;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.Window;
import android.widget.ImageView;
import android.widget.TextView;

import com.facebook.AccessToken;
import com.facebook.AccessTokenTracker;
import com.facebook.CallbackManager;
import com.facebook.FacebookCallback;
import com.facebook.FacebookException;
import com.facebook.FacebookSdk;
import com.facebook.GraphRequest;
import com.facebook.GraphResponse;
import com.facebook.HttpMethod;
import com.facebook.ProfileTracker;
import com.facebook.appevents.AppEventsLogger;
import com.facebook.login.DefaultAudience;
import com.facebook.login.LoginBehavior;
import com.facebook.login.LoginManager;
import com.facebook.login.LoginResult;
import com.facebook.share.ShareApi;
import com.facebook.share.Sharer;
import com.facebook.share.model.GameRequestContent;
import com.facebook.share.model.GameRequestContent.ActionType;
import com.facebook.share.model.ShareLinkContent;
import com.facebook.share.model.SharePhoto;
import com.facebook.share.model.SharePhotoContent;
import com.facebook.share.widget.GameRequestDialog;
import com.facebook.share.widget.LikeView;
import com.facebook.share.widget.LikeView.ObjectType;
import com.facebook.share.widget.ShareDialog;

public class ShareFacebook implements InterfaceShare {

	static final String TAG = "ShareFacebook";

	static final int LOGIN_TYPE_READ = 1;
	static final int LOGIN_TYPE_WRITE = 2;

	static final int ERROR_CODE_UNKOWN_EXCEPTION = 1;
	static final int ERROR_CODE_CANCEL = 2;
	static final int ERROR_CODE_ACCESSTOKEN = 3;

	static final int SHARE_TYPE_ONLINE_IMAGE = 1;
	static final int SHARE_TYPE_VIDEO = 2;
	static final int SHARE_TYPE_LOCAL_IMAGE = 3;

	static final int SHARE_NO_NATIVE_FACEBOOK = 1024;
	static final int SHARE_NORMAL_ERROR = 1023;

	protected ShareFacebook mAdapter;
	protected Context mContext = null;
	protected String mAppKey;
	protected String mAppSecret;
	protected String mAppToken;
	protected List<String> mPermissions;
	protected boolean mTrackingPendingCall = false;
	private boolean mIsLoginSuccess = false;
	private boolean mIsGettingEmail = false;
	private Hashtable<String, String> mDictinaryData = new Hashtable<String, String>();
	private Hashtable<String, String> mShareDictionary = new Hashtable<String, String>();
	private CallbackManager mCallbackManager;
	private AccessTokenTracker mAccessTokenTracker;
	private boolean mIsInLoginProcess = false;
	private boolean mIsGamePaused = false;
	private AppEventsLogger mAppEventsLogger;
	private int mCurrentLoginType = ShareFacebook.LOGIN_TYPE_READ;
	private Handler mHandler;
	private GameRequestDialog mGameRequestDialog = null;

	private int mPendingAction = 0;
	private static int PENDING_ACTION_LOGIN_SUCCESS = 1;
	private static int PENDING_ACTION_LOGIN_FAILED = 1;

	public ShareFacebook(Context context) {
		mContext = context;
		mAdapter = this;
	}

	@Override
	public void configDeveloperInfo(Hashtable<String, String> cpInfo) {
		mAppKey = cpInfo.get("AppID");
		mAppSecret = cpInfo.get("AppSecret");
		mAppToken = cpInfo.get("AppToken");
		String permission = cpInfo.get("Permission");
		mPermissions = Arrays.asList(permission.split(","));

		FacebookSdk.setApplicationId(mAppKey);
		FacebookSdk.sdkInitialize(mContext);
		try {
			if (null == mAppEventsLogger) {
				mAppEventsLogger = AppEventsLogger.newLogger(mContext);
			}
			AppEventsLogger.activateApp(mContext);
		} catch (Throwable e) {

		}
		LoginManager manager = LoginManager.getInstance();
		mCallbackManager = CallbackManager.Factory.create();
		manager.registerCallback(mCallbackManager, new FacebookCallback<LoginResult>() {
			@Override
			public void onSuccess(LoginResult loginResult) {
				if (mCurrentLoginType == ShareFacebook.LOGIN_TYPE_READ) {
					com.facebook.Profile.fetchProfileForCurrentAccessToken();
				} else if (mCurrentLoginType == ShareFacebook.LOGIN_TYPE_WRITE) {
					shareContent();
				}
			}

			@Override
			public void onError(FacebookException exception) {
				setError(ERROR_CODE_UNKOWN_EXCEPTION, exception.getLocalizedMessage());
				if (mCurrentLoginType == ShareFacebook.LOGIN_TYPE_READ) {
					AccessToken.setCurrentAccessToken(null);
					mDictinaryData.clear();
					sendLoginFailed();
					mIsLoginSuccess = false;
				} else if (mCurrentLoginType == ShareFacebook.LOGIN_TYPE_WRITE) {
					shareCancele();
				}
			}

			@Override
			public void onCancel() {
				setError(ERROR_CODE_CANCEL, "");
				if (mCurrentLoginType == ShareFacebook.LOGIN_TYPE_READ) {
					AccessToken.setCurrentAccessToken(null);
					mDictinaryData.clear();
					sendLoginFailed();
					setError(1, "userCancel");
					mIsLoginSuccess = false;
				} else if (mCurrentLoginType == ShareFacebook.LOGIN_TYPE_WRITE) {
					shareContent();
				}

			}
		});
		new ProfileTracker() {
			@Override
			protected void onCurrentProfileChanged(com.facebook.Profile oldProfile,
					com.facebook.Profile currentProfile) {
			}
		};
	}

	public void debugLog(String msg) {
		Log.e("FaceBook", msg);
	}

	public void getUserInformation() {
		if (null == AccessToken.getCurrentAccessToken()) {
			return;
		}
		if (true == mIsGettingEmail) {
			return;
		}
		mIsGettingEmail = true;
		GraphRequest request = GraphRequest.newMeRequest(AccessToken.getCurrentAccessToken(),
				new GraphRequest.GraphJSONObjectCallback() {
					@Override
					public void onCompleted(JSONObject object, GraphResponse response) {
						mIsGettingEmail = false;
						try {
							mIsLoginSuccess = true;
							AccessToken currentAccessToken = AccessToken.getCurrentAccessToken();
							String email = "";
							if (object.has("email")) {
								email = object.getString("email");
							} else if (object.has("name")) {
								email = object.getString("name");
							} else {
								email = object.getString("id");
							}
							String name = object.getString("name");
							String id = object.getString("id");
							String link = object.getString("link");
							String locale = object.getString("locale");
							String token = currentAccessToken.getToken();
							mDictinaryData.put("name", name);
							mDictinaryData.put("email", email);
							mDictinaryData.put("id", id);
							mDictinaryData.put("link", link);
							mDictinaryData.put("local", locale);
							mDictinaryData.put("token", token);
							if (true == mIsInLoginProcess) {
								sendLoginSuccess();
							}
						} catch (Throwable e) {
							setError(ERROR_CODE_UNKOWN_EXCEPTION, e.getLocalizedMessage());
							sendLoginFailed();
							logout();
						}
						mIsInLoginProcess = false;
					}
				});
		request.executeAsync();
	}

	public void setDebugMode(boolean debug) {

	}

	public String getSDKVersion() {
		return "";
	}

	public String getPluginVersion() {
		return "";
	}

	public void sendLoginSuccess() {
		if (true == mIsGamePaused) {
			mPendingAction = PENDING_ACTION_LOGIN_SUCCESS;
			return;
		}
		PluginWrapper.runOnMainThread(new Runnable() {
			public void run() {
				ShareWrapper.onShareResult(mAdapter, ShareWrapper.SHARERESULT_LOGIN_SUCCESS, "");
			}
		});
		mPendingAction = 0;
	}

	public void sendLoginFailed() {
		if (true == mIsGamePaused) {
			mPendingAction = PENDING_ACTION_LOGIN_FAILED;
			return;
		}
		PluginWrapper.runOnMainThread(new Runnable() {
			public void run() {
				ShareWrapper.onShareResult(mAdapter, ShareWrapper.SHARERESULT_LOGIN_FAILED, "");
			}
		});
		mPendingAction = 0;
	}

	public void onPause() {
		mIsGamePaused = true;
		try {
			AppEventsLogger.deactivateApp(mContext);
		} catch (Exception e) {

		}
	}

	public void onResume() {
		try {
			AppEventsLogger.activateApp(mContext);
			mIsGamePaused = false;
			if (PENDING_ACTION_LOGIN_SUCCESS == mPendingAction) {
				sendLoginSuccess();
				mPendingAction = 0;
			}
			if (PENDING_ACTION_LOGIN_FAILED == mPendingAction) {
				sendLoginFailed();
				mPendingAction = 0;
			}
		} catch (Exception e) {

		}
	}

	public void login() {
		setError(0, "");
		if (true == mIsLoginSuccess) {
			sendLoginSuccess();
			return;
		}
		mIsInLoginProcess = true;
		if (true == isLoggedIn()) {
			PluginWrapper.runOnMainThread(new Runnable() {
				public void run() {
					getUserInformation();
				}
			});
			return;
		}
		final LoginManager manager = LoginManager.getInstance();
		manager.setLoginBehavior(LoginBehavior.SSO_WITH_FALLBACK);
		mCurrentLoginType = ShareFacebook.LOGIN_TYPE_READ;
		manager.logInWithReadPermissions((Activity) mContext, mPermissions);
		if (null == mAccessTokenTracker) {
			mAccessTokenTracker = new AccessTokenTracker() {
				@Override
				protected void onCurrentAccessTokenChanged(AccessToken oldAccessToken, AccessToken currentAccessToken) {
					if (null != currentAccessToken) {
						getUserInformation();
						AccessToken.setCurrentAccessToken(currentAccessToken);
					} else {
						setError(ERROR_CODE_ACCESSTOKEN, "");
						mDictinaryData.clear();
						mIsLoginSuccess = false;
					}
				}
			};
		}
	}

	public void logout() {
		try {
			final LoginManager manager = LoginManager.getInstance();
			manager.logOut();
			mDictinaryData.clear();
			mIsLoginSuccess = false;
		} catch (Throwable t) {

		}
	}

	private void gameRequestFailed(int errorCode, String errorMessage) {
		setError(errorCode, errorMessage);
		ShareWrapper.onShareResult(mAdapter, ShareWrapper.SHARERESULT_GAME_REQUEST_FAILED, "");
	}

	@SuppressLint("DefaultLocale")
	public void sendGameRequest(final JSONObject shareParams) {
		if (null == shareParams) {
			gameRequestFailed(SHARE_NORMAL_ERROR, "parameter is error");
			return;
		}
		try {
			String title = null;
			if( shareParams.has("title")){
				title = shareParams.getString("title");
			}
			String message = null;
			if( shareParams.has("message")){
				message = shareParams.getString("message");
			}
			String objectID = null;
			if( shareParams.has("objectID")){
				objectID = shareParams.getString("objectID");
			}
			String actionType = null;
			if( shareParams.has("actionType")){
				actionType = shareParams.getString("actionType");
			}
			String to = null;
			if( shareParams.has("to")){
				to = shareParams.getString("to");
			}
			String data = null;
			if( shareParams.has("data")){
				data = shareParams.getString("data");
			}
			if (null == mGameRequestDialog) {
				mGameRequestDialog = new GameRequestDialog((Activity) mContext);
				mGameRequestDialog.registerCallback(mCallbackManager, new FacebookCallback<GameRequestDialog.Result>() {
					@Override
					public void onCancel() {
						setError(0, "");
						ShareWrapper.onShareResult(mAdapter, ShareWrapper.SHARERESULT_GAME_REQUEST_CANCEL, "");
					}
					@Override
					public void onError(FacebookException error) {
						gameRequestFailed(SHARE_NORMAL_ERROR, error.toString());
					}

					@Override
					public void onSuccess(GameRequestDialog.Result result) {
						setError(0, "");
						mDictinaryData.put("appRequests", result.getRequestId());
						ShareWrapper.onShareResult(mAdapter, ShareWrapper.SHARERESULT_GAME_REQUEST_SUCCESS, "");
					}
				});
			}
			GameRequestContent.Builder builder = new GameRequestContent.Builder();
			if (title != null && title.length() > 0) {
				builder.setTitle(title);
			}
			if (message != null && message.length() > 0) {
				builder.setMessage(message);
			}
			if (objectID != null && objectID.length() > 0) {
				builder.setObjectId(objectID);
			}
			if (to != null && to.length() > 0) {
				builder.setTo(to);
			}
			if (to != null && to.length() > 0) {
				builder.setTo(to);
			}
			if (data != null && data.length() > 0) {
				builder.setData(data);
			}
			if (actionType != null && actionType.length() > 0) {
				actionType = actionType.toLowerCase();
				if (actionType.equals("send")) {
					builder.setActionType(ActionType.SEND);
				} else if (actionType.equals("askfor")) {
					builder.setActionType(ActionType.ASKFOR);
				}
			}
			GameRequestContent content = builder.build();
			if (mGameRequestDialog.canShow(content)) {
				mGameRequestDialog.show(content);
			} else {
				setError(SHARE_NORMAL_ERROR, "can't show the dialog");
				ShareWrapper.onShareResult(mAdapter, ShareWrapper.SHARERESULT_GAME_REQUEST_FAILED, "");
			}
		} catch (Throwable e) {
			setError(SHARE_NORMAL_ERROR, e.getMessage());
			ShareWrapper.onShareResult(mAdapter, ShareWrapper.SHARERESULT_GAME_REQUEST_FAILED, "");
		}
	}

	private void graphRequestFailed(int errorCode, String errorString) {
		setError(errorCode, errorString);
		ShareWrapper.onShareResult(mAdapter, ShareWrapper.SHARERESULT_GRAPH_REQUEST_FAILED, "");
	}

	public void doGraphRequest(final JSONObject params) {
		final AccessToken accesstoken = AccessToken.getCurrentAccessToken();
		PluginWrapper.runOnMainThread(new Runnable() {
			public void run() {
				try {
					String httpMethod = params.getString("Param1");
					String action = params.getString("Param2");
					JSONObject bundleParams = params.getJSONObject("Param3");
					if (null != httpMethod && action != null) {
						doGraphRequest(httpMethod, action, bundleParams, accesstoken);
					} else {
						graphRequestFailed(SHARE_NORMAL_ERROR, "parameter is error");
					}
				} catch (Throwable e) {
					graphRequestFailed(SHARE_NORMAL_ERROR, e.getLocalizedMessage());
				}
			}
		});
	}

	@SuppressLint("DefaultLocale")
	public void doGraphRequest(String httpMethod, String action, JSONObject params, AccessToken accesstoken) {
		try {
			httpMethod = httpMethod.toLowerCase();
			HttpMethod hm = HttpMethod.DELETE;
			if (httpMethod.equals("get")) {
				hm = HttpMethod.GET;
			} else if (httpMethod.equals("post")) {
				hm = HttpMethod.POST;
			} else if (httpMethod.equals("delete")) {
				hm = HttpMethod.DELETE;
			}
			Bundle paramsBundle = null;
			if (null != params && params.length() > 0) {
				paramsBundle = new Bundle();
				Iterator<?> it = params.keys();
				while (it.hasNext()) {
					String key = (String) it.next().toString();
					if (null != key && !"".equals(key)) {
						String value = params.getString(key);
						paramsBundle.putString(key, value);
					}
				}
			}
			GraphRequest request = new GraphRequest(accesstoken, action, paramsBundle, hm);
			request.setCallback(new com.facebook.GraphRequest.Callback() {
				@Override
				public void onCompleted(GraphResponse response) {
					if (response.getError() == null) {
						setError(0, "");
						mDictinaryData.put("graphRequestResult", response.getRawResponse());
						ShareWrapper.onShareResult(mAdapter, ShareWrapper.SHARERESULT_GRAPH_REQUEST_SUCCESS, "");
					} else {
						graphRequestFailed(response.getError().getErrorCode(), response.getError().getErrorMessage());
					}
				}
			});
			request.executeAsync();
			
		} catch (Throwable e) {
			graphRequestFailed(SHARE_NORMAL_ERROR, e.getLocalizedMessage());
		}
	}

	public void getInvitableFriends() {
		Hashtable<String, String> params = new Hashtable<String, String>();
		params.put("title", "title");
		params.put("message", "message");
		params.put("objectID", "848165281918938");
		params.put("actionType", "send");
		params.put("data", "[\"roleid\":\"20\",\"test\":\"true\"]");
		//sendGameRequest(params);

		GraphRequest request2 = new GraphRequest(AccessToken.getCurrentAccessToken(),
				"110455775960367_1381605175469856", null, HttpMethod.DELETE);
		request2.setCallback(new com.facebook.GraphRequest.Callback() {

			@Override
			public void onCompleted(GraphResponse response) {
				if (response.getError() == null) {

				} else {
				}
			}
		});
		request2.executeAsync();

		/*
		 * GraphRequest request = new GraphRequest(
		 * AccessToken.getCurrentAccessToken(), "me/invitable_friends", null,
		 * HttpMethod.GET); request.setCallback(new
		 * com.facebook.GraphRequest.Callback() {
		 * 
		 * @Override public void onCompleted(GraphResponse response) { if
		 * (response.getError() == null) {
		 * 
		 * } else { } } }); request.executeAsync();
		 */

		// AVnzGW17HfvSIUjQEc8fRYOH8G75nqBW0IWpWThZHwYovEVR9UV9D93T0BWEdKDuC-
		// _Yl9O_inRNNfcmTOQeG2dGnfeBVyLapvFLddBqoarx1g

		// /340531736127258,1386318725004581
		/*
		 * GraphRequest request = new GraphRequest(
		 * AccessToken.getCurrentAccessToken(), "me/friends", null,
		 * HttpMethod.GET); request.setCallback(new
		 * com.facebook.GraphRequest.Callback() {
		 * 
		 * @Override public void onCompleted(GraphResponse response) { if
		 * (response.getError() == null) {
		 * 
		 * } else { } } }); request.executeAsync();
		 */
		/*
		 * Bundle params2 = new Bundle(); params2.putString("title",
		 * "this is title"); params2.putString("message", "this is message");
		 * params2.putString("object_id", "848165281918938");
		 * params2.putString("action_type", "send"); params2.putString("to",
		 * "340531736127258,1386318725004581"); GraphRequest request = new
		 * GraphRequest( AccessToken.getCurrentAccessToken(), "apprequests",
		 * params2, HttpMethod.POST); request.setCallback(new
		 * com.facebook.GraphRequest.Callback() {
		 * 
		 * @Override public void onCompleted(GraphResponse response) { if
		 * (response.getError() == null) {
		 * 
		 * } else { } } }); request.executeAsync();
		 */
	}

	public void getFriends() {

	}

	public void inviteFriends(Hashtable<String, String> params) {

	}

	public void getAppRequest() {

	}

	@SuppressLint("InflateParams")
	public void Like(JSONObject jsonObject) {
		try {
			final String m_title = jsonObject.getString("title");
			final String m_content = jsonObject.getString("content");
			final String url = jsonObject.getString("url");

			mHandler = new Handler(mContext.getMainLooper());
			mHandler.post(new Runnable() {

				@Override
				public void run() {
					View v = LayoutInflater.from(mContext).inflate(R.layout.facebooklike, null);

					LikeView likeView = (LikeView) v.findViewById(R.id.like_view);
					// Set the object for which you want to get likes from your
					// users (Photo, Link or even your FB Fan page)
					// likeView.setObjectId("https://www.facebook.com/AndroidProgrammerGuru");
					likeView.setObjectIdAndType(url, ObjectType.PAGE);
					// likeView.setForegroundColor(-256);

					TextView title = (TextView) v.findViewById(R.id.fb_title);
					TextView content = (TextView) v.findViewById(R.id.fb_content);

					title.setText(m_title);
					content.setText(m_content);

					ImageView iv = (ImageView) v.findViewById(R.id.fb_close);

					// new
					// AlertDialog.Builder(mContext,R.style.dialog).setView(v).create().show();
					final AlertDialog dialog = new AlertDialog.Builder(mContext).create();
					dialog.show();
					Window window = dialog.getWindow();
					window.setContentView(v);

					iv.setOnClickListener(new OnClickListener() {

						@Override
						public void onClick(View v) {
							dialog.dismiss();
						}
					});
				}
			});
		} catch (JSONException e) {
			e.printStackTrace();
		}
	}

	public String getValue(String key) {
		if (null != mDictinaryData && mDictinaryData.containsKey(key)) {
			return mDictinaryData.get(key);
		}
		return "";
	}

	private void setError(int errorCode, String errorMsg) {
		mDictinaryData.put("errorCode", String.format("%d", errorCode));
		if (null != errorMsg) {
			mDictinaryData.put("errorMsg", errorMsg);
			debugLog(errorMsg);
		}
	}

	public void onActivityResult(int requestCode, int resultCode, Intent intent) {
		Log.d(TAG, "activity result in plugin: requestCode(" + requestCode + "), resultCode(" + resultCode + ")");
		mCallbackManager.onActivityResult(requestCode, resultCode, intent);

		mTrackingPendingCall = false;

	}

	public void logEvent(JSONObject eventParams) {
		if (null == eventParams) {
			return;
		}
		if (null == mAppEventsLogger) {
			return;
		}
		try {
			String type = eventParams.getString("type");
			String event = eventParams.getString("body");
			String value = eventParams.getString("value");
			String currency = eventParams.getString("currencyCode");
			double money = 0.0;
			try {
				money = Double.parseDouble(value);
			} catch (Exception e) {

			}
			if (type.equals("Purchase")) {
				mAppEventsLogger.logPurchase(BigDecimal.valueOf(money), Currency.getInstance(currency));
			} else {
				mAppEventsLogger.logEvent(event);
			}
		} catch (Exception e) {

		}
	}

	private boolean isLoggedIn() {
		AccessToken accessToken = AccessToken.getCurrentAccessToken();
		if (null == accessToken) {
			return false;
		}
		if (accessToken.isExpired()) {
			return false;
		}
		return true;
	}

	private boolean hasPublishPermission() {
		AccessToken accessToken = AccessToken.getCurrentAccessToken();
		if (null == accessToken) {
			return false;
		}
		if (accessToken.isExpired()) {
			return false;
		}
		return accessToken.getPermissions().contains("publish_actions");
	}

	public void shareContent() {
		if (null == mShareDictionary) {
			shareCancele();
			debugLog("ShareDictionary is empty");
			return;
		}
		try {
			String title = mShareDictionary.get("title");
			String link = mShareDictionary.get("link");
			String desc = mShareDictionary.get("description");
			String url = mShareDictionary.get("url");
			String shareTypeString = mShareDictionary.get("shareType");
			int shareType = Integer.parseInt(shareTypeString);
			boolean isForceShowDialog = false;
			try {
				isForceShowDialog = Boolean.parseBoolean(mShareDictionary.get("isForceShowDialog"));
			} catch (Throwable e) {

			}
			if (true == hasPublishPermission() && false == isForceShowDialog) {
				shareSilence(url, title, link, desc, shareType);
			} else {
				shareWithDialog(url, title, link, desc, shareType);
			}
		} catch (Throwable e) {
			debugLog(e.getMessage());
			shareCancele();
		}
	}

	private ShareDialog mShareDialog = null;
	private FacebookCallback<Sharer.Result> shareCallback = new FacebookCallback<Sharer.Result>() {
		@Override
		public void onCancel() {
			setError(ERROR_CODE_CANCEL, "");
			shareCancele();
		}

		@Override
		public void onError(FacebookException error) {
			setError(ERROR_CODE_UNKOWN_EXCEPTION, error.getLocalizedMessage());
			shareCancele();
		}

		@Override
		public void onSuccess(Sharer.Result result) {
			ShareWrapper.onShareResult(mAdapter, ShareWrapper.SHARERESULT_SUCCESS, "");
		}
	};

	private void shareWithDialog(String url, String title, String link, String desc, int shareType) {
		try {
			if (null == mShareDialog) {
				mShareDialog = new ShareDialog((Activity) mContext);
				mShareDialog.registerCallback(mCallbackManager, shareCallback);
			}
			if (shareType == SHARE_TYPE_VIDEO) {
				ShareLinkContent shareContent = new ShareLinkContent.Builder().setContentTitle(title)
						.setContentDescription(desc).setContentUrl(Uri.parse(url)).build();
				if (mShareDialog.canShow(shareContent)) {
					mShareDialog.show(shareContent);
				} else {
					shareCancele();
					debugLog("can't show content");
				}
			} else if (shareType == SHARE_TYPE_LOCAL_IMAGE) {
				SharePhotoContent shareContent = buildSharePhotoContent(url, title, link, desc);
				if (mShareDialog.canShow(shareContent)) {
					mShareDialog.show(shareContent);
				} else {
					setError(SHARE_NO_NATIVE_FACEBOOK, "");
					shareCancele();
					debugLog("can't show content");
				}
			} else if (shareType == SHARE_TYPE_ONLINE_IMAGE) {
				ShareLinkContent shareContent = buildShareLinkContent(url, title, link, desc);
				if (mShareDialog.canShow(shareContent)) {
					mShareDialog.show(shareContent);
				} else {
					shareCancele();
					debugLog("can't show content");
				}
			}
		} catch (Throwable e) {
			shareCancele();
		}
	}

	private SharePhotoContent buildSharePhotoContent(String linkContent, String title, String link, String desc) {
		try {
			Bitmap bitmap = BitmapFactory.decodeFile(linkContent);
			final SharePhoto photo = new SharePhoto.Builder().setUserGenerated(false).setBitmap(bitmap).build();
			ArrayList<SharePhoto> photos = new ArrayList<SharePhoto>() {
				private static final long serialVersionUID = 1L;

				{
					add(photo);
				}
			};
			SharePhotoContent imageContent = new SharePhotoContent.Builder().setPhotos(photos).build();
			return imageContent;
		} catch (Throwable e) {

		}
		return null;
	}

	private ShareLinkContent buildShareLinkContent(String linkContent, String title, String link, String desc) {
		try {
			ShareLinkContent.Builder builder = new ShareLinkContent.Builder().setContentTitle(title)
					.setContentDescription(desc).setContentUrl(Uri.parse(linkContent));
			return builder.build();
		} catch (Throwable e) {
			e.printStackTrace();
		}
		return null;
	}

	private void shareSilence(String url, String title, String link, String desc, int shareType) {
		try {
			if (shareType == SHARE_TYPE_VIDEO) {
				Bundle params2 = new Bundle();
				params2.putString("link", url);
				GraphRequest request = new GraphRequest(AccessToken.getCurrentAccessToken(), "me/feed", params2,
						HttpMethod.POST);
				request.setCallback(new com.facebook.GraphRequest.Callback() {

					@Override
					public void onCompleted(GraphResponse response) {
						if (response.getError() == null) {
							ShareWrapper.onShareResult(mAdapter, ShareWrapper.SHARERESULT_SUCCESS, "");
						} else {
							shareCancele();
						}
					}
				});
				request.executeAsync();
			} else if (shareType == SHARE_TYPE_ONLINE_IMAGE) {
				Bundle params2 = new Bundle();
				params2.putString("link", url);
				params2.putString("caption", title);
				params2.putString("description", desc);
				GraphRequest request = new GraphRequest(AccessToken.getCurrentAccessToken(), "me/feed", params2,
						HttpMethod.POST);
				request.setCallback(new com.facebook.GraphRequest.Callback() {

					@Override
					public void onCompleted(GraphResponse response) {
						if (response.getError() == null) {
							ShareWrapper.onShareResult(mAdapter, ShareWrapper.SHARERESULT_SUCCESS, "");
						} else {
							shareCancele();
						}
					}
				});
				request.executeAsync();
			} else if (shareType == SHARE_TYPE_LOCAL_IMAGE) {
				ShareApi.share(buildSharePhotoContent(url, title, link, desc), shareCallback);
			}
		} catch (Throwable e) {
			shareCancele();
		}
	}

	public void shareCancele() {
		ShareWrapper.onShareResult(mAdapter, ShareWrapper.SHARERESULT_FAIL, "");
	}

	public void share(Hashtable<String, String> shareParams) {
		setError(0, "");
		mShareDictionary = shareParams;
		if (true == hasPublishPermission()) {
			PluginWrapper.runOnMainThread(new Runnable() {
				public void run() {
					shareContent();
				}
			});
			return;
		}
		boolean isSilence = false;
		try {
			if (true == mShareDictionary.containsKey("isSilence")) {
				isSilence = Boolean.parseBoolean(mShareDictionary.get("isSilence"));
			}
		} catch (Throwable e) {

		}
		if (false == isLoggedIn() && true == isSilence) {
			shareCancele();
			return;
		}
		if (false == hasPublishPermission() && true == isSilence) {
			shareCancele();
			return;
		}

		SharedPreferences sp = getSharedPreferences();
		if (null == sp) {
			shareCancele();
			return;
		}
		if (sp.getInt("FACEBOOK_ASKED", 0) >= 1) {
			shareContent();
			return;
		}
		sp.edit().putInt("FACEBOOK_ASKED", 1);
		sp.edit().commit();
		mCurrentLoginType = ShareFacebook.LOGIN_TYPE_WRITE;
		final LoginManager manager = LoginManager.getInstance();
		manager.setDefaultAudience(DefaultAudience.FRIENDS);
		manager.setLoginBehavior(LoginBehavior.SSO_WITH_FALLBACK);
		manager.logInWithPublishPermissions((Activity) mContext, Arrays.asList("publish_actions"));
	}

	private SharedPreferences getSharedPreferences() {
		return PluginWrapper.getContext().getSharedPreferences(PluginWrapper.getContext().getPackageName(),
				Context.MODE_PRIVATE);
	}
}
