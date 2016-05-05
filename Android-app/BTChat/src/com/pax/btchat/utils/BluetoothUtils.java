package com.pax.btchat.utils;

import java.lang.reflect.Field;
import java.lang.reflect.Method;

import android.bluetooth.BluetoothDevice;
import android.util.Log;

public class BluetoothUtils {

	private static final String TAG = "BTUtils";

	/**
	 * 与设备配对 参考源码：platform/packages/apps/Settings.git
	 * 	Settings/src/com/android/settings/bluetooth/CachedBluetoothDevice.java
	 */
	public static boolean createBond(Class btClass, BluetoothDevice btDevice)
			throws Exception {
		Method createBondMethod = btClass.getMethod("createBond");
		Boolean retVal = (Boolean) createBondMethod.invoke(btDevice);
		return retVal.booleanValue();
	}
	
	/**
	 * 与设备解除配对 参考源码：platform/packages/apps/Settings.git
	 * 	Settings/src/com/android/settings/bluetooth/CachedBluetoothDevice.java
	 */
	public static boolean removeBond(Class btClass, BluetoothDevice btDevice)
			throws Exception {
		Method removeBondMethod = btClass.getMethod("removeBond");
		Boolean returnValue = (Boolean) removeBondMethod.invoke(btDevice);
		return returnValue.booleanValue();
	}
	
	public static boolean setPin(Class btClass, BluetoothDevice btDevice,
			String str) throws Exception {
		try {
			Method removeBondMethod = btClass.getDeclaredMethod("setPin",
					new Class[]{byte[].class});
			Boolean retVal = (Boolean) removeBondMethod.invoke(btDevice,
					new Object[]{str.getBytes()});
			Log.d(TAG, "" + retVal);
		}catch (SecurityException e) {
			// throw new RuntimeException(e.getMessage());
			e.printStackTrace();
		} catch (IllegalArgumentException e) {
			// throw new RuntimeException(e.getMessage());
			e.printStackTrace();
		} catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	
		return true;
	}
	
	public static boolean setPairingConfirmation(Class btClass, 
			BluetoothDevice device, boolean confirm) throws Exception {
		Method setPairiConfirmMethod = btClass.getMethod("setPairingConfirmation", boolean.class);
		Boolean retVal = (Boolean) setPairiConfirmMethod.invoke(device, confirm);
		return retVal.booleanValue();
	}
	
	// 取消用户输入
	public static boolean cancelPairingUserInput(Class btClass,
			BluetoothDevice device) throws Exception {
		Method createBondMethod = btClass.getMethod("cancelPairingUserInput");
		// cancelBondProcess()
		Boolean retVal = (Boolean) createBondMethod.invoke(device);
		return retVal.booleanValue();
	}
	
	// 取消配对
	public static boolean cancelBondProcess(Class btClass, 
			BluetoothDevice device) throws Exception {
		Method createBondMethod = btClass.getMethod("cancelBondProcess");
		Boolean retVal = (Boolean) createBondMethod.invoke(device);
		return retVal.booleanValue();
	}
	
	public static void printAllInform(Class clsShow)
	{
		try {
			// 取得所有方法
			Method[] hideMethod = clsShow.getMethods();
			int i = 0;
			for (; i < hideMethod.length; i++) {
				Log.e("method name", hideMethod[i].getName() + ";and the i is:" + i);
			}
			// 取得所有常量
			Field[] allFields = clsShow.getFields();
			for (i = 0; i < allFields.length; i++)
			{
				Log.e("Field name", allFields[i].getName());
			}
		}catch (SecurityException e) {
			// throw new RuntimeException(e.getMessage());
			e.printStackTrace();
		}catch (IllegalArgumentException e) {
			// throw new RuntimeException(e.getMessage());
			e.printStackTrace();
		}catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
}
