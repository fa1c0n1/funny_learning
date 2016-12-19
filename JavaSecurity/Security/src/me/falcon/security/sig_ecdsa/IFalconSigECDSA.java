package me.falcon.security.sig_ecdsa;

import java.security.KeyFactory;
import java.security.KeyPair;
import java.security.KeyPairGenerator;
import java.security.PrivateKey;
import java.security.PublicKey;
import java.security.Signature;
import java.security.interfaces.ECPrivateKey;
import java.security.interfaces.ECPublicKey;
import java.security.spec.PKCS8EncodedKeySpec;
import java.security.spec.X509EncodedKeySpec;

import org.apache.commons.codec.binary.Hex;

public class IFalconSigECDSA {

	private static final String DATA = "imooc security sig_ecdsa";
	
	public static void main(String[] args) {
		jdkSigECDSA();
	}
	
	public static void jdkSigECDSA() {
		try {
			//1.初始化密钥
			KeyPairGenerator keyPairGenerator = KeyPairGenerator.getInstance("EC");
			keyPairGenerator.initialize(256);
			KeyPair keyPair = keyPairGenerator.generateKeyPair();
			ECPublicKey ecPublicKey = (ECPublicKey) keyPair.getPublic();
			ECPrivateKey ecPrivateKey = (ECPrivateKey) keyPair.getPrivate();
			
			//2.执行签名[用私钥签名，公钥验证]
			PKCS8EncodedKeySpec pkcs8EncodedKeySpec = new PKCS8EncodedKeySpec(ecPrivateKey.getEncoded());
			KeyFactory keyFactory = KeyFactory.getInstance("EC");
			PrivateKey privateKey = keyFactory.generatePrivate(pkcs8EncodedKeySpec);
			Signature signature = Signature.getInstance("SHA1withECDSA");
			signature.initSign(privateKey);
			signature.update(DATA.getBytes());
			byte[] sign = signature.sign();
			System.out.println("JDK SIG_ECDSA: " + Hex.encodeHexString(sign));
			
			
			//3.验证签名
			X509EncodedKeySpec x509EncodedKeySpec = new X509EncodedKeySpec(ecPublicKey.getEncoded());
			PublicKey publicKey = keyFactory.generatePublic(x509EncodedKeySpec);
			signature.initVerify(publicKey);
			signature.update(DATA.getBytes());
			boolean verified = signature.verify(sign);
			System.out.println("JDK SIG_ECDSA Verified: " + verified);
		} catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
}
