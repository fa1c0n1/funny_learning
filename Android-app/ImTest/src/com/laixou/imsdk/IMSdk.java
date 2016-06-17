package com.laixou.imsdk;

public class IMSdk {
    static {
        System.loadLibrary("imSDK");
    }

    private static IMSdk mInstance = new IMSdk();

    private IMSdk() {
    }

    public static IMSdk getInstance() {
        return mInstance;
    }

    //连接到服务器
    //发送消息
    public native void send(MessageBean msg);
    //断开连接
    //判断是否连接
    public native void setCallback(ConnListener callback);
    public native void connect(String host,int port);
    public native int isConn();
    public native void close();

    public interface ConnListener {
        void onConn();

        void onDisconn();

        void getData();

        void onConnErr();
    }
}
