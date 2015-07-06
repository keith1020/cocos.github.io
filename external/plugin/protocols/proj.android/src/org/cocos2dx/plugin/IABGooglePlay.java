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

import java.util.ArrayList;
import java.util.Currency;
import java.util.HashMap;
import java.util.Hashtable;
import java.util.List;
import java.util.Locale;
import java.util.Map;
import java.util.UUID;

import org.cocos2dx.plugin.util.IabHelper;
import org.cocos2dx.plugin.util.IabResult;
import org.cocos2dx.plugin.util.Inventory;
import org.cocos2dx.plugin.util.Purchase;
import org.cocos2dx.plugin.util.SkuDetails;

import android.annotation.SuppressLint;
import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.util.Base64;

@SuppressLint("NewApi")
public class IABGooglePlay implements InterfaceIAP {

	// Debug tag, for logging
	static final String TAG = "IABGooglePlay";

	// (arbitrary) request code for the purchase flow
	static final int RC_REQUEST = 10001;
	IabHelper mIabHelper;
	IABGooglePlay mAdapter;
	Context mContext;
	ArrayList<String> mProductIdentifiers;
	String mPublicKey;
	String mZoneID;
	String mAccountID;
	String mPayTag;
	String mCurrencyCode = "";
	boolean mOsInComsumeState = false;
	boolean mIsQueryInventory = false;
	List<Purchase> mPurchaseListWaitToConsum = new ArrayList<Purchase>();
	HashMap<String, String> mDictinaryData = new HashMap<String, String>();
	HashMap<String, Integer> mPriceData = new HashMap<String, Integer>();

	public IABGooglePlay(Context context) {
		mContext = context;
	}

	public void debugLog(String s) {
		// /Log.d(TAG, s);
	}

	private void setErrorMessage(String error) {
		mDictinaryData.put("errorMsg", error);
	}

	private void payFailed(final int errorCode, final String errorMessage) {
		setErrorMessage(errorMessage);
		if (errorCode == IabHelper.BILLING_RESPONSE_RESULT_USER_CANCELED
				|| errorCode == IabHelper.IABHELPER_USER_CANCELLED) {
			IAPWrapper.onPayResult(mAdapter, IAPWrapper.PAYRESULT_CANCEL,
					errorMessage);
		} else {
			IAPWrapper.onPayResult(mAdapter, IAPWrapper.PAYRESULT_FAIL,
					errorMessage);
		}
	}

	@Override
	public void configDeveloperInfo(Hashtable<String, String> cpInfo) {
		// TODO Auto-generated method stub
		String publicKey = cpInfo.get("publicKey");
		String zoneID = cpInfo.get("zoneID");
		String accountID = cpInfo.get("accountID");
		mZoneID = zoneID;
		mAccountID = accountID;
		mPublicKey = publicKey;

		Runnable runnable = new Runnable() {
			@Override
			public void run() {
				mIabHelper = new IabHelper(PluginWrapper.getContext(),
						mPublicKey);
				mIabHelper
						.startSetup(new IabHelper.OnIabSetupFinishedListener() {
							public void onIabSetupFinished(IabResult result) {
								if (!result.isSuccess()) {
									return;
								}
								if (false == mIsQueryInventory) {
									mIsQueryInventory = true;
									try {
										mIabHelper.queryInventoryAsync(true,
												mProductIdentifiers,
												mGotInventoryListener);
									} catch (Throwable e) {
									}
								}
							}
						});
			}
		};
		PluginWrapper.runOnMainThread(runnable);
		Currency currency = Currency.getInstance(Locale.getDefault());
		mCurrencyCode = currency.getCurrencyCode();
		mAdapter = this;
	}

	// Listener that's called when we finish querying the items we own
	IabHelper.QueryInventoryFinishedListener mGotInventoryListener = new IabHelper.QueryInventoryFinishedListener() {
		@Override
		public void onQueryInventoryFinished(IabResult result,
				Inventory inventory) {
			mIsQueryInventory = false;
			debugLog("Query inventory finished.");
			if (result.isFailure()) {
				debugLog("Failed to query inventory: " + result);
				setErrorMessage(result.getMessage());
				IAPWrapper.onPayResult(mAdapter,
						IAPWrapper.GETPRODUCTINFO_FAILED, result.getMessage());
				return;
			}
			if (null != mProductIdentifiers) {
				for (int i = 0; i < mProductIdentifiers.size(); i++) {
					String id = mProductIdentifiers.get(i);
					SkuDetails sDetail = inventory.getSkuDetails(id);
					if (null != sDetail) {
						mCurrencyCode = sDetail.getCurrencyCode();
						mDictinaryData
								.put(sDetail.getSku(), sDetail.getPrice());
						mPriceData.put(sDetail.getSku(),
								sDetail.getPriceAmountMicros() / 10000);
						Purchase p = inventory.getPurchase(sDetail.getSku());
						if (null != p) {
							savePurchase(p, result);
						}
					}
				}
			}
			IAPWrapper.onPayResult(mAdapter, IAPWrapper.GETPRODUCTINFO_SUCCESS,
					"");
		}
	};

