package me.falcon.security.des;

import java.security.Key;
import java.security.SecureRandom;
import java.security.Security;

import javax.crypto.Cipher;
import javax.crypto.KeyGenerator;
import javax.crypto.SecretKey;
import javax.crypto.SecretKeyFactory;
import javax.crypto.spec.DESedeKeySpec;

import org.apache.commons.codec.binary.Hex;
import org.bouncycastle.jce.provider.BouncyCastleProvider;

public class IFalcon3DES {

	private static final String DATA = "imooc security 3des";
	
	public static void main(String[] args) {
		jdk3DES();
		bc3DES();
	}
	
	public static void jdk3DES() {
		try {
			//生成KEY
			KeyGenerator keyGenerator = KeyGenerator.getInstance("DESede");
//			keyGenerator.init(168);//or 112
			keyGenerator.init(new SecureRandom());//SecureRandom的作用是生成默认长度的key
			SecretKey secretKey = keyGenerator.generateKey();
			byte[] keyBytes = secretKey.getEncoded();
			
			//KEY转换
			DESedeKeySpec desKeySpec = new DESedeKeySpec(keyBytes);
			SecretKeyFactory skFactory = SecretKeyFactory.getInstance("DESede");
			Key convertSecretKey = skFactory.generateSecret(desKeySpec);
			
			//加密
			Cipher cipher = Cipher.getInstance("DESede/ECB/PKCS5Padding");
			cipher.init(Cipher.ENCRYPT_MODE, convertSecretKey);
			byte[] encrytRetBytes = cipher.doFinal(DATA.getBytes());
			System.out.println("JDK 3DES encrypt: " + Hex.encodeHexString(encrytRetBytes));
			
			//解密
			cipher.init(Cipher.DECRYPT_MODE, convertSecretKey);
			byte[] decryptRetBytes = cipher.doFinal(encrytRetBytes);
			System.out.println("JDK 3DES decrypt: " + new String(decryptRetBytes));
		} catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
	
	public static void bc3DES() {
		try {
			Security.addProvider(new BouncyCastleProvider());
			
			//生成KEY
			KeyGenerator keyGenerator = KeyGenerator.getInstance("DESede", "BC");
//			keyGenerator.init(168);//or 112
			keyGenerator.init(new SecureRandom());//SecureRandom的作用是生成默认长度的key
			SecretKey secretKey = keyGenerator.generateKey();
			byte[] keyBytes = secretKey.getEncoded();
			
			//KEY转换
			DESedeKeySpec desKeySpec = new DESedeKeySpec(keyBytes);
			SecretKeyFactory skFactory = SecretKeyFactory.getInstance("DESede");
			Key convertSecretKey = skFactory.generateSecret(desKeySpec);
			
			//加密
			Cipher cipher = Cipher.getInstance("DESede/ECB/PKCS5Padding");
			cipher.init(Cipher.ENCRYPT_MODE, convertSecretKey);
			byte[] encrytRetBytes = cipher.doFinal(DATA.getBytes());
			System.out.println(" BC 3DES encrypt: " + Hex.encodeHexString(encrytRetBytes));
			
			//解密
			cipher.init(Cipher.DECRYPT_MODE, convertSecretKey);
			byte[] decryptRetBytes = cipher.doFinal(encrytRetBytes);
			System.out.println(" BC 3DES decrypt: " + new String(decryptRetBytes));
		} catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
}


