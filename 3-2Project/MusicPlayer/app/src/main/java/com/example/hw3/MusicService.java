package com.example.hw3;


import android.app.NotificationChannel;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.app.Service;
import android.content.Context;
import android.content.Intent;
import android.graphics.Bitmap;
import android.media.MediaPlayer;
import android.os.Build;
import android.os.IBinder;
import android.util.Log;
import android.widget.RemoteViews;

import androidx.core.app.NotificationCompat;

import java.io.IOException;
import java.util.ArrayList;

public class MusicService extends Service implements MediaPlayer.OnPreparedListener {
    private static int NOTIFICATION_ID = 951214;
    private static String ACTION_INIT = "myMusicPlayer.INIT";
    private static String ACTION_PLAY = "myMusicPlayer.START";
    private static String ACTION_PREV = "myMusicPlayer.PREV";
    private static String ACTION_NEXT = "myMusicPlayer.NEXT";
    private static String BROADCAST_PLAY_STATE_CHANGED = "broadcast.changed";

    MediaPlayer mediaPlayer;
    ArrayList<Musics> musicList;
    RemoteViews remoteViews;
    NotificationManager mNotificationManager;
    NotificationCompat.Builder mBuilder;
    int idx;
    boolean isPlaying;
    boolean isChanged;


    @Override
    public void onCreate() {
        super.onCreate();

        Log.d("asd", "Service onCreate " + idx);

        mediaPlayer = new MediaPlayer();
        mediaPlayer.setOnCompletionListener(mOnComplete);
        mediaPlayer.setOnErrorListener(mOnError);
        musicList = MusicList.getInstance().getMusicsArrayList();

        idx = 0;
        isPlaying = false;
        isChanged = false;
        startForegroundService();
    }

    @Override
    public IBinder onBind(Intent intent) {
        return null;
    }

    public void sendBR(){
        Intent intent = new Intent();
        intent.setAction(BROADCAST_PLAY_STATE_CHANGED);
        intent.putExtra("CURRENT_INDEX", idx);
        intent.putExtra("CURRENT_STATUS", isPlaying);

        int duration = musicList.get(idx).getMusicDuration();
        int curPosition = mediaPlayer.getCurrentPosition();
        intent.putExtra("DURATION", duration);
        intent.putExtra("CUR_POSITION", curPosition);

        sendBroadcast(intent);
    }

    @Override
    public int onStartCommand(Intent intent, int flags, int startId) {
        if(intent.getAction().equals(ACTION_INIT)){
            Log.d("Intent", "INIT");
            if(isPlaying){
                isPlaying = true;
                int tempIdx = intent.getIntExtra("MUSIC_INDEX", 0);
                if(idx != tempIdx){
                    idx = tempIdx;
                    mediaPlayer.reset();
                    loadMedia(idx);
                    sendBR();
                    updateNotificationView();
                }
                else{
                    sendBR();
                    updateNotificationView();
                }
            }
            else{
                isPlaying = true;
                idx = intent.getIntExtra("MUSIC_INDEX", 0);
                if(!mediaPlayer.isPlaying() && mediaPlayer.getCurrentPosition() > 1){
                    mediaPlayer.reset();
                }
                loadMedia(idx);
                sendBR();
                updateNotificationView();
            }
        }
        else if(intent.getAction().equals(ACTION_PLAY)){
            Log.d("Intent", "PLAY");
            if(isPlaying){
                isPlaying = false;
                mediaPlayer.pause();
                sendBR();
                updateNotificationView();
            }
            else{
                isPlaying = true;
                mediaPlayer.start();
                sendBR();
                updateNotificationView();
            }
        }
        else if(intent.getAction().equals(ACTION_PREV)){
            Log.d("Intent", "PREV");
            idx = idx == 0 ? musicList.size() - 1 : idx - 1;
            mediaPlayer.reset();
            loadMedia(idx);
            sendBR();
            if(!isPlaying)
                isChanged = true;
            updateNotificationView();
            isPlaying = true;
        }
        else if(intent.getAction().equals(ACTION_NEXT)){
            Log.d("Intent", "NEXT");
            idx = idx == musicList.size() - 1 ? 0 : idx + 1;
            mediaPlayer.reset();
            loadMedia(idx);
            sendBR();
            if(!isPlaying)
                isChanged = true;
            updateNotificationView();
            isPlaying = true;
        }
        return START_STICKY;
    }

    public boolean loadMedia(int idx){
        try {
            mediaPlayer.setDataSource(musicList.get(idx).musicPath);
            mediaPlayer.setOnPreparedListener(this);
            mediaPlayer.prepareAsync();
        } catch (IOException e) {
            e.printStackTrace();
            return false;
        }
        return true;
    }

