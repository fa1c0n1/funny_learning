package com.laixou.imsdk;

/**
 * 作者: Administrator
 * 时间: 2016/6/2:11:07
 * 说明:
 */
public interface IMServiceControll {
    void setCallback(IMSdk.ConnListener callback);

    void connect();

    void send(MessageBean msg);

    int isConn();

    void close();
}
