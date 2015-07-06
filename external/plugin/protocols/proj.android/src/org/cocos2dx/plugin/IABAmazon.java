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

import java.util.HashMap;
import java.util.HashSet;
import java.util.Hashtable;
import java.util.List;
import java.util.Locale;
import java.util.Map;
import java.util.Set;
import java.util.UUID;

import com.amazon.device.iap.PurchasingListener;
import com.amazon.device.iap.PurchasingService;
import com.amazon.device.iap.model.ProductDataResponse;
import com.amazon.device.iap.model.ProductDataResponse.RequestStatus;
import com.amazon.device.iap.model.FulfillmentResult;
import com.amazon.device.iap.model.Product;
import com.amazon.device.iap.model.PurchaseResponse;
import com.amazon.device.iap.model.PurchaseUpdatesResponse;
import com.amazon.device.iap.model.Receipt;
import com.amazon.device.iap.model.RequestId;
import com.amazon.device.iap.model.UserData;
import com.amazon.device.iap.model.UserDataResponse;

import android.annotation.SuppressLint;
import android.content.Context;
import android.content.SharedPreferences;
import android.database.sqlite.SQLiteDatabase;
import android.database.sqlite.SQLiteOpenHelper;
import android.util.Base64;
import android.util.Log;

import java.util.Currency;

@SuppressLint("NewApi")
public class IABAmazon implements InterfaceIAP, PurchasingListener {

	// Debug tag, for logging
	static final String TAG = "IABAmazon";
	private IABAmazon mAdapter;
	private Context mContext;
	private HashMap<String, String> mDictinaryData = new HashMap<String, String>();
	private Set<String> mProductIdentifiers;
	private UserData mCurrentUserData = null;
	private boolean mSetupSuccess = false;
	private String mCurrencyCode = "";
	private String mZoneID;
	private String mAccountID;
	private String mPayTag;

	public class AmazonSQLiteHelper extends SQLiteOpenHelper {
		// table name
		public static final String TABLE_PURCHASES = "purchases";
		// receipt id
		public static final String COLUMN_RECEIPT_ID = "receipt_id";
		// amazon user id
		public static final String COLUMN_USER_ID = "user_id";
		// purchase record status
		public static final String COLUMN_STATUS = "status";
		// database name
		private static final String DATABASE_NAME = "receipts.db";
		private static final int DATABASE_VERSION = 1;
		// Database creation sql statement
		private static final String DATABASE_CREATE = "create table "
				+ TABLE_PURCHASES + "(" + COLUMN_RECEIPT_ID
				+ " text primary key not null, " + COLUMN_USER_ID
				+ " text not null, " + COLUMN_STATUS + " text not null " + ");";

		public AmazonSQLiteHelper(final Context context) {
			super(context, DATABASE_NAME, null, DATABASE_VERSION);
		}

		@Override
		public void onCreate(final SQLiteDatabase database) {
			database.execSQL(DATABASE_CREATE);
		}

		@Override
		public void onUpgrade(final SQLiteDatabase db, final int oldVersion,
				final int newVersion) {
		}
	}

	public IABAmazon(Context context) {
		mContext = context;
		mAdapter = this;
	}

	@Override
	public void configDeveloperInfo(Hashtable<String, String> cpInfo) {

		String zoneID = cpInfo.get("zoneID");
		String accountID = cpInfo.get("accountID");
		mZoneID = zoneID;
		mAccountID = accountID;
		PurchasingService.registerListener(mContext, this);
		PurchasingService.getUserData();
		PurchasingService.getPurchaseUpdates(true);
		Log.d(TAG, "IS_SANDBOX_MODE:" + PurchasingService.IS_SANDBOX_MODE);
	}

	@Override
	public void payForProduct(Hashtable<String, String> mcpInfo) {
		// TODO Auto-generated method stub
		String productID = mcpInfo.get("productId");
		mPayTag = mcpInfo.get("payTag");
		PurchasingService.purchase(productID);
	}

	public void onPause() {
	}

