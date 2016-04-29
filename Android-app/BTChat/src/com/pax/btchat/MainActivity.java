package com.pax.btchat;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.ArrayList;
import java.util.List;
import java.util.UUID;

import android.app.Activity;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothServerSocket;
import android.bluetooth.BluetoothSocket;
import android.content.Intent;
import android.content.IntentFilter;
import android.os.Bundle;
import android.os.ConditionVariable;
import android.os.Handler;
import android.os.Message;
import android.text.TextUtils;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ListView;
import android.widget.Toast;

import com.pax.btchat.domain.ChatMsg;
import com.pax.btchat.receiver.BluetoothReceiver;
import com.pax.btchat.ui.BtDeviceListActivity;
import com.pax.btchat.ui.ChatAdapter;
import com.pax.btchat.utils.ToastUtils;

public class MainActivity extends Activity {
	private static final String TAG = "BTChat";
	private static final UUID btUUID = UUID.fromString("00001101-0000-1000-8000-00805F9B34FB");
	private static final int REQUEST_CONNECT_DEVICE_SECURE = 1;
	
	private ChatAdapter mAdapter;
	private List<ChatMsg> chatMsgs;
	
	private BluetoothAdapter mBtAdapter;
	private ConditionVariable done;
	private AcceptThread acceptThread;
	private ConnectThread connThread;
	private BluetoothReceiver mReceiver;
	
	private BluetoothSocket btRxSocket = null;
	private BluetoothSocket btTxSocket = null;
	
	private ListView lvChat;
	private Button btnSend;
	private EditText etMsg;
	private MenuItem tmpMenu;
	
