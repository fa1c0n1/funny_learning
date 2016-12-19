package me.falcon.security.md;

import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;
import java.security.Security;

import org.apache.commons.codec.binary.Hex;
import org.apache.commons.codec.digest.DigestUtils;
import org.bouncycastle.crypto.Digest;
import org.bouncycastle.crypto.digests.MD2Digest;
import org.bouncycastle.crypto.digests.MD4Digest;
import org.bouncycastle.crypto.digests.MD5Digest;
import org.bouncycastle.jce.provider.BouncyCastleProvider;

public class IFalconMD {
	
	private static final String DATA = "imooc security md";

	public static void main(String[] args) {
		jdkMD2();
		jdkMD5();
		bcMD2();
		bcMD4_1();
		bcMD4_2();
		bcMD5();
		ccMD2();
		ccMD5();
	}
	
	public static void jdkMD2() {
		try {
			MessageDigest md = MessageDigest.getInstance("MD2");
			byte[] md5Bytes = md.digest(DATA.getBytes());
			System.out.println("JDK\tMD2: " + Hex.encodeHexString(md5Bytes));
		} catch (NoSuchAlgorithmException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
	
	public static void jdkMD5() {
		try {
			MessageDigest md = MessageDigest.getInstance("MD5");
			byte[] md5Bytes = md.digest(DATA.getBytes());
			System.out.println("JDK\tMD5: " + Hex.encodeHexString(md5Bytes));
		} catch (NoSuchAlgorithmException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
	
	public static void bcMD2() {
		Digest digest = new MD2Digest();
		digest.update(DATA.getBytes(), 0, DATA.getBytes().length);
		byte[] md4Bytes = new byte[digest.getDigestSize()];
		digest.doFinal(md4Bytes, 0);
		System.out.println(" BC\tMD2: " + org.bouncycastle.util.encoders.Hex.toHexString(md4Bytes));
	}
	
	public static void bcMD4_1() {
		Digest digest = new MD4Digest();
		digest.update(DATA.getBytes(), 0, DATA.getBytes().length);
		byte[] md4Bytes = new byte[digest.getDigestSize()];
		digest.doFinal(md4Bytes, 0);
		System.out.println("BC\tMD4: " + org.bouncycastle.util.encoders.Hex.toHexString(md4Bytes));
	}
	
	public static void bcMD4_2() {
		try {
			//因为JDK的MessageDigest默认是不支持MD4的，要支持的话
			//就得将BC扩展以security.provider的方式添加到java.security的支持列表中
			Security.addProvider(new BouncyCastleProvider());
			MessageDigest md = MessageDigest.getInstance("MD4");
			byte[] md4Bytes = md.digest(DATA.getBytes());
			System.out.println("JDK_BC\tMD4: " + Hex.encodeHexString(md4Bytes));
		} catch (NoSuchAlgorithmException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
	
	public static void bcMD5() {
		Digest digest = new MD5Digest();
		digest.update(DATA.getBytes(), 0, DATA.getBytes().length);
		byte[] md4Bytes = new byte[digest.getDigestSize()];
		digest.doFinal(md4Bytes, 0);
		System.out.println(" BC\tMD5: " + org.bouncycastle.util.encoders.Hex.toHexString(md4Bytes));
	}
	
	public static void ccMD2() {
		String md2HexStr = DigestUtils.md2Hex(DATA.getBytes());
		System.out.println(" CC\tMD2: " + md2HexStr);
	}
	
	public static void ccMD5() {
		String md5HexStr = DigestUtils.md5Hex(DATA.getBytes());
		System.out.println(" CC\tMD5: " + md5HexStr);
	}
}
