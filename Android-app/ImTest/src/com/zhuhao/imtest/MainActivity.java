package com.zhuhao.imtest;

import android.app.Activity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

import com.laixou.imsdk.IMSdk;
import com.laixou.imsdk.IMServiceControll;
import com.laixou.imsdk.MessageBean;

public class MainActivity extends Activity implements View.OnClickListener {


    private static final String TAG = "ceshi";
    Button btnConn;

    Button btnIsconn;

    Button btnSend;

    Button btnClose;

    Button btnSetcallback;
    private IMServiceControll im;
    private IMSdk.ConnListener connListener;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        btnSetcallback = (Button) findViewById(R.id.btn_setcallback);
        btnConn = (Button) findViewById(R.id.btn_conn);
        btnIsconn = (Button) findViewById(R.id.btn_isconn);
        btnSend = (Button) findViewById(R.id.btn_send);
        btnClose = (Button) findViewById(R.id.btn_close);

        btnSetcallback.setOnClickListener(this);
        btnConn.setOnClickListener(this);
        btnIsconn.setOnClickListener(this);
        btnSend.setOnClickListener(this);
        btnClose.setOnClickListener(this);

        connListener = new IMSdk.ConnListener() {
            @Override
            public void onConn() {
                Log.d(TAG, "conn success:");
            }

            @Override
            public void onDisconn() {
                Log.d(TAG, "disconn:");
            }

            @Override
            public void getData() {
                Log.d(TAG, "Get data:");
            }

            @Override
            public void onConnErr() {
                Log.d(TAG, "conn failed:");
            }
        };
        im = new TestConn();
    }

    int code = 0;


    public void onClick(View view) {
        switch (view.getId()) {
            case R.id.btn_setcallback:
                im.setCallback(connListener);
                break;
            case R.id.btn_conn:
                im.connect();
                break;
            case R.id.btn_isconn:
                int conn = im.isConn();
                Log.d(TAG, "是否连接:" + conn);
                break;
            case R.id.btn_send:
                MessageBean msg = new MessageBean(code, 123, 123, 1, 0, 0, "123123", "1.1.1", "测试消息啊", 0, "是是是");
                code = 2;
                im.send(msg);
                break;
            case R.id.btn_close:
                im.close();
                break;
        }
    }

    public class TestConn implements IMServiceControll {

        @Override
        public void setCallback(IMSdk.ConnListener callback) {
            IMSdk.getInstance().setCallback(callback);
        }

        @Override
        public void connect() {
            new Thread() {
                @Override
                public void run() {
                    super.run();
                    IMSdk.getInstance().connect("120.76.96.153", 8081);
                }
            }.start();
        }

        @Override
        public void send(MessageBean msg) {
            IMSdk.getInstance().send(msg);
        }

        @Override
        public int isConn() {
            return IMSdk.getInstance().isConn();
        }

        @Override
        public void close() {
            IMSdk.getInstance().close();
        }
    }
}
