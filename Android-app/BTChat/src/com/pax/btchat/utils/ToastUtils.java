package com.pax.btchat.utils;

import android.app.Activity;
import android.widget.Toast;

public class ToastUtils {

	public static void showToast(final Activity context, final String text){
		if("main".equals(Thread.currentThread().getName())){
			Toast.makeText(context, text, Toast.LENGTH_SHORT).show();
		}else {
			context.runOnUiThread(new Runnable() {
				
				@Override
				public void run() {
					Toast.makeText(context, text, Toast.LENGTH_SHORT).show();
				}
			});
		}
	}
}
