package me.falcon.security.base64;

import java.io.IOException;

import org.apache.commons.codec.binary.Base64;

import sun.misc.BASE64Decoder;
import sun.misc.BASE64Encoder;

public class IFalconBase64 {
	
	private static final String DATA = "imooc security base64";

	public static void main(String[] args) {
//		jdkBase64(); //不建议使用
//		commonsCodesBase64();
		bouncyCastleBase64();
	}
	
	public static void jdkBase64() {
		try {
			BASE64Encoder encoder = new BASE64Encoder();
			String encode = encoder.encode(DATA.getBytes());
			System.out.println("encode: " + encode);
			
			BASE64Decoder decoder = new BASE64Decoder();
			byte[] decodeBuf = decoder.decodeBuffer(encode);
			System.out.println("decode: " + new String(decodeBuf));
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
	
	public static void commonsCodesBase64() {
		byte[] encodeBytes = Base64.encodeBase64(DATA.getBytes());
		System.out.println("encode: " + new String(encodeBytes));
		byte[] decodeBytes = Base64.decodeBase64(encodeBytes);
		System.out.println("decode: " + new String(decodeBytes));
	}
	
	public static void bouncyCastleBase64() {
		byte[] encodeBytes = org.bouncycastle.util.encoders.Base64.encode(DATA.getBytes());
		System.out.println("encode: " + new String(encodeBytes));
		byte[] decodeBytes = org.bouncycastle.util.encoders.Base64.decode(encodeBytes);
		System.out.println("decode: " + new String(decodeBytes));
	}
}
