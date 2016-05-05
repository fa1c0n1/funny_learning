package com.pax.btchat.ui;

import java.util.ArrayList;
import java.util.Set;

import com.pax.btchat.R;

import android.app.Activity;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.view.Window;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.ListView;
import android.widget.TextView;
import android.widget.AdapterView.OnItemClickListener;

public class BtDeviceListActivity extends Activity {
	private static final String TAG = "BTChat";
	public static final String EXTRA_DEVICE_ADDRESS = "device_address";
    public static final String ALL_DEVICE_ADDRESS = "all_device_address";
    public static final String PAIR_DEVICE_ADDRESS = "pair_device_address";

    private BluetoothAdapter mBtAdapter;
    private ArrayAdapter<String> mPairedDevicesArrayAdapter;
    private ArrayAdapter<String> mNewDevicesArrayAdapter;
    

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        Log.d(TAG, "onCreate");
        
        requestWindowFeature(Window.FEATURE_INDETERMINATE_PROGRESS);
        setContentView(R.layout.activity_bt_device_list);

        setResult(Activity.RESULT_CANCELED);
        mPairedDevicesArrayAdapter = new ArrayAdapter<String>(this, R.layout.device_name);
        mNewDevicesArrayAdapter = new ArrayAdapter<String>(this, R.layout.device_name);

        ListView pairedListView = (ListView) findViewById(R.id.paired_devices);
        pairedListView.setAdapter(mPairedDevicesArrayAdapter);
        pairedListView.setOnItemClickListener(mDeviceClickListener);

        ListView newDevicesListView = (ListView) findViewById(R.id.new_devices);
        newDevicesListView.setAdapter(mNewDevicesArrayAdapter);
        newDevicesListView.setOnItemClickListener(mDeviceClickListener);

        IntentFilter filter = new IntentFilter(BluetoothDevice.ACTION_FOUND);
        this.registerReceiver(mReceiver, filter);

        filter = new IntentFilter(BluetoothAdapter.ACTION_DISCOVERY_FINISHED);
        this.registerReceiver(mReceiver, filter);
        
        filter = new IntentFilter(BluetoothDevice.ACTION_NAME_CHANGED);
        this.registerReceiver(mReceiver, filter);

        mBtAdapter = BluetoothAdapter.getDefaultAdapter();

        Set<BluetoothDevice> pairedDevices = mBtAdapter.getBondedDevices();

        if (pairedDevices.size() > 0) {
            findViewById(R.id.title_paired_devices).setVisibility(View.VISIBLE);
            for (BluetoothDevice device : pairedDevices) {
            	Log.d(TAG, "pairedDevices:" + device.getName() + "\n" + device.getAddress());
                mPairedDevicesArrayAdapter.add(device.getName() + "\n" + device.getAddress());
            }
        } else {
            String noDevices = "No devices have been paired";
            Log.d(TAG, "noDevices:"+noDevices);
            mPairedDevicesArrayAdapter.add(noDevices);
        }
        
