package com.laixou.imsdk;

/**
 * 作者: Administrator
 * 时间: 2016/6/1:17:01
 * 说明:
 */
public class MessageBean {
    int cmd;//消息类型
    int sender;//发送人
    int receiver;//接收人
    int msgid;//消息id
    int msgscope;//消息范围,单人或群发
    int device;//设备
    int status;//消息状态
    String token;//token,最大32个字节.
    String version;//版本,最大16个字节
    String body;//消息内容,最大512个字节.
    String chk;//校验码 最大32个字节.

    public MessageBean(
            int cmd,//消息类型
            int sender,//发送人
            int receiver,//接收人
            int msgid,//userId
            int msgscope,//消息范围,单人或群发
            int device,//设备
            String token,//token,最大32个字节.
            String version,//版本,最大16个字节
            String body,//消息内容,最大512个字节.
            int status,//消息状态
            String chk) {//校验码 最大32个字节.
        this.cmd = cmd;
        this.sender = sender;
        this.receiver = receiver;
        this.msgid = msgid;
        this.msgscope = msgscope;
        this.device = device;
        this.token = token;
        this.version = version;
        this.body = body;
        this.status = status;
        this.chk = chk;
    }

    public MessageBean() {
    }

    public int getCmd() {
        return cmd;
    }

    public void setCmd(int cmd) {
        this.cmd = cmd;
    }

    public int getSender() {
        return sender;
    }

    public void setSender(int sender) {
        this.sender = sender;
    }

    public int getReceiver() {
        return receiver;
    }

    public void setReceiver(int receiver) {
        this.receiver = receiver;
    }

    public int getMsgid() {
        return msgid;
    }

    public void setMsgid(int msgid) {
        this.msgid = msgid;
    }

    public int getMsgscope() {
        return msgscope;
    }

    public void setMsgscope(int msgscope) {
        this.msgscope = msgscope;
    }

    public int getDevice() {
        return device;
    }

    public void setDevice(int device) {
        this.device = device;
    }

    public String getToken() {
        return token;
    }

    public void setToken(String token) {
        this.token = token;
    }

    public String getVersion() {
        return version;
    }

    public void setVersion(String version) {
        this.version = version;
    }

    public String getBody() {
        return body;
    }

    public void setBody(String body) {
        this.body = body;
    }

    public int getStatus() {
        return status;
    }

    public void setStatus(int status) {
        this.status = status;
    }

    public String getChk() {
        return chk;
    }

    public void setChk(String chk) {
        this.chk = chk;
    }
}
