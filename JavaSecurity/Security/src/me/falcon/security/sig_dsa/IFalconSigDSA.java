package me.falcon.security.sig_dsa;

import java.security.KeyFactory;
import java.security.KeyPair;
import java.security.KeyPairGenerator;
import java.security.PrivateKey;
import java.security.PublicKey;
import java.security.Signature;
import java.security.interfaces.DSAPrivateKey;
import java.security.interfaces.DSAPublicKey;
import java.security.spec.PKCS8EncodedKeySpec;
import java.security.spec.X509EncodedKeySpec;

import org.apache.commons.codec.binary.Hex;

public class IFalconSigDSA {

	private static final String DATA = "imooc security sig_dsa";
	
	public static void main(String[] args) {
		jdkSigDSA();
	}
	
	public static void jdkSigDSA() {
		try {
			//1.初始化密钥
			KeyPairGenerator keyPairGenerator = KeyPairGenerator.getInstance("DSA");
			keyPairGenerator.initialize(512);
			KeyPair keyPair = keyPairGenerator.generateKeyPair();
			DSAPublicKey dsaPublicKey = (DSAPublicKey) keyPair.getPublic();
			DSAPrivateKey dsaPrivateKey = (DSAPrivateKey) keyPair.getPrivate();
			
			//2.执行签名[用私钥签名，公钥验证]
			PKCS8EncodedKeySpec pkcs8EncodedKeySpec = new PKCS8EncodedKeySpec(dsaPrivateKey.getEncoded());
			KeyFactory keyFactory = KeyFactory.getInstance("DSA");
			PrivateKey privateKey = keyFactory.generatePrivate(pkcs8EncodedKeySpec);
			Signature signature = Signature.getInstance("SHA1withDSA");
			signature.initSign(privateKey);
			signature.update(DATA.getBytes());
			byte[] sign = signature.sign();
			System.out.println("JDK SIG_DSA: " + Hex.encodeHexString(sign));
			
			//3.验证签名
			X509EncodedKeySpec x509EncodedKeySpec = new X509EncodedKeySpec(dsaPublicKey.getEncoded());
			PublicKey publicKey = keyFactory.generatePublic(x509EncodedKeySpec);
			signature.initVerify(publicKey);
			signature.update(DATA.getBytes());
			boolean verified = signature.verify(sign);
			System.out.println("JDK SIG_DSA Verified: " + verified);
		} catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
}
