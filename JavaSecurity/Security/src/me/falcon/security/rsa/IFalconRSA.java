package me.falcon.security.rsa;

import java.security.KeyFactory;
import java.security.KeyPair;
import java.security.KeyPairGenerator;
import java.security.NoSuchAlgorithmException;
import java.security.PrivateKey;
import java.security.PublicKey;
import java.security.interfaces.RSAPrivateKey;
import java.security.interfaces.RSAPublicKey;
import java.security.spec.PKCS8EncodedKeySpec;
import java.security.spec.X509EncodedKeySpec;

import javax.crypto.Cipher;

import org.apache.commons.codec.binary.Hex;

public class IFalconRSA {

	private static final String DATA = "imooc security rsa";

	public static void main(String[] args) {
		jdkRSA();
	}
	
	public static void jdkRSA() {
		try {
			//1.初始化密钥
			KeyPairGenerator keyPairGenerator = KeyPairGenerator.getInstance("RSA");
			keyPairGenerator.initialize(512);
			KeyPair keyPair = keyPairGenerator.generateKeyPair();
			RSAPublicKey rsaPublicKey = (RSAPublicKey) keyPair.getPublic();
			RSAPrivateKey rsaPrivateKey = (RSAPrivateKey) keyPair.getPrivate();
			System.out.println("RSA Public Key: " + Hex.encodeHexString(rsaPublicKey.getEncoded()));
			System.out.println("RSA Private Key: " + Hex.encodeHexString(rsaPrivateKey.getEncoded()));
			
			//2_1. [私钥加密、公钥解密]——加密
			PKCS8EncodedKeySpec pkcs8EncodedKeySpec = new PKCS8EncodedKeySpec(rsaPrivateKey.getEncoded());
			KeyFactory keyFactory = KeyFactory.getInstance("RSA");
			PrivateKey privateKey = keyFactory.generatePrivate(pkcs8EncodedKeySpec);
			Cipher cipher = Cipher.getInstance("RSA");
			cipher.init(Cipher.ENCRYPT_MODE, privateKey);
			byte[] prvEncryptBytes = cipher.doFinal(DATA.getBytes());
			System.out.println("[私钥加密、公钥解密]——加密: " + Hex.encodeHexString(prvEncryptBytes));
			
			//2_2. [私钥加密、公钥解密]——解密
			X509EncodedKeySpec x509EncodedKeySpec = new X509EncodedKeySpec(rsaPublicKey.getEncoded()); 
			PublicKey publicKey = keyFactory.generatePublic(x509EncodedKeySpec);
			cipher.init(Cipher.DECRYPT_MODE, publicKey);
			byte[] pubDecryptBytes = cipher.doFinal(prvEncryptBytes);
			System.out.println("[私钥加密、公钥解密]——解密: " + new String(pubDecryptBytes));
			
			//3_1. [公钥加密、私钥解密]——加密
			cipher = Cipher.getInstance("RSA");
			cipher.init(Cipher.ENCRYPT_MODE, publicKey);
			byte[] pubEncryptBytes = cipher.doFinal(DATA.getBytes());
			System.out.println("[公钥加密、私钥解密]——加密: " + Hex.encodeHexString(pubEncryptBytes));
			
			//3_2. [公钥加密、私钥解密]——解密
			cipher = Cipher.getInstance("RSA");
			cipher.init(Cipher.DECRYPT_MODE, privateKey);
			byte[] prvDecryptBytes = cipher.doFinal(pubEncryptBytes);
			System.out.println("[公钥加密、私钥解密]——解密: " + new String(prvDecryptBytes));
		} catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
	}
}
