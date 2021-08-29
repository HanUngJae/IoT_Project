package com.example.shcsmarthomecare;

import androidx.appcompat.app.AppCompatActivity;

import android.content.ContentValues;
import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.view.View;

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

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        //String json;

        //HttpConnector thread = new HttpConnector();
        //thread.start();
        //json = thread.getJson();

        //Log.d("jsonparsing","json in main : " + json);
    }


    public void heaterClicked(View view) {
        Intent intent = new Intent(this, HeaterActivity.class);
        startActivity(intent);
    }

    public void ledClicked(View view) {
        Intent intent = new Intent(this, LedActivity.class);
        startActivity(intent);
    }

    public void valveClicked(View view) {
        Intent intent = new Intent(this, ValveActivity.class);
        startActivity(intent);
    }

    public void electricClicked(View view) {
        Intent intent = new Intent(this, ElectricActivity.class);
        startActivity(intent);
    }

}