	private void ConsumeProduct(Purchase p) {
		if (true == mOsInComsumeState) {
			mPurchaseListWaitToConsum.add(p);
			return;
		}
		if (null != p && null != mConsumeFinishedListener) {
			mIabHelper.consumeAsync(p, mConsumeFinishedListener);
			mOsInComsumeState = true;
		}
	}

	// Called when consumption is complete
	IabHelper.OnConsumeFinishedListener mConsumeFinishedListener = new IabHelper.OnConsumeFinishedListener() {
		@Override
		public void onConsumeFinished(Purchase purchase, IabResult result) {
			if (result.isSuccess()) {
				IAPWrapper.onPayResult(mAdapter,
						IAPWrapper.PAYRESULT_COMSUME_SUCCESS, "");
			} else {
				setErrorMessage(result.getMessage());
				IAPWrapper.onPayResult(mAdapter,
						IAPWrapper.PAYRESULT_COMSUME_FAILED, "");
			}
			String uuid = getPurchaseUUIDByPurchase(purchase);
			SharedPreferences sp = getSharedPreferences();
			if (uuid != null && null != sp) {
				sp.edit().remove(uuid).commit();
			}
			mOsInComsumeState = false;
			// /!在还没有消费的列表中，提取第一项，进行消费
			if (null != mPurchaseListWaitToConsum
					&& mPurchaseListWaitToConsum.size() > 0) {
				Purchase waitToComsume = mPurchaseListWaitToConsum.get(0);
				mPurchaseListWaitToConsum.remove(0);
				mIabHelper
						.consumeAsync(waitToComsume, mConsumeFinishedListener);
			}
		}
	};

	private Purchase getPurchaseByUUID(String uuid) {
		SharedPreferences sp = getSharedPreferences();
		if (null == sp) {
			return null;
		}
		String value = sp.getString(uuid, "");
		if (null == value) {
			return null;
		}
		try {
			value = AESencrp.decrypt(value);
			value = new String(Base64.decode(value, Base64.DEFAULT));
			String[] v = value.split(";");
			if (null != v && v.length > 2) {
				String json = v[0];
				String sig = v[1];
				return new Purchase(json, sig);
			}
		} catch (Exception e) {

		}
		return null;
	}

