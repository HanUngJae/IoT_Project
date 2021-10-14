package com.example.shcsmarthomecare;

import androidx.appcompat.app.AppCompatActivity;

import android.content.ContentValues;
import android.content.Intent;
import android.os.Bundle;
import android.os.StrictMode;
import android.util.Log;
import android.view.View;
import android.widget.Toast;

import org.json.JSONException;
import org.json.JSONObject;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.HttpURLConnection;
import java.net.MalformedURLException;
import java.net.ProtocolException;
import java.net.URL;

public class MainActivity extends AppCompatActivity {
    private int resCode = 0;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        if (android.os.Build.VERSION.SDK_INT > 9) { StrictMode.ThreadPolicy policy = new StrictMode.ThreadPolicy.Builder().permitAll().build(); StrictMode.setThreadPolicy(policy); }

        URL url = null;
        resCode = 0;

        try {
            url = new URL("http://piflask.iptime.org:5000/json_test");
            HttpURLConnection conn = (HttpURLConnection)url.openConnection();
            conn.setRequestMethod("GET");
            conn.setConnectTimeout(2*1000);
            conn.setReadTimeout(2*1000);

            resCode = conn.getResponseCode();
            Log.d("HttpURLConnection","resCode11111 : " + resCode);
            conn.disconnect();

        } catch (MalformedURLException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }


    public void heaterClicked(View view) {
        Log.d("HttpURLConnection","resCode : " + resCode);
        if(resCode == 200){
            Intent intent = new Intent(this, HeaterActivity.class);
            startActivity(intent);
        }
        else{
            Toast.makeText(getApplicationContext(),"연결 실패",Toast.LENGTH_SHORT).show();
        }
        
    }

    public void ledClicked(View view) {
        if(resCode == 200){
            Intent intent = new Intent(this, LedActivity.class);
            startActivity(intent);
        }
        else{
            Toast.makeText(getApplicationContext(),"연결 실패",Toast.LENGTH_SHORT).show();
        }

    }

    public void valveClicked(View view) {
        if(resCode == 200){
            Intent intent = new Intent(this, ValveActivity.class);
            startActivity(intent);
        }
        else{
            Toast.makeText(getApplicationContext(),"연결 실패",Toast.LENGTH_SHORT).show();
        }


    }

    public void electricClicked(View view) {
        if(resCode == 200){
            Intent intent = new Intent(this, ElectricActivity.class);
            startActivity(intent);
        }
        else{
            Toast.makeText(getApplicationContext(),"연결 실패",Toast.LENGTH_SHORT).show();
        }

    }

}