package com.pax.btchat.receiver;

import com.pax.btchat.utils.BluetoothUtils;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.os.ConditionVariable;
import android.util.Log;

public class BluetoothReceiver extends BroadcastReceiver {
	private static final String TAG = "BTChat";
	private ConditionVariable done;

	public BluetoothReceiver(ConditionVariable done) {
		this.done = done;
	}

	@Override
	public void onReceive(Context context, Intent intent) {
		String action = intent.getAction();
		if (BluetoothAdapter.ACTION_STATE_CHANGED.equals(action)) {
			Log.d(TAG, "ACTION_STATE_CHANGED");
			int state = intent.getIntExtra(BluetoothAdapter.EXTRA_STATE, 0);
			if (state == BluetoothAdapter.STATE_ON) {
				done.open();
			}
		} else if (BluetoothDevice.ACTION_BOND_STATE_CHANGED.equals(action)) {
			Log.d(TAG, "ACTION_BOND_STATE_CHANGED");
			int bondState = intent.getIntExtra(BluetoothDevice.EXTRA_BOND_STATE, 0);
			Log.d(TAG, "bondState=" + bondState);
			/*if (bondState == BluetoothDevice.BOND) {
				
			}*/
		} else if (BluetoothDevice.ACTION_PAIRING_REQUEST.equals(action)) {
			Log.d(TAG, "action=ACTION_PAIRING_REQUEST");
			BluetoothDevice btDevice = intent.getParcelableExtra(BluetoothDevice.EXTRA_DEVICE);
			int mType = intent.getIntExtra(
					BluetoothDevice.EXTRA_PAIRING_VARIANT,
					BluetoothDevice.ERROR);
			Log.d(TAG, "mType=" + mType);
			switch (mType) {
			case BluetoothDevice.PAIRING_VARIANT_PASSKEY_CONFIRMATION:
				try {
					BluetoothUtils.setPairingConfirmation(btDevice.getClass(), btDevice, true);
				}catch (Exception e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
				break;

			case BluetoothDevice.PAIRING_VARIANT_PIN:
				Log.d(TAG, "PAIRING_VARIANT_PIN");
				break;

			default:
				break;
			}
		}
	}
	
}

