package me.falcon.jnidemo;

import android.app.Activity;
import android.os.Bundle;
import android.os.Environment;
import android.widget.TextView;

public class MainActivity extends Activity {
	static {
		System.loadLibrary("jnidemo");
	}
	
	private TextView tvPkgName;
	private NativeTools ndkTools;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		ndkTools = new NativeTools();
		initViewAndData();
	}

	private void initViewAndData() {
		tvPkgName = (TextView) findViewById(R.id.tv_pkgname);
		tvPkgName.setText("包名: " + ndkTools.nativeGetPackageName());
		tvPkgName.append("\n" + "FileDir: " + ndkTools.nativeGetFilesDir());
		ndkTools.nativeGetFilesDirFromJava(getFilesDir().getAbsolutePath());
	}

}
