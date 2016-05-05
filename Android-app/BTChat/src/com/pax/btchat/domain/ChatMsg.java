package com.pax.btchat.domain;

public class ChatMsg {

	public static final int TYPE_RECEIVED = 0;
	public static final int TYPE_SEND = 1;
	
	private int type;
	private String content;
	
	public ChatMsg(int type, String content) {
		this.type = type;
		this.content = content;
	}
	
	public int getType() {
		return type;
	}
	public void setType(int type) {
		this.type = type;
	}
	public String getContent() {
		return content;
	}
	public void setContent(String content) {
		this.content = content;
	}
	
}
