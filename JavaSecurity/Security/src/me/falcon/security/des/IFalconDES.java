package me.falcon.security.des;

import java.security.Key;
import java.security.Security;

import javax.crypto.Cipher;
import javax.crypto.KeyGenerator;
import javax.crypto.SecretKey;
import javax.crypto.SecretKeyFactory;
import javax.crypto.spec.DESKeySpec;

import org.apache.commons.codec.binary.Hex;
import org.bouncycastle.jce.provider.BouncyCastleProvider;

public class IFalconDES {
	
	private static final String DATA = "imooc security des";

	public static void main(String[] args) {
		jdkDES();
		bcDES();
	}
	
	public static void jdkDES() {
		try {
			//生成KEY
			KeyGenerator keyGenerator = KeyGenerator.getInstance("DES");
			keyGenerator.init(56);
			SecretKey secretKey = keyGenerator.generateKey();
			byte[] keyBytes = secretKey.getEncoded();
			
			//KEY转换
			DESKeySpec desKeySpec = new DESKeySpec(keyBytes);
			SecretKeyFactory skFactory = SecretKeyFactory.getInstance("DES");
			Key convertSecretKey = skFactory.generateSecret(desKeySpec);
			
			//加密
			Cipher cipher = Cipher.getInstance("DES/ECB/PKCS5Padding");
			cipher.init(Cipher.ENCRYPT_MODE, convertSecretKey);
			byte[] encrytRetBytes = cipher.doFinal(DATA.getBytes());
			System.out.println("JDK DES encrypt: " + Hex.encodeHexString(encrytRetBytes));
			
			//解密
			cipher.init(Cipher.DECRYPT_MODE, convertSecretKey);
			byte[] decryptRetBytes = cipher.doFinal(encrytRetBytes);
			System.out.println("JDK DES decrypt: " + new String(decryptRetBytes));
		} catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
	
	public static void bcDES() {
		try {
			//用这种方式，可以不需要去了解BC相关类的用法，因为
			//剩下的代码和jdkDES()是几乎一样的.
			//实际上它们的provider是不一样的，可用
			//keyGenerator.getProvider()查看
			Security.addProvider(new BouncyCastleProvider());
			
			//生成KEY(如果不指定provider为BC，则默认还是用jdk的
			KeyGenerator keyGenerator = KeyGenerator.getInstance("DES", "BC");
			keyGenerator.init(56);
			SecretKey secretKey = keyGenerator.generateKey();
			byte[] keyBytes = secretKey.getEncoded();
			
			//KEY转换
			DESKeySpec desKeySpec = new DESKeySpec(keyBytes);
			SecretKeyFactory skFactory = SecretKeyFactory.getInstance("DES");
			Key convertSecretKey = skFactory.generateSecret(desKeySpec);
			
			//加密
			Cipher cipher = Cipher.getInstance("DES/ECB/PKCS5Padding");
			cipher.init(Cipher.ENCRYPT_MODE, convertSecretKey);
			byte[] encrytRetBytes = cipher.doFinal(DATA.getBytes());
			System.out.println(" BC DES encrypt: " + Hex.encodeHexString(encrytRetBytes));
			
			//解密
			cipher.init(Cipher.DECRYPT_MODE, convertSecretKey);
			byte[] decryptRetBytes = cipher.doFinal(encrytRetBytes);
			System.out.println(" BC DES decrypt: " + new String(decryptRetBytes));
		} catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
}