        doDiscovery();
    }

    @Override
    protected void onDestroy() {
    	Log.d(TAG, "onDestroy");
        super.onDestroy();

        if (mBtAdapter != null) {
            mBtAdapter.cancelDiscovery();
        }
        
        mPairedDevicesArrayAdapter.clear();
        mNewDevicesArrayAdapter.clear();
        unregisterReceiver(mReceiver);
    }

    private void doDiscovery() {
    	Log.d(TAG, "doDiscovery...");
        setProgressBarIndeterminateVisibility(true);
        setTitle("搜索设备中...");
        findViewById(R.id.title_new_devices).setVisibility(View.VISIBLE);

        if (mBtAdapter.isDiscovering()) {
            mBtAdapter.cancelDiscovery();
        }

        mBtAdapter.startDiscovery();
    }

    private OnItemClickListener mDeviceClickListener = new OnItemClickListener() {
        public void onItemClick(AdapterView<?> av, View v, int arg2, long arg3) {
            mBtAdapter.cancelDiscovery();

            String info = ((TextView) v).getText().toString();
            if (info.equals("No devices found")) {
            	return;
            }
            
            String address = info.substring(info.length() - 17);

            Intent intent = new Intent();
            intent.putExtra(EXTRA_DEVICE_ADDRESS, address);
            ArrayList<String> devices = new ArrayList<String>();
            ArrayList<String> paireddevices = new ArrayList<String>();
            for (int i = 0, size = mNewDevicesArrayAdapter.getCount(); i < size; i++) {
            	String item = mNewDevicesArrayAdapter.getItem(i);
                if (item.equals("No devices found")) {
                	continue;
                }
                
            	devices.add(item);
            }
            for (int i = 0, size = mPairedDevicesArrayAdapter.getCount(); i < size; i++) {
            	String item = mPairedDevicesArrayAdapter.getItem(i);
                if (item.equals("No devices found")) {
                	continue;
                }
            	devices.add(item);
            	paireddevices.add(item);
            }
            intent.putStringArrayListExtra(PAIR_DEVICE_ADDRESS, paireddevices);
            intent.putStringArrayListExtra(ALL_DEVICE_ADDRESS, devices);
            setResult(Activity.RESULT_OK, intent);
            finish();
        }
    };

    private final BroadcastReceiver mReceiver = new BroadcastReceiver() {
        @Override
        public void onReceive(Context context, Intent intent) {
            String action = intent.getAction();
            Log.d(TAG, "action:"+action);
            if (BluetoothDevice.ACTION_FOUND.equals(action)) {
                BluetoothDevice device = intent.getParcelableExtra(BluetoothDevice.EXTRA_DEVICE);
                if (device.getBondState() != BluetoothDevice.BOND_BONDED) {
                	String itemInfo = device.getName() + "\n" + device.getAddress();
                	Log.d(TAG, "FOUND:"+itemInfo);
                	boolean isSame = false;
                	
                	for (int i = 0; i < mNewDevicesArrayAdapter.getCount(); i++) {
                		String info = mNewDevicesArrayAdapter.getItem(i);
                		if (info.equals(itemInfo)) {
                			Log.d(TAG, "FOUND EQU:"+itemInfo);
                			isSame = true;
                			break;
                		} 
                	}
                	
                	if (!isSame) {
                		mNewDevicesArrayAdapter.add(itemInfo);
                	}
                }
            } else if (BluetoothAdapter.ACTION_DISCOVERY_FINISHED.equals(action)) {
            	Log.d(TAG, "btdevice finish scan");
                setProgressBarIndeterminateVisibility(false);
                setTitle("请选择要连接的设备");
                if (mNewDevicesArrayAdapter.getCount() == 0) {
                    String noDevices = "No devices found";
                    mNewDevicesArrayAdapter.add(noDevices);
                }
            } else if (BluetoothDevice.ACTION_NAME_CHANGED.equals(action)) {
                BluetoothDevice device = intent.getParcelableExtra(BluetoothDevice.EXTRA_DEVICE);
                Log.d(TAG, "CHANGED:"+device.getName() + "\n" + device.getAddress());
                if (device.getBondState() != BluetoothDevice.BOND_BONDED) {
                	for (int i = 0; i < mNewDevicesArrayAdapter.getCount(); i++) {
                		String info = mNewDevicesArrayAdapter.getItem(i);
                		String[] s = info.split("\n");
                		if (s.length != 2)
                			continue;
                		
                		if (s[1].equals(device.getAddress())) {
                			if (device.getName().equals(s[0])) {
                				Log.d(TAG, "CHANGED EQU");
                				continue;
                			}
                			
                			String newName = device.getName() + "\n" + s[1];
                			mNewDevicesArrayAdapter.insert(newName, i);
                			mNewDevicesArrayAdapter.remove(info);
                			break;
                		}
                	}
                }
            }
        }
    };
}
