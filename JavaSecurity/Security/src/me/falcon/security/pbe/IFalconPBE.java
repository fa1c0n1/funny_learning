package me.falcon.security.pbe;

import java.security.Key;
import java.security.NoSuchAlgorithmException;
import java.security.SecureRandom;
import java.security.Security;
import java.util.Random;

import javax.crypto.Cipher;
import javax.crypto.SecretKeyFactory;
import javax.crypto.spec.PBEKeySpec;
import javax.crypto.spec.PBEParameterSpec;

import org.apache.commons.codec.binary.Hex;
import org.bouncycastle.jce.provider.BouncyCastleProvider;

public class IFalconPBE {
	
	private static final String DATA = "imooc security pbe";

	public static void main(String[] args) {
		jdkPBE();
		bcPBE();
	}
	
	public static void jdkPBE() {
		try {
			//初始化salt(盐)
			SecureRandom sRandom = new SecureRandom();
			byte[] salt = sRandom.generateSeed(8);
			
			//口令与密钥
			String password = "iFalcon";
			PBEKeySpec pbeKeySpec = new PBEKeySpec(password.toCharArray());
			SecretKeyFactory skFactory = SecretKeyFactory.getInstance("PBEWITHMD5andDES");
			Key key = skFactory.generateSecret(pbeKeySpec);
			
			//加密
			PBEParameterSpec pbeParameterSpec = new PBEParameterSpec(salt, 100);//100是迭代次数
			Cipher cipher = Cipher.getInstance("PBEWITHMD5andDES");
			cipher.init(Cipher.ENCRYPT_MODE, key, pbeParameterSpec);
			byte[] encryptRetBytes = cipher.doFinal(DATA.getBytes());
			System.out.println("JDK PBE encrypt: " + Hex.encodeHexString(encryptRetBytes));
			
			//解密
			cipher.init(Cipher.DECRYPT_MODE, key, pbeParameterSpec);
			byte[] decryptRetBytes = cipher.doFinal(encryptRetBytes);
			System.out.println("JDK PBE decrypt: " + new String(decryptRetBytes));
		} catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
	
	public static void bcPBE() {
		try {
			Security.addProvider(new BouncyCastleProvider());
			
			//初始化salt(盐)
			SecureRandom sRandom = new SecureRandom();
			byte[] salt = sRandom.generateSeed(8);
			
			//口令与密钥
			String password = "iFalcon";
			PBEKeySpec pbeKeySpec = new PBEKeySpec(password.toCharArray());
			SecretKeyFactory skFactory = SecretKeyFactory.getInstance("PBEWITHMD5andDES", "BC");
			Key key = skFactory.generateSecret(pbeKeySpec);
			
			//加密
			PBEParameterSpec pbeParameterSpec = new PBEParameterSpec(salt, 100);//100是迭代次数
			Cipher cipher = Cipher.getInstance("PBEWITHMD5andDES", "BC");
			cipher.init(Cipher.ENCRYPT_MODE, key, pbeParameterSpec);
			byte[] encryptRetBytes = cipher.doFinal(DATA.getBytes());
			System.out.println(" BC PBE encrypt: " + Hex.encodeHexString(encryptRetBytes));
			
			//解密
			cipher.init(Cipher.DECRYPT_MODE, key, pbeParameterSpec);
			byte[] decryptRetBytes = cipher.doFinal(encryptRetBytes);
			System.out.println(" BC PBE decrypt: " + new String(decryptRetBytes));
		} catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
	
}
