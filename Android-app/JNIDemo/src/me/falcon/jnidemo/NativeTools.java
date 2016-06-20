package me.falcon.jnidemo;

public class NativeTools {

	public native String nativeGetPackageName();
	public native String nativeGetFilesDir();
	public native void nativeGetFilesDirFromJava(String filesDir);
	public native int nativeSumArray(int[] arr);
	public native void nativeGetSDCardDirFromJava(String sdcard);
	public native void nativeCreateFile(String fileName);
}
