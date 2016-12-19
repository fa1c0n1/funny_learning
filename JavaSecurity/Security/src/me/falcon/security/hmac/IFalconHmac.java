package me.falcon.security.hmac;

import java.security.NoSuchAlgorithmException;

import javax.crypto.KeyGenerator;
import javax.crypto.Mac;
import javax.crypto.SecretKey;
import javax.crypto.spec.SecretKeySpec;

import org.apache.commons.codec.binary.Hex;
import org.bouncycastle.crypto.digests.MD5Digest;
import org.bouncycastle.crypto.macs.HMac;
import org.bouncycastle.crypto.params.KeyParameter;

public class IFalconHmac {

	private static final String DATA = "imooc security hmac";
	
	public static void main(String[] args) {
		jdkHmacMD5();
		bcHmacMD5();
	}
	
	public static void jdkHmacMD5() {
		try {
			//初始化KeyGenerator
			KeyGenerator keyGenerator = KeyGenerator.getInstance("HmacMD5");
			//产生密钥
			SecretKey secretKey = keyGenerator.generateKey();
			//获得密钥
			//byte[] key = secretKey.getEncoded(); //这样生成的密钥每次都不同,生成的摘要也会不同
			byte[] key = Hex.decodeHex(new char[]{'a','a','a','a','a','b','b','b','b','b'});
			
			//还原密钥
			SecretKey restoreSecretKey = new SecretKeySpec(key, "HmacMD5");
			//实例化MAC
			Mac mac = Mac.getInstance(restoreSecretKey.getAlgorithm());
			//初始化Mac
			mac.init(restoreSecretKey);
			//计算消息摘要
			byte[] hmacMD5Bytes = mac.doFinal(DATA.getBytes());
			System.out.println("JDK\tHmacMD5: " + Hex.encodeHexString(hmacMD5Bytes));
		} catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
	
	public static void bcHmacMD5() {
		HMac hmac = new HMac(new MD5Digest());
		hmac.init(new KeyParameter(org.bouncycastle.util.encoders.Hex.decode("aaaaabbbbb")));
		hmac.update(DATA.getBytes(), 0, DATA.getBytes().length);
		
		byte[] hmacMD5Bytes = new byte[hmac.getMacSize()];
		//计算消息摘要
		hmac.doFinal(hmacMD5Bytes, 0);
		System.out.println(" BC\tHmacMD5: " + org.bouncycastle.util.encoders.Hex.toHexString(hmacMD5Bytes));
	}
}
