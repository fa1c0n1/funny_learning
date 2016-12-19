package me.falcon.security.aes;

import java.security.Key;
import java.security.SecureRandom;
import java.security.Security;

import javax.crypto.Cipher;
import javax.crypto.KeyGenerator;
import javax.crypto.SecretKey;
import javax.crypto.spec.SecretKeySpec;

import org.apache.commons.codec.binary.Hex;
import org.bouncycastle.jce.provider.BouncyCastleProvider;

public class IFalconAES {

	private static final String DATA = "imooc security aes";
	
	public static void main(String[] args) {
		jdkAES();
		bcAES();
	}
	
	public static void jdkAES() {
		try {
			//生成KEY
			KeyGenerator keyGenerator = KeyGenerator.getInstance("AES");
			keyGenerator.init(128);
			SecretKey secretKey = keyGenerator.generateKey();
			byte[] keyBytes = secretKey.getEncoded();
			
			//KEY转换
			Key key = new SecretKeySpec(keyBytes, "AES");
			
			//加密
			Cipher cipher = Cipher.getInstance("AES/ECB/PKCS5Padding");
			cipher.init(Cipher.ENCRYPT_MODE, key);
			byte[] encryptRetBytes = cipher.doFinal(DATA.getBytes());
			System.out.println("JDK AES encrypt: " + Hex.encodeHexString(encryptRetBytes));
			
			//解密
			cipher.init(Cipher.DECRYPT_MODE, key);
			byte[] decryptRetBytes = cipher.doFinal(encryptRetBytes);
			System.out.println("JDK AES decrypt: " + new String(decryptRetBytes));
		} catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
	
	public static void bcAES() {
		try {
			Security.addProvider(new BouncyCastleProvider());
			
			//生成KEY
			KeyGenerator keyGenerator = KeyGenerator.getInstance("AES", "BC");
			keyGenerator.init(128);
			SecretKey secretKey = keyGenerator.generateKey();
			byte[] keyBytes = secretKey.getEncoded();
			
			//KEY转换
			Key key = new SecretKeySpec(keyBytes, "AES");
			
			//加密
			Cipher cipher = Cipher.getInstance("AES/ECB/PKCS5Padding");
			cipher.init(Cipher.ENCRYPT_MODE, key);
			byte[] encryptRetBytes = cipher.doFinal(DATA.getBytes());
			System.out.println(" BC AES encrypt: " + Hex.encodeHexString(encryptRetBytes));
			
			//解密
			cipher.init(Cipher.DECRYPT_MODE, key);
			byte[] decryptRetBytes = cipher.doFinal(encryptRetBytes);
			System.out.println(" BC AES decrypt: " + new String(decryptRetBytes));
		} catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
}