    MediaPlayer.OnCompletionListener mOnComplete = new MediaPlayer.OnCompletionListener() {
    @Override
        public void onCompletion(MediaPlayer mediaPlayer) {
            idx = idx == musicList.size() - 1 ? 0 : idx + 1;
            mediaPlayer.reset();
            loadMedia(idx);
            sendBR();
            updateNotificationView();
            mediaPlayer.start();
        }
    };


    MediaPlayer.OnErrorListener mOnError = new MediaPlayer.OnErrorListener() {
        @Override
        public boolean onError(MediaPlayer mediaPlayer, int i, int i1) {
            String err = "OnError occured. what=" + i + ", extra=" + i1;
            return false;
        }
    };

    @Override
    public void onDestroy() {
        Log.d("asd", "service onDestroy");
        mediaPlayer.reset();
        mediaPlayer = null;
        stopForeground(true);
        stopSelf();
        super.onDestroy();
    }

    @Override
    public void onPrepared(MediaPlayer mediaPlayer) {
        mediaPlayer.start();
    }

    void startForegroundService() {
        mNotificationManager = (NotificationManager)getSystemService(Context.NOTIFICATION_SERVICE);

        Intent notibarIntent = new Intent(this, PlayMusicActivity.class);
        notibarIntent.putExtra("MUSIC_INDEX", idx);
        notibarIntent.setFlags(Intent.FLAG_ACTIVITY_SINGLE_TOP );
        PendingIntent mainPendingIntent = PendingIntent.getActivity(this,
                0, notibarIntent, PendingIntent.FLAG_CANCEL_CURRENT);

        remoteViews = new RemoteViews(getPackageName(), R.layout.notification_layout);
        remoteViews.setOnClickPendingIntent(R.id.notification_album_img, mainPendingIntent);

        if (Build.VERSION.SDK_INT >= 26) {
            String CHANNEL_ID = "MyMusicPlayer_ID";
            NotificationChannel channel = new NotificationChannel(CHANNEL_ID,
                    "MyMusicPlayer_ID", NotificationManager.IMPORTANCE_DEFAULT);

            mNotificationManager.createNotificationChannel(channel);

            mBuilder = new NotificationCompat.Builder(this, CHANNEL_ID);
        }
        else {
            mBuilder = new NotificationCompat.Builder(this);
        }
        mBuilder.setSmallIcon(R.drawable.music_start)
                .setContent(remoteViews);

        Intent notiStartBtn = new Intent(ACTION_PLAY);
        Intent notiPrevBtn = new Intent(ACTION_PREV);
        Intent notiNextBtn = new Intent(ACTION_NEXT);
        PendingIntent notiStart = PendingIntent.getService(this, 0, notiStartBtn, 0);
        PendingIntent notiPrev = PendingIntent.getService(this, 0, notiPrevBtn, 0);
        PendingIntent notiNext = PendingIntent.getService(this, 0, notiNextBtn, 0);

        remoteViews.setOnClickPendingIntent(R.id.notification_start_btn, notiStart);
        remoteViews.setOnClickPendingIntent(R.id.notification_prev_btn, notiPrev);
        remoteViews.setOnClickPendingIntent(R.id.notification_next_btn, notiNext);

        startForeground(NOTIFICATION_ID, mBuilder.build());
    }

    void updateNotificationView(){
        Musics curMusic = musicList.get(idx);
        Bitmap albumart = curMusic.getAlbumArt();

        if(albumart != null)
            remoteViews.setImageViewBitmap(R.id.notification_album_img, curMusic.getAlbumArt());
        else
            remoteViews.setImageViewResource(R.id.notification_album_img, R.drawable.no_cover);
        remoteViews.setTextViewText(R.id.notification_music_name, curMusic.getMusicName());

        if(isPlaying){
            remoteViews.setImageViewResource(R.id.notification_start_btn, R.drawable.music_pause);
            mBuilder.setSmallIcon(R.drawable.music_start);
        }
        else{
            if (isChanged) {
                remoteViews.setImageViewResource(R.id.notification_start_btn, R.drawable.music_pause);
                mBuilder.setSmallIcon(R.drawable.music_start);
                isChanged = false;
            }
            else{
                remoteViews.setImageViewResource(R.id.notification_start_btn, R.drawable.music_start);
                mBuilder.setSmallIcon(R.drawable.music_pause);
            }
        }

        mNotificationManager.notify(NOTIFICATION_ID, mBuilder.build());
    }
}
