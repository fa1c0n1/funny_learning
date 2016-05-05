package com.pax.btchat.ui;

import java.util.List;

import com.pax.btchat.R;
import com.pax.btchat.domain.ChatMsg;

import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;
import android.widget.LinearLayout;
import android.widget.TextView;

public class ChatAdapter extends ArrayAdapter<ChatMsg> {
	
	private int resId;

	public ChatAdapter(Context context, int textViewResourceId, List<ChatMsg> objects) {
		super(context, textViewResourceId, objects);
		this.resId = textViewResourceId;
	}

	@Override
	public View getView(int position, View convertView, ViewGroup parent) {
		ChatMsg cmsg = getItem(position);
		View mView;
		ViewHolder mViewHolder;
		if (convertView == null) {
			mView = LayoutInflater.from(getContext()).inflate(resId, null);
			mViewHolder = new ViewHolder();
			mViewHolder.layoutLeft = (LinearLayout) mView.findViewById(R.id.layout_left);
			mViewHolder.layoutRight = (LinearLayout) mView.findViewById(R.id.layout_right);
			mViewHolder.tvMsgLeft = (TextView) mView.findViewById(R.id.tv_msg_left);
			mViewHolder.tvMsgRight = (TextView) mView.findViewById(R.id.tv_msg_right);
			mView.setTag(mViewHolder);
		} else {
			mView = convertView;
			mViewHolder = (ViewHolder) mView.getTag();
		}
		
		if (cmsg.getType() == ChatMsg.TYPE_RECEIVED) {
			mViewHolder.layoutLeft.setVisibility(View.VISIBLE);
			mViewHolder.layoutRight.setVisibility(View.GONE);
			mViewHolder.tvMsgLeft.setText(cmsg.getContent());
		} else if (cmsg.getType() == ChatMsg.TYPE_SEND) {
			mViewHolder.layoutRight.setVisibility(View.VISIBLE);
			mViewHolder.layoutLeft.setVisibility(View.GONE);
			mViewHolder.tvMsgRight.setText(cmsg.getContent());
		}
		
		return mView;
	}

	class ViewHolder {
		LinearLayout layoutLeft;
		LinearLayout layoutRight;
		TextView tvMsgLeft;
		TextView tvMsgRight;
	}
}
