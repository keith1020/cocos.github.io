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

import java.util.Hashtable;

import com.google.android.gms.games.Games;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.util.Log;

public class SocialGooglePlay implements InterfaceSocial,
		SocialGooglePlayHelper.GameHelperListener {

	static final String TAG = "SocialGooglePlay";

	private Context mContext;
	private SocialGooglePlayHelper mSocialGooglePlayHelper;

	public SocialGooglePlay(Context context) {
		mContext = context;
		if (null == mSocialGooglePlayHelper) {
			mSocialGooglePlayHelper = new SocialGooglePlayHelper(
					(Activity) mContext, SocialGooglePlayHelper.CLIENT_GAMES
							| SocialGooglePlayHelper.CLIENT_PLUS);
		}
		mSocialGooglePlayHelper.setup(this, (Activity) mContext);
		mSocialGooglePlayHelper.setConnectOnStart(false);

	}

	public void onActivityResult(int requestCode, int responseCode,
			Intent intent) {
		mSocialGooglePlayHelper.onActivityResult(requestCode, responseCode,
				intent);
	}

	@Override
	public void configDeveloperInfo(Hashtable<String, String> cpInfo) {
		// TODO Auto-generated method stub
	}

	public void onStop() {
		// TODO Auto-generated method stub
		mSocialGooglePlayHelper.onStop();
	}

	@Override
	public void submitScore(final String leaderboardID, final String score) {
		try {
			if (mSocialGooglePlayHelper.isSignedIn()) {
				mSocialGooglePlayHelper.submitScore(leaderboardID,
						Long.parseLong(score));
			}
		} catch (Exception e) {

		}
	}

	@Override
	public void showLeaderboard(String leaderboardID) {
		// TODO Auto-generated method stub
		try {
			if (mSocialGooglePlayHelper.isSignedIn()) {
				mSocialGooglePlayHelper.showLeaderboard(leaderboardID);
			}
		} catch (Exception e) {

		}
	}

	@Override
	public void unlockAchievement(Hashtable<String, String> achInfo) {
		try {
			if (mSocialGooglePlayHelper.isSignedIn()) {
				mSocialGooglePlayHelper.unlockAchievement(achInfo);
			}
		} catch (Exception e) {

		}
	}

	@Override
	public void showAchievements() {
		// TODO Auto-generated method stub
		try {
			if (mSocialGooglePlayHelper.isSignedIn()) {
				mSocialGooglePlayHelper.showAchievements();
			}
		} catch (Exception e) {

		}
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

	@Override
	public void login() {
		PluginWrapper.runOnMainThread(new Runnable() {
			public void run() {
				mSocialGooglePlayHelper.beginUserInitiatedSignIn();
			}
		});
	}

	@Override
	public void logout() {
		// TODO Auto-generated method stub
		PluginWrapper.runOnMainThread(new Runnable() {
			public void run() {
				mSocialGooglePlayHelper.signOut();
			}
		});
	}
  
	@Override
	public void onSignInFailed() {
		SocialWrapper.onSocialResult(this, SocialWrapper.SOCIAL_LOGIN_FAILED,
				"");
	}

	@Override
	public void onSignInSucceeded(String accountID, String email, String token) {
		if (null == accountID || null == email || null == token) {
			SocialWrapper.onSocialResult(this,
					SocialWrapper.SOCIAL_LOGIN_FAILED, "");
			return;
		}
		// TODO Auto-generated method stub
		SocialWrapper.onSocialResult(this, SocialWrapper.SOCIAL_LOGIN_SUCCCESS,
				accountID + ";" + email + ";" + token);
	}
}
