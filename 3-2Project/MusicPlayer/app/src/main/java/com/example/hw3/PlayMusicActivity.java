package com.example.hw3;


import androidx.annotation.MainThread;
import androidx.appcompat.app.AppCompatActivity;

import android.content.BroadcastReceiver;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.ServiceConnection;
import android.graphics.Bitmap;
import android.os.Bundle;
import android.os.IBinder;
import android.os.RemoteException;
import android.util.Log;
import android.view.View;
import android.widget.ImageView;
import android.widget.ProgressBar;
import android.widget.TextView;

import java.util.ArrayList;
import java.util.Timer;
import java.util.TimerTask;

public class PlayMusicActivity extends AppCompatActivity {
    private static String ACTION_INIT = "myMusicPlayer.INIT";
    private static String ACTION_PLAY = "myMusicPlayer.START";
    private static String ACTION_PREV = "myMusicPlayer.PREV";
    private static String ACTION_NEXT = "myMusicPlayer.NEXT";
    private static String BROADCAST_PLAY_STATE_CHANGED = "broadcast.changed";

    ArrayList<Musics> musicList;
    int midx;
    int currentPosition;
    ImageView mAlbumArt;
    ImageView mPlayBtn;
    ImageView mPrevBtn;
    ImageView mNextBtn;
    TextView mFileName;
    TextView playTime;
    ProgressBar mProgress;
    MusicList _musicList;
    boolean isStart;
    boolean threadFlag;
    Thread thread = null;
    String duration;
    private static Timer job;

    BroadcastReceiver broadcastReceiver;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_music);

        mAlbumArt = (ImageView)findViewById(R.id.player_album_art_img) ;
        mFileName = (TextView)findViewById(R.id.music_text);
        mPlayBtn = (ImageView)findViewById(R.id.start_btn);
        mPrevBtn = (ImageView)findViewById(R.id.prev_btn);
        mNextBtn = (ImageView)findViewById(R.id.next_btn);
        mProgress = (ProgressBar)findViewById(R.id.progressbar);
        playTime = (TextView)findViewById(R.id.progress_time);
        threadFlag = false;
        _musicList = MusicList.getInstance();
        Intent intent = getIntent();
        midx = intent.getIntExtra("MUSIC_INDEX", 0);
        Log.i("asd", "PlayMusicActivity onCreate" + midx);
        musicList = _musicList.getMusicsArrayList();

        IntentFilter intentFilter = new IntentFilter(BROADCAST_PLAY_STATE_CHANGED);
        broadcastReceiver = new BroadcastReceiver() {
            @Override
            public void onReceive(Context context, Intent intent) {
                Log.d("Intent", "onReceive " + intent.getIntExtra("CURRENT_INDEX", 0) +
                        intent.getBooleanExtra("CURRENT_STATUS", false)+ intent.getIntExtra("CUR_POSITION", 0) + intent.getAction());

                updateMusic(intent.getIntExtra("CURRENT_INDEX", 0),
                        intent.getBooleanExtra("CURRENT_STATUS", false), intent.getIntExtra("CUR_POSITION", 0),
                        intent.getIntExtra("DURATION", 0));
            }
        };
        registerReceiver(broadcastReceiver, intentFilter);

        isStart = true;
        Intent serviceIntent = new Intent(PlayMusicActivity.this, MusicService.class);
        serviceIntent.setAction(ACTION_INIT);
        serviceIntent.putExtra("MUSIC_INDEX", midx);
        startService(serviceIntent);
    }

    @Override
    protected void onDestroy() {
        unregisterReceiver(broadcastReceiver);

        super.onDestroy();
    }

    class ProgressChange extends TimerTask{
        @Override
        public void run() {
            runOnUiThread(new Runnable() {
                @Override
                public void run() {
                    currentPosition += 1000;
                    playTime.setText(formatDuration(currentPosition) + " / " + duration);
                    mProgress.setProgress(currentPosition);
                }
            });
        }
    }
    public void setProgressChange(){
        ProgressChange progressChange = new ProgressChange();
        job = new Timer();
        job.scheduleAtFixedRate(progressChange, 1000, 1000);
    }

    public void cancelProgressChange(){
        job.cancel();
        job.purge();
        job = null;
    }

    public static String formatDuration(int duration) {
        int seconds = duration / 1000;
        String time = String.format(
                "%02d:%02d",
                (seconds % 3600) / 60,
                seconds % 60);
        return time;
    }

    public void updateMusic(int idx, boolean isPlaying, int current, final int max){
        currentPosition = current;      //재생위치 받음
        duration = formatDuration(max); //총 재생시간

        if(isPlaying){
            mPlayBtn.setImageResource(R.drawable.music_pause);
            if(idx != midx){    //음악 이동
                midx = idx;
                cancelProgressChange();
                playTime.setText("00:00 / " + duration);
                mProgress.setMax(max);
                mProgress.setProgress(0);

                setProgressChange();
            }
            else{   //새로재생
                Log.d("asd", "start Music");
                threadFlag = true;
                mProgress.setMax(max);
                mProgress.setProgress(0);

                setProgressChange();
            }
        }
        else{   //정지
            if(midx != idx){
                midx = idx;
                if(job != null){
                    cancelProgressChange();
                }
                mPlayBtn.setImageResource(R.drawable.music_pause);
                playTime.setText("00:00 / " + duration);
                mProgress.setMax(max);
                mProgress.setProgress(0);

                setProgressChange();
            }
            else{
                Log.d("asd", "stop Music");
                mPlayBtn.setImageResource(R.drawable.music_start);
                cancelProgressChange();
            }
        }

        Bitmap img = musicList.get(idx).getAlbumArt();
        if(img != null){
            mAlbumArt.setImageBitmap(img);
        }
        else{
            mAlbumArt.setImageResource(R.drawable.no_cover);
        }
        mFileName.setText(musicList.get(idx).getMusicName());
    }

    @Override
    protected void onNewIntent(Intent intent) {
        Log.d("asd", "onNewIntent");
        super.onNewIntent(intent);
    }

    public void onClick(View view) {
        switch (view.getId()){
            case R.id.start_btn:
            {
                Intent serviceIntent = new Intent(PlayMusicActivity.this, MusicService.class);
                serviceIntent.setAction(ACTION_PLAY);
                startService(serviceIntent);
                break;
            }
            case R.id.prev_btn:
            {
                Intent serviceIntent = new Intent(PlayMusicActivity.this, MusicService.class);
                serviceIntent.setAction(ACTION_PREV);
                startService(serviceIntent);
                break;
            }
            case R.id.next_btn:
            {
                Intent serviceIntent = new Intent(PlayMusicActivity.this, MusicService.class);
                serviceIntent.setAction(ACTION_NEXT);
                startService(serviceIntent);
                break;
            }
        }
    }
}