	private Handler mHandler = new Handler(){

		@Override
		public void handleMessage(Message msg) {
			ChatMsg cmsg = null;
			switch (msg.what) {
			case ChatMsg.TYPE_RECEIVED:
				cmsg = new ChatMsg(ChatMsg.TYPE_RECEIVED, (String) msg.obj);
				break;
			case ChatMsg.TYPE_SEND:
				cmsg = new ChatMsg(ChatMsg.TYPE_SEND, (String) msg.obj);
				break;
			}
			etMsg.setText("");
			chatMsgs.add(cmsg);
			mAdapter.notifyDataSetChanged();
			lvChat.smoothScrollToPosition(chatMsgs.size());
		}
	};

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		initView();
		initData();
		initEvent();
	}
	
	private void initView() {
		lvChat = (ListView) findViewById(R.id.lv_chat);
		etMsg = (EditText) findViewById(R.id.et_msg);
		btnSend = (Button) findViewById(R.id.btn_send);
	}
	
	private void initData() {
		chatMsgs = new ArrayList<ChatMsg>();
	}
	
	private void initEvent() {
		acceptThread = new AcceptThread();
		new Thread(acceptThread).start();
		mAdapter = new ChatAdapter(MainActivity.this, R.layout.msg_item, chatMsgs);
		lvChat.setAdapter(mAdapter);
		
		btnSend.setOnClickListener(new OnClickListener() {
			
			@Override
			public void onClick(View v) {
				String chatMsg = etMsg.getText().toString();
				if(TextUtils.isEmpty(chatMsg)) {
					Toast.makeText(MainActivity.this, "消息不能为空", Toast.LENGTH_SHORT).show();
					return ;
				} 
				
				Message msg = Message.obtain();
				msg.what = ChatMsg.TYPE_SEND;
				msg.obj = chatMsg;
				mHandler.sendMessage(msg);
				
				BluetoothSocket tmpSocket = btRxSocket == null ? btTxSocket : btRxSocket;
				if (tmpSocket != null) {
					try {
						OutputStream out = tmpSocket.getOutputStream();
						out.write(chatMsg.getBytes());
						out.flush();
					} catch (IOException e) {
						Log.d(TAG, "btnSend IOException message: " + e.getMessage());
						e.printStackTrace();
					}
				}			
				
			}
		});
	}
	
	@Override
	protected void onDestroy() {
		Log.d(TAG, "kill myself...");
		super.onDestroy();
		System.exit(0);
	}
	
	private class AcceptThread implements Runnable {
		private BluetoothServerSocket serverSocket = null;
		private byte[] buf;
		private int readLen;
		
		public AcceptThread() {
			buf = new byte[1024];
		}
	
		@Override
		public void run() {
			InputStream in = null;
			mBtAdapter = BluetoothAdapter.getDefaultAdapter();
			IntentFilter filter = new IntentFilter();
			filter.addAction(BluetoothAdapter.ACTION_STATE_CHANGED);
			filter.addAction(BluetoothDevice.ACTION_PAIRING_REQUEST);
			filter.addAction(BluetoothDevice.ACTION_BOND_STATE_CHANGED);
			done = new ConditionVariable();
			mReceiver = new BluetoothReceiver(done);
			registerReceiver(mReceiver, filter);
			
			if (mBtAdapter != null) {
				if (!mBtAdapter.isEnabled()) {
					mBtAdapter.enable();
					done.block();
					done.close();
				}
				try {
					serverSocket = mBtAdapter.listenUsingRfcommWithServiceRecord("local1", btUUID);
					while (serverSocket != null){
						try {
							while (true) {
								Log.d(TAG, "outer while");
								btRxSocket = serverSocket.accept();
								changeMenuTitle(tmpMenu, "断开连接");
								BluetoothDevice mDevice = btRxSocket.getRemoteDevice();
								ToastUtils.showToast(MainActivity.this, "已连接到" + mDevice.getName());
								in = btRxSocket.getInputStream();
								while (true) {
									Log.d(TAG, "inner while");
									readLen = in.read(buf);
									Log.d(TAG, "length=" + readLen);
									if (readLen > 0) {
										Message msg = Message.obtain();
										msg.what = ChatMsg.TYPE_RECEIVED;
										msg.obj = new String(buf, 0, readLen);
										mHandler.sendMessage(msg);
									}
								}
							}
						} catch (IOException e) {
							Log.d(TAG, "btRx disconnect");
							e.printStackTrace();
						} finally {
							Log.d(TAG, "finally_1 ...");
							try {
								if (in != null) {
									in.close();
									in = null;
								}
								if (btRxSocket != null) {
									ToastUtils.showToast(MainActivity.this, "已经断开与" + 
												btRxSocket.getRemoteDevice().getName() + "的连接");
									changeMenuTitle(tmpMenu, "搜索设备");
									btRxSocket.close();
									btRxSocket = null;
								}
							} catch (IOException e) {
								e.printStackTrace();
							}
						}
						
					}
				} catch (IOException e1) {
					e1.printStackTrace();
				} finally {
					Log.d(TAG, "finally_2 ...");
					if (serverSocket != null) {
						try {
							serverSocket.close();
							serverSocket = null;
						} catch (IOException e) {
							// TODO Auto-generated catch block
							e.printStackTrace();
						}
					}
					if (mReceiver != null) {
						unregisterReceiver(mReceiver);
						mReceiver = null;
					}
				}
			}
		}
		
		//跳出inner while的循环,从而断开本次连接
		public void cancel() {
			try {
				if (btRxSocket != null) {
					btRxSocket.close();
				}
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
		
	}
	
	private void changeMenuTitle(final MenuItem item, final String title) {
		if("main".equals(Thread.currentThread().getName())){
			item.setTitle(title);
		}else {
			runOnUiThread(new Runnable() {
				
				@Override
				public void run() {
					item.setTitle(title);
				}
			});
		}
	}
	
	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		getMenuInflater().inflate(R.menu.main, menu);
		tmpMenu = menu.getItem(0);
		return true;
	}
	
	@Override
	public boolean onOptionsItemSelected(MenuItem item) {
		switch (item.getItemId()) {
		case R.id.menu_conn:
			if ("搜索设备".equals(item.getTitle())) {
				searchBtDevice();
			} else if ("断开连接".equals(item.getTitle())) {
				if (acceptThread != null && btRxSocket != null) {
					acceptThread.cancel();
				}
				if (connThread != null && btTxSocket != null) {
					connThread.cancel();
				}
//				ToastUtils.showToast(MainActivity.this, "已经断开与" + deviceName + "的连接");
//				changeMenuTitle(item, "搜索设备");
			}
			
			break;
		}
		return super.onOptionsItemSelected(item);
	}
	
	//搜索蓝牙设备
	private void searchBtDevice() {
		Intent intent;
	    intent = new Intent(MainActivity.this, BtDeviceListActivity.class);
		startActivityForResult(intent, REQUEST_CONNECT_DEVICE_SECURE);
	}
	
	//连接蓝牙设备
	private void connectBtDevice(BluetoothDevice device) {
		connThread = new ConnectThread(device);
		new Thread(connThread).start();
	}
	
	@Override
	protected void onActivityResult(int requestCode, int resultCode, Intent data) {
		switch (requestCode) {
        case REQUEST_CONNECT_DEVICE_SECURE:
            if (resultCode == Activity.RESULT_OK) {
				String address = data.getExtras().getString(BtDeviceListActivity.EXTRA_DEVICE_ADDRESS);
            	if(null != address && !"".equals(address.trim()) 
            			&& !"NULL".equals(address.trim()) && !"null".equals(address.trim())) {
            		BluetoothDevice device = mBtAdapter.getRemoteDevice(address);
            		connectBtDevice(device);
        		}
            	
            }
            
            break;
        }
	}
	
	private class ConnectThread implements Runnable {
		private volatile boolean isStopped = false;
		private BluetoothDevice btDevice;
		private InputStream mInputStream;
		private byte[] buf;
		private int readLen;
		
		public ConnectThread(BluetoothDevice device) {
			BluetoothSocket tmp = null;
			this.btDevice = device;
			try {
				tmp = btDevice.createRfcommSocketToServiceRecord(btUUID);
				btTxSocket = tmp;
				buf = new byte[1024];
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}

		@Override
		public void run() {
			mBtAdapter.cancelDiscovery();
			try {
				btTxSocket.connect();
			} catch (IOException e) {
				Log.d(TAG, "btTxSocket connect fail");
				try {
					btTxSocket.close();
				} catch (IOException e1) {
					// TODO Auto-generated catch block
					e1.printStackTrace();
				}
				return ;
			}
			
			runOnUiThread(new Runnable() {
				
				@Override
				public void run() {
					tmpMenu.setTitle("断开连接");
					Toast.makeText(MainActivity.this, "已连接到" + btDevice.getName(), Toast.LENGTH_SHORT).show();
				}
			});
			
			try {
				mInputStream = btTxSocket.getInputStream();
				while (true) {
					Log.d(TAG, "btTx while");
					readLen = mInputStream.read(buf);
					Log.d(TAG, "btTx readLen=" + readLen);
					if (readLen > 0) {
						Message msg = Message.obtain();
						msg.what = ChatMsg.TYPE_RECEIVED;
						msg.obj = new String(buf, 0, readLen);
						mHandler.sendMessage(msg);
					}
				}
			} catch (IOException e) {
				Log.d(TAG, "btTx disconnect");
				e.printStackTrace();
			} finally {
				Log.d(TAG, "btTx finally ...");
				try {
					if (mInputStream != null) {
						mInputStream.close();
						mInputStream = null;
					}
					if (btTxSocket != null) {
						ToastUtils.showToast(MainActivity.this, "已经断开与" + 
								btTxSocket.getRemoteDevice().getName() + "的连接");
						changeMenuTitle(tmpMenu, "搜索设备");
						btTxSocket.close();
						btTxSocket = null;
					}
				} catch (IOException e) {
					e.printStackTrace();
				}
			}
		}
		
		//跳出btTx while的循环,从而断开本次连接
		public void cancel() {
			try {
				if (btTxSocket != null) {
					btTxSocket.close();
				}
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
	}
}