	private String getPurchaseUUIDByPurchase(Purchase p) {
		SharedPreferences sp = getSharedPreferences();
		if (null == sp) {
			return null;
		}
		Map<String, ?> allTracations = sp.getAll();
		try {
			debugLog("allTracations size = " + allTracations.size());
			for (Map.Entry<String, ?> entry : allTracations.entrySet()) {
				debugLog("key = " + entry.getKey());
				String value = (String) entry.getValue();
				value = AESencrp.decrypt(value);
				value = new String(Base64.decode(value, Base64.DEFAULT));
				String[] v = value.split(";");
				if (null != v && v.length > 2) {
					String json = v[0];
					String sig = v[1];
					if (json.equalsIgnoreCase(p.getOriginalJson())
							&& sig.equalsIgnoreCase(p.getSignature())) {
						return entry.getKey();
					}
				}
			}
		} catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
			debugLog("decrypt error : " + e.toString());
		}
		return null;
	}

	private void savePurchase(Purchase purchase, IabResult result) {
		try {
			int price = 0;
			String purchaseUDID = UUID.randomUUID().toString();
			String purchaseData = purchase.getOriginalJson() + ";";
			purchaseData = purchaseData + purchase.getSignature() + ";";
			purchaseData = purchaseData + result.getResponse() + ";";
			purchaseData = purchaseData + mZoneID + ";";
			purchaseData = purchaseData + mAccountID + ";";
			purchaseData = purchaseData + mPayTag + ";";
			purchaseData = purchaseData + price + ";";
			purchaseData = purchaseData + mCurrencyCode;
			SharedPreferences sp = getSharedPreferences();
			purchaseData = new String(Base64.encodeToString(
					purchaseData.getBytes(), Base64.DEFAULT));
			String value = AESencrp.encrypt(purchaseData);
			sp.edit().putString(purchaseUDID, value).commit();
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	@Override
	public void payForProduct(Hashtable<String, String> cpInfo) {
		class purchaseRunnable implements Runnable {
			Hashtable<String, String> mcpInfo;

			public purchaseRunnable(Hashtable<String, String> info) {
				mcpInfo = info;
			}

			public void run() {
				// TODO Auto-generated method stub
				String productID = mcpInfo.get("productId");
				mPayTag = mcpInfo.get("payTag");

				debugLog("mPayTag = " + mPayTag);

				String extraData = mcpInfo.get("extraData");
				mIabHelper.launchPurchaseFlow(
						(Activity) PluginWrapper.getContext(), productID,
						RC_REQUEST, mPurchaseFinishedListener, extraData);
			}
		}
		if (mIabHelper.isSetupDone()) {
			if (true == mIsQueryInventory) {
				IAPWrapper.onPayResult(mAdapter, IAPWrapper.PAYRESULT_CANCEL,
						"");
				return;
			}
			PluginWrapper.runOnMainThread(new purchaseRunnable(cpInfo));
		} else {
			setErrorMessage("googleplay service is not available");
			IAPWrapper.onPayResult(mAdapter, IAPWrapper.PAYRESULT_FAIL, "");
		}
	}

	public void onActivityResult(int requestCode, int resultCode, Intent data) {
		debugLog("onActivityResult(" + requestCode + "," + resultCode + ","
				+ data);
		if (RC_REQUEST == requestCode) {
			mIabHelper.handleActivityResult(requestCode, resultCode, data);
		}
	}

	// Callback for when a purchase is finished
	IabHelper.OnIabPurchaseFinishedListener mPurchaseFinishedListener = new IabHelper.OnIabPurchaseFinishedListener() {
		public void onIabPurchaseFinished(IabResult result, Purchase purchase) {
			debugLog("Purchase finished: " + result + ", purchase: " + purchase);
			if (result.isFailure()) {
				payFailed(result.getResponse(), result.getMessage());
				return;
			}
			savePurchase(purchase, result);
			IAPWrapper.onPayResult(mAdapter, IAPWrapper.PAYRESULT_SUCCESS, "");
		}
	};

	private SharedPreferences getSharedPreferences() {
		return PluginWrapper.getContext().getSharedPreferences(
				PluginWrapper.getContext().getPackageName(),
				Context.MODE_PRIVATE);
	}

	public String getNoneVerifiedReceipt() {
		String result = "";
		try{
		SharedPreferences sp = getSharedPreferences();
		if (null == sp) {
			return result;
		}
		Map<String, ?> allTracations = sp.getAll();
		try {
			debugLog("allTracations size = " + allTracations.size());
			for (Map.Entry<String, ?> entry : allTracations.entrySet()) {

				debugLog("key = " + entry.getKey());

				String value = (String) entry.getValue();
				value = AESencrp.decrypt(value);
				result = result + entry.getKey() + ":";
				result = result + value + ";";

				debugLog("value = " + value);
			}
		} catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
			debugLog("decrypt error : " + e.toString());
		}
		debugLog("send to server : " + result);
		}catch(Throwable e){
			
		}
		return result;
	}

	public void removeNoneVerifiedReceipt(final String key) {
		PluginWrapper.runOnMainThread(new Runnable() {
			public void run() {
				try {
					Purchase p = getPurchaseByUUID(key);
					if (null != p) {
						ConsumeProduct(p);
					}
				} catch (Exception e) {
					e.printStackTrace();
				}
			}
		});
	}

	public void getProductsInformation(String products) {
		if (null == mIabHelper) {
			IAPWrapper.onPayResult(mAdapter, IAPWrapper.GETPRODUCTINFO_FAILED,
					"");
			return;
		}
		debugLog("try to get products " + products);

		String[] productIDs = products.split(",");
		if (null == productIDs) {
			IAPWrapper.onPayResult(mAdapter, IAPWrapper.GETPRODUCTINFO_FAILED,
					"");
			return;
		}
		mProductIdentifiers = new ArrayList<String>();
		for (int i = 0; i < productIDs.length; i++) {
			mProductIdentifiers.add(productIDs[i]);
		}
		if (mIabHelper.isSetupDone()) {
			if (false == mIsQueryInventory) {
				mIsQueryInventory = true;
				PluginWrapper.runOnMainThread(new Runnable() {
					public void run() {
						try {
							mIabHelper.queryInventoryAsync(true,
									mProductIdentifiers, mGotInventoryListener);
						} catch (Exception e) {
							IAPWrapper.onPayResult(mAdapter,
									IAPWrapper.GETPRODUCTINFO_FAILED, "");
						}
					}
				});
			}
		} else {
			IAPWrapper.onPayResult(mAdapter, IAPWrapper.GETPRODUCTINFO_FAILED,
					"");
		}
	}

	public String getValue(String key) {
		String ret = mDictinaryData.get(key);
		if (null == ret) {
			return "";
		}
		return ret;
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

}
