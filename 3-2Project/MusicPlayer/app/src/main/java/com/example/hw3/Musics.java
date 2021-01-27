package com.example.hw3;


import android.content.ContentResolver;
import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.media.MediaMetadataRetriever;
import android.util.Log;

public class Musics {
    String musicPath;
    String musicName;
    int musicDuration;

    public Musics(String musicPath, String musicName, int musicDuration){
        this.musicPath = musicPath;
        this.musicName = musicName;
        this.musicDuration = musicDuration;
    }

    public String getMusicName(){
        return musicName;
    }

    public String getMusicPath(){
        return musicPath;
    }

    public int getMusicDuration(){
        return musicDuration;
    }

    public Bitmap getAlbumArt(){
        Log.e("asd","Musics: getAlbumArt " + musicPath);
        MediaMetadataRetriever mmr = new MediaMetadataRetriever();
        mmr.setDataSource(musicPath);
        byte [] data = mmr.getEmbeddedPicture();
        Log.e("asd","Musics: getAlbumArt");
        return data != null ? BitmapFactory.decodeByteArray(data, 0, data.length) : null;
    }
}