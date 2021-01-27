package com.example.hw3;

import android.media.MediaPlayer;

import java.util.ArrayList;

public class MusicList {
    private ArrayList<Musics> musicsArrayList;
    public static MusicList musicList = null;

    public static MusicList getInstance(ArrayList<Musics> musicsArrayList){
        if(musicList == null){
            musicList = new MusicList(musicsArrayList);
        }
        return musicList;
    }

    public static MusicList getInstance(){
        return musicList;
    }

    public MusicList(ArrayList<Musics> musicsArrayList){
        this.musicsArrayList = musicsArrayList;
    }

    public ArrayList<Musics> getMusicsArrayList() {
        return musicsArrayList;
    }
}
