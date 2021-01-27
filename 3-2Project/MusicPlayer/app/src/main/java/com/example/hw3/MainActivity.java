package com.example.hw3;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;
import androidx.core.app.ActivityCompat;
import androidx.core.content.ContextCompat;

import android.Manifest;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.database.Cursor;
import android.media.MediaScannerConnection;
import android.os.Bundle;
import android.os.Environment;
import android.provider.MediaStore;
import android.util.Log;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ListView;
import android.widget.Toast;

import java.util.ArrayList;

public class MainActivity extends AppCompatActivity {
    ArrayList<Musics> musicList;
    ListView listView;
    ItemAdapter itemAdapter = null;
    boolean checked = false;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        String permission = Manifest.permission.READ_EXTERNAL_STORAGE;

        if(ContextCompat.checkSelfPermission(this,
                permission) != PackageManager.PERMISSION_GRANTED){
            ActivityCompat.requestPermissions(this, new String[]{permission}, 1);
        }
        else{
            checked = true;
        }

        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        if(checked){
            musicList = new ArrayList<Musics>();

            String selection = MediaStore.Audio.Media.IS_MUSIC + " != 0";
            String[] projection = {
                    MediaStore.Audio.Media.DATA,//path
                    MediaStore.Audio.Media.DISPLAY_NAME,//파일이름
                    MediaStore.Audio.Media.DURATION //재생시간
            };

            Cursor cursor = getContentResolver().query(
                    MediaStore.Audio.Media.EXTERNAL_CONTENT_URI,
                    projection, selection, null, null);


            while(cursor.moveToNext()) {
                musicList.add(new Musics(cursor.getString(0), cursor.getString(1), Integer.parseInt(cursor.getString(2))));
            }
            cursor.close();
            MusicList _musicList = MusicList.getInstance(musicList);

            listView = (ListView)findViewById(R.id.list_view);
            itemAdapter = new ItemAdapter(this, musicList);
            listView.setAdapter(itemAdapter);

            listView.setOnItemClickListener(new AdapterView.OnItemClickListener() {
                @Override
                public void onItemClick(AdapterView<?> adapterView, View view, int position, long id) {
                    Log.d("asd", "now clicked music: " + musicList.get(position).getMusicName());
                    Intent intent = new Intent(MainActivity.this, PlayMusicActivity.class);
                    intent.putExtra("MUSIC_INDEX", position);
                    startActivity(intent);
                }
            });
        }
    }

    @Override
    public void onRequestPermissionsResult(int requestCode, @NonNull String[] permissions, @NonNull int[] grantResults) {
        if(requestCode == 1){
            if(grantResults[0] == PackageManager.PERMISSION_GRANTED){
                checked = true;
                startActivity(new Intent(MainActivity.this, MainActivity.class));
            }
            else{
                Toast.makeText(MainActivity.this, "권한을 수락해주세요.", Toast.LENGTH_SHORT).show();
            }
        }
    }


}