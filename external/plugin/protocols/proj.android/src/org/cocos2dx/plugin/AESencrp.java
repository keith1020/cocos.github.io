package org.cocos2dx.plugin;

import java.security.*;
import javax.crypto.*;
import javax.crypto.spec.SecretKeySpec;
import android.annotation.SuppressLint;
import android.util.Base64;

@SuppressLint({ "NewApi", "TrulyRandom" })
class AESencrp {

	private static final String ALGO = "AES";
	private static final byte[] keyValue = new byte[] { 'm', 'o', 'o', 'n',
			't', 'o', 'n', 'A', 'G', 'a', 'm', 'e','1','2','3','4' };

	@SuppressLint("TrulyRandom")
	public static String encrypt(String Data) throws Exception {
		Key key = generateKey();
		Cipher c = Cipher.getInstance(ALGO);
		c.init(Cipher.ENCRYPT_MODE, key);
		byte[] encVal = c.doFinal(Data.getBytes());
		String encryptedValue = new String(Base64.encodeToString(encVal,
				Base64.DEFAULT));
		return encryptedValue;
	}

	public static String decrypt(String encryptedData) throws Exception {
		Key key = generateKey();
		Cipher c = Cipher.getInstance(ALGO);
		c.init(Cipher.DECRYPT_MODE, key);
		byte[] decordedValue = Base64.decode(encryptedData.getBytes(),
				Base64.DEFAULT);
		byte[] decValue = c.doFinal(decordedValue);
		String decryptedValue = new String(decValue);
		return decryptedValue;
	}

	private static Key generateKey() throws Exception {
		Key key = new SecretKeySpec(keyValue, ALGO);
		return key;
	}

}