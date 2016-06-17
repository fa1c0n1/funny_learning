package me.falcon.jnidemo;

public class NativeTools {

	public native String nativeGetPackageName();
	public native String nativeGetFilesDir();
	public native void nativeGetFilesDirFromJava(String filesDir);
}
