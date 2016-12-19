package me.falcon.security.sha;

import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;
import java.security.Security;

import org.apache.commons.codec.binary.Hex;
import org.apache.commons.codec.digest.DigestUtils;
import org.bouncycastle.crypto.Digest;
import org.bouncycastle.crypto.digests.SHA1Digest;
import org.bouncycastle.crypto.digests.SHA224Digest;
import org.bouncycastle.jce.provider.BouncyCastleProvider;

public class IFalconSHA {

	private static final String DATA = "imooc security sha";
	
	public static void main(String[] args) {
		jdkSHA1();
		bcSHA1();
		bcSHA224_1();
		bcSHA224_2();
		ccSHA1();
	}

	public static void jdkSHA1() {
		try {
			MessageDigest md = MessageDigest.getInstance("SHA");
			byte[] sha1Bytes = md.digest(DATA.getBytes());
			System.out.println("JDK\tSHA1: " + Hex.encodeHexString(sha1Bytes));
		} catch (NoSuchAlgorithmException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
	
	public static void bcSHA1() {
		Digest digest = new SHA1Digest();
		digest.update(DATA.getBytes(), 0, DATA.getBytes().length);
		byte[] sha1Bytes = new byte[digest.getDigestSize()];
		digest.doFinal(sha1Bytes , 0);
		System.out.println(" BC\tSHA1: " + org.bouncycastle.util.encoders.Hex.toHexString(sha1Bytes));
	}
	
	public static void bcSHA224_1() {
		Digest digest = new SHA224Digest();
		digest.update(DATA.getBytes(), 0, DATA.getBytes().length);
		byte[] sha224Bytes = new byte[digest.getDigestSize()];
		digest.doFinal(sha224Bytes , 0);
		System.out.println(" BC\tSHA224: " + org.bouncycastle.util.encoders.Hex.toHexString(sha224Bytes));
	}
	
	public static void bcSHA224_2() {
		try {
			Security.addProvider(new BouncyCastleProvider());
			MessageDigest md = MessageDigest.getInstance("SHA224");
			byte[] sha224Bytes = md.digest(DATA.getBytes());
			System.out.println("JDK_BC\tSHA224: " + org.bouncycastle.util.encoders.Hex.toHexString(sha224Bytes));
		} catch (NoSuchAlgorithmException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
	
	public static void ccSHA1() {
		String sha1HexStr = DigestUtils.sha1Hex(DATA.getBytes());
		System.out.println(" CC\tSHA1: " + sha1HexStr);
	}
	
	//256、384、512。。。
}
