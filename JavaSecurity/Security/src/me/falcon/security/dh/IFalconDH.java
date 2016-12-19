package me.falcon.security.dh;

import java.security.KeyFactory;
import java.security.KeyPair;
import java.security.KeyPairGenerator;
import java.security.NoSuchAlgorithmException;
import java.security.PrivateKey;
import java.security.PublicKey;
import java.security.spec.X509EncodedKeySpec;
import java.util.Objects;

import javax.crypto.Cipher;
import javax.crypto.KeyAgreement;
import javax.crypto.SecretKey;
import javax.crypto.interfaces.DHPublicKey;
import javax.crypto.spec.DHParameterSpec;

import org.apache.commons.codec.binary.Hex;

public class IFalconDH {

	private static final String DATA = "imooc security dh";

	public static void main(String[] args) {
		jdkDH();
	}
	
	public static void jdkDH() {
		try {
			//1.初始化发送方密钥
			KeyPairGenerator senderKeyPairGenerator = KeyPairGenerator.getInstance("DH");
			senderKeyPairGenerator.initialize(512);
			KeyPair senderKeyPair = senderKeyPairGenerator.generateKeyPair();
			byte[] senderPublicKeyBytes = senderKeyPair.getPublic().getEncoded(); //发送方公钥,要发送给接手方
			
			//2. (根据发送方密钥)初始化接收方密钥
			KeyFactory receiverKeyFactory = KeyFactory.getInstance("DH");
			X509EncodedKeySpec x509EncodedKeySpec = new X509EncodedKeySpec(senderPublicKeyBytes);
			PublicKey receiverPublicKey = receiverKeyFactory.generatePublic(x509EncodedKeySpec);
			DHParameterSpec dhParameterSpec = ((DHPublicKey)receiverPublicKey).getParams();
			KeyPairGenerator receiverKeyPairGenerator = KeyPairGenerator.getInstance("DH");
			receiverKeyPairGenerator.initialize(dhParameterSpec);
			KeyPair receiverKeyPair = receiverKeyPairGenerator.generateKeyPair();
			PrivateKey receiverPrivateKey = receiverKeyPair.getPrivate();
			
			//3.构建本地密钥
			KeyAgreement receiverKeyAgreement = KeyAgreement.getInstance("DH");
			receiverKeyAgreement.init(receiverPrivateKey);
			receiverKeyAgreement.doPhase(receiverPublicKey, true);
			SecretKey receiverDesKey = receiverKeyAgreement.generateSecret("DES");
			byte[] receiverPublicKeyBytes = receiverKeyPair.getPublic().getEncoded();
			
			KeyFactory senderKeyFactory = KeyFactory.getInstance("DH");
			x509EncodedKeySpec = new X509EncodedKeySpec(receiverPublicKeyBytes);
			PublicKey senderPublicKey = senderKeyFactory.generatePublic(x509EncodedKeySpec);
			KeyAgreement senderKeyAgreement = KeyAgreement.getInstance("DH");
			senderKeyAgreement.init(senderKeyPair.getPrivate());
			senderKeyAgreement.doPhase(senderPublicKey, true);
			SecretKey senderDesKey = senderKeyAgreement.generateSecret("DES");
			
			if (Objects.equals(receiverDesKey, senderDesKey)) {
				System.out.println("双方密钥相同, 本地密钥构建完成");
			}
			
			//4.加密
			Cipher cipher = Cipher.getInstance("DES");
			cipher.init(Cipher.ENCRYPT_MODE, senderDesKey);
			byte[] encryptRetBytes = cipher.doFinal(DATA.getBytes());
			System.out.println("JDK DH encrypt: " + Hex.encodeHexString(encryptRetBytes));
			
			//5.解密
			cipher.init(Cipher.DECRYPT_MODE, receiverDesKey);
			byte[] decryptRetBytes = cipher.doFinal(encryptRetBytes);
			System.out.println("JDK DH decrypt: " + new String(decryptRetBytes));
		} catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
}
