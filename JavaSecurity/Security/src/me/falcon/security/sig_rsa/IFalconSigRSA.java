package me.falcon.security.sig_rsa;

import java.security.KeyFactory;
import java.security.KeyPair;
import java.security.KeyPairGenerator;
import java.security.PrivateKey;
import java.security.PublicKey;
import java.security.Signature;
import java.security.interfaces.RSAPrivateKey;
import java.security.interfaces.RSAPublicKey;
import java.security.spec.PKCS8EncodedKeySpec;
import java.security.spec.X509EncodedKeySpec;

import org.apache.commons.codec.binary.Hex;

public class IFalconSigRSA {
	
	private static final String DATA = "imooc security sig_rsa";

	public static void main(String[] args) {
		jdkSigRSA();
	}
	
	public static void jdkSigRSA() {
		try {
			//1.初始化密钥
			KeyPairGenerator keyPairGenerator = KeyPairGenerator.getInstance("RSA");
			keyPairGenerator.initialize(512);
			KeyPair keyPair = keyPairGenerator.generateKeyPair();
			RSAPublicKey rsaPublicKey = (RSAPublicKey) keyPair.getPublic();
			RSAPrivateKey rsaPrivateKey = (RSAPrivateKey) keyPair.getPrivate();
			
			//2.执行签名[用私钥签名，公钥验证]
			PKCS8EncodedKeySpec pkcs8EncodedKeySpec = new PKCS8EncodedKeySpec(rsaPrivateKey.getEncoded());
			KeyFactory keyFactory = KeyFactory.getInstance("RSA");
			PrivateKey privateKey = keyFactory.generatePrivate(pkcs8EncodedKeySpec);
			Signature signature = Signature.getInstance("MD5withRSA");
			signature.initSign(privateKey);
			signature.update(DATA.getBytes());
			byte[] sign = signature.sign();
			System.out.println("JDK SIG_RSA: " + Hex.encodeHexString(sign));
			
			//3.验证签名
			X509EncodedKeySpec x509EncodedKeySpec = new X509EncodedKeySpec(rsaPublicKey.getEncoded());
			PublicKey publicKey = keyFactory.generatePublic(x509EncodedKeySpec);
			signature.initVerify(publicKey);
			signature.update(DATA.getBytes());
			boolean verified = signature.verify(sign);
			System.out.println("JDK SIG_RSA Verified: " + verified);
		} catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
}
