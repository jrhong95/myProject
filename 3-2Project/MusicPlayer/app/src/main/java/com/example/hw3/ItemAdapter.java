package com.example.hw3;

import android.content.Context;
import android.graphics.Bitmap;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.ImageView;
import android.widget.TextView;

import java.util.ArrayList;

public class ItemAdapter extends BaseAdapter {
    Context mContext;
    ArrayList<Musics> musicsArrayList;
    LayoutInflater mLayoutInflater;

    public ItemAdapter(Context mContext, ArrayList<Musics> musicsArrayList){
        this.mContext = mContext;
        this.musicsArrayList = musicsArrayList;
        mLayoutInflater = LayoutInflater.from(mContext);
        Log.e("asd","ItemAdapter : Constructor");
    }

    @Override
    public int getCount() {
        return musicsArrayList.size();
    }

    @Override
    public Musics getItem(int i) {
        return musicsArrayList.get(i);
    }

    @Override
    public long getItemId(int i) {
        return i;
    }

    @Override
    public View getView(int pos, View view, ViewGroup viewGroup) {
        View itemView = view;
        if(itemView == null){
            itemView = mLayoutInflater.inflate(R.layout.list_view_item, null);
        }

        TextView textView = (TextView)itemView.findViewById(R.id.music_name_text);
        textView.setText(getItem(pos).getMusicName());

        ImageView imageView = (ImageView)itemView.findViewById(R.id.album_art_img);
        Bitmap img = musicsArrayList.get(pos).getAlbumArt();
        if(img != null){
            imageView.setImageBitmap(img);
        }
        else{
            imageView.setImageResource(R.drawable.no_cover);
        }

        return itemView;
    }
}