	public void onResume() {
		PurchasingService.getUserData();
		PurchasingService.getPurchaseUpdates(false);
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
	public void removeNoneVerifiedReceipt(String key) {
		debugLog("remove key : " + key);
		SharedPreferences sp = getSharedPreferences();
		if (null == sp) {
			return;
		}
		String value = sp.getString(key, "");
		String[] values = value.split(";");
		if (values.length > 2) {
			PurchasingService.notifyFulfillment(values[1],
					FulfillmentResult.FULFILLED);
		}
		sp.edit().remove(key).commit();
	}

	public void debugLog(String s) {
		Log.d(TAG, s);
	}

	public String getNoneVerifiedReceipt() {
		String result = "";
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
		return result;
	}

	public void getProductsInformation(String products) {
		debugLog("try to get products " + products);
		String[] productIDs = products.split(",");
		if (null == productIDs) {
			IAPWrapper.onPayResult(mAdapter, IAPWrapper.GETPRODUCTINFO_FAILED,
					"");
			return;
		}
		if (false == mSetupSuccess) {
			IAPWrapper.onPayResult(mAdapter, IAPWrapper.GETPRODUCTINFO_FAILED,
					"");
			return;
		}
		mProductIdentifiers = new HashSet<String>();
		for (int i = 0; i < productIDs.length; i++) {
			mProductIdentifiers.add(productIDs[i]);
		}
		PurchasingService.getProductData(mProductIdentifiers);
	}

	public String getValue(String key) {
		String ret = mDictinaryData.get(key);
		if (null == ret) {
			return "";
		}
		return ret;
	}

	@Override
	public void onProductDataResponse(ProductDataResponse response) {
		RequestStatus status = response.getRequestStatus();

		switch (status) {
		case SUCCESSFUL: {
			Map<String, Product> productData = response.getProductData();
			for (final String key : productData.keySet()) {
				Product p = productData.get(key);
				mDictinaryData.put(p.getSku(), p.getPrice());
			}
			IAPWrapper.onPayResult(mAdapter, IAPWrapper.GETPRODUCTINFO_SUCCESS,
					"");
			break;
		}
		case FAILED:
		case NOT_SUPPORTED: {
			setErrorMessage( response.toString() );
			IAPWrapper.onPayResult(mAdapter, IAPWrapper.GETPRODUCTINFO_FAILED,
					"");
			break;
		}
		}
	}

	public void saveAmazonReceipt(Receipt r, RequestId id) {
		if (null == mCurrentUserData) {
			return;
		}
		try {
			int price = 0;
			String purchaseUDID = UUID.randomUUID().toString();
			String purchaseData = r.getReceiptId() + ";";
			purchaseData = purchaseData + mCurrentUserData.getUserId() + ";";
			purchaseData = purchaseData + "" + ";";
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
			return;
		} catch (Exception e) {

		}
	}

	@Override
	public void onPurchaseResponse(PurchaseResponse response) {
		final PurchaseResponse.RequestStatus status = response
				.getRequestStatus();
		if (status != PurchaseResponse.RequestStatus.SUCCESSFUL) {
			setErrorMessage( response.toString() );
			IAPWrapper.onPayResult(mAdapter, IAPWrapper.PAYRESULT_FAIL, "");
			return;
		}
		saveAmazonReceipt(response.getReceipt(), response.getRequestId());
		IAPWrapper.onPayResult(mAdapter, IAPWrapper.PAYRESULT_SUCCESS, "");
	}

	@Override
	public void onPurchaseUpdatesResponse(PurchaseUpdatesResponse response) {
		final PurchaseUpdatesResponse.RequestStatus status = response
				.getRequestStatus();
		if (status != PurchaseUpdatesResponse.RequestStatus.SUCCESSFUL) {
			setErrorMessage( response.toString() );
			return;
		}
		List<Receipt> receipts = response.getReceipts();
		for (Receipt r : receipts) {
			saveAmazonReceipt(r, response.getRequestId());
		}
		if (response.hasMore()) {
            PurchasingService.getPurchaseUpdates(false);
        }
	}

	@SuppressLint("NewApi")
	public static String getCurrencyCode(String marketPlace) {
		final String DEFAULT_CURRENCY_CODE = "NO_CODE";
		if (marketPlace == null || marketPlace.isEmpty()) {
			return DEFAULT_CURRENCY_CODE;
		}
		try {
			Locale locale = new Locale("", marketPlace);
			return Currency.getInstance(locale).getCurrencyCode();
		} catch (IllegalArgumentException ex) {
		}
		return DEFAULT_CURRENCY_CODE;
	}

	@Override
	public void onUserDataResponse(UserDataResponse userDataResponse) {
		if (userDataResponse.getRequestStatus() == UserDataResponse.RequestStatus.SUCCESSFUL) {
			mCurrentUserData = userDataResponse.getUserData();
			mCurrencyCode = getCurrencyCode(mCurrentUserData.getMarketplace());
			mSetupSuccess = true;
		} else {
			mSetupSuccess = false;
			setErrorMessage( userDataResponse.toString() );
		}
	}

	private SharedPreferences getSharedPreferences() {
		return PluginWrapper.getContext().getSharedPreferences(
				PluginWrapper.getContext().getPackageName() + "AMAZON_STORE",
				Context.MODE_PRIVATE);
	}

	private void setErrorMessage(String error) {
		mDictinaryData.put("errorMsg", error);
	}

}
