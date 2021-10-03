package com.example.shcsmarthomecare;

import androidx.appcompat.app.AppCompatActivity;

import android.annotation.SuppressLint;
import android.app.Activity;
import android.content.ContentValues;
import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.Switch;
import android.widget.Toast;

import com.android.volley.AuthFailureError;
import com.android.volley.Request;
import com.android.volley.RequestQueue;
import com.android.volley.Response;
import com.android.volley.VolleyError;
import com.android.volley.toolbox.StringRequest;
import com.android.volley.toolbox.Volley;


import com.kyleduo.switchbutton.SwitchButton;

import org.json.JSONException;
import org.json.JSONObject;

import java.net.HttpURLConnection;
import java.net.URL;
import java.util.HashMap;
import java.util.Map;

import retrofit2.HttpException;


public class LedActivity extends AppCompatActivity {

    private String json = "";
    private Button on_btn;
    private Button off_btn;
    private Button led_save_btn;
    private Switch switchButton1;
    private Switch switchButton2;
    private Switch switchButton3;
    private Switch switchButton4;
    private Button back_btn;

    //Handler handler = new Handler();
    private RequestQueue queue;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_led);
        this.InitializeView();


        try{
            HttpConnector thread = new HttpConnector();
            thread.setDaemon(true);
            thread.start();
            json = thread.getJson();
        }catch (Exception e) {
            e.printStackTrace();
        }

        if(json != null)
        {
            try{
                JSONObject jsonObject = new JSONObject(json);

                String led1 = jsonObject.getString("led1");
                String led2 = jsonObject.getString("led2");
                String led3 = jsonObject.getString("led3");
                String led4 = jsonObject.getString("led4");

                Log.d("JasonParsing", "Select line led1 : " + led1);
                Log.d("JasonParsing", "Select line led2 : " + led2);
                Log.d("JasonParsing", "Select line led3 : " + led3);
                Log.d("JasonParsing", "Select line led4 : " + led4);

                if(led1.equals("1"))
                    switchButton1.setChecked(true);
                else
                    switchButton1.setChecked(false);

                if(led2.equals("1"))
                    switchButton2.setChecked(true);
                else
                    switchButton2.setChecked(false);

                if(led3.equals("1"))
                    switchButton3.setChecked(true);
                else
                    switchButton3.setChecked(false);

                if(led4.equals("1"))
                    switchButton4.setChecked(true);
                else
                    switchButton4.setChecked(false);

            }catch(JSONException e) {
            }
        }


        //this.InitializeListener();

        View.OnClickListener listener = new View.OnClickListener()
        {

            @Override
            public void onClick(View v) {
                switch (v.getId())
                {
                    case R.id.back_btn1:
                        finish();
                        break;

                    case R.id.on_allLed_btn:
                        switchButton1.setChecked(true);
                        switchButton2.setChecked(true);
                        switchButton3.setChecked(true);
                        switchButton4.setChecked(true);
                        break;

                    case R.id.off_allLed_btn:
                        switchButton1.setChecked(false);
                        switchButton2.setChecked(false);
                        switchButton3.setChecked(false);
                        switchButton4.setChecked(false);
                        break;

                    case R.id.led_save_btn:
                        JSONObject jsonObject = new JSONObject();
                        try {
                            jsonObject.put("led","Led_Controller");

                            if(switchButton1.isChecked() == true)
                                jsonObject.put("led1","1");
                            else
                                jsonObject.put("led1","0");

                            if(switchButton2.isChecked() == true)
                                jsonObject.put("led2","1");
                            else
                                jsonObject.put("led2","0");

                            if(switchButton3.isChecked() == true)
                                jsonObject.put("led3","1");
                            else
                                jsonObject.put("led3","0");

                            if(switchButton4.isChecked() == true)
                                jsonObject.put("led4","1");
                            else
                                jsonObject.put("led4","0");

                        }catch (JSONException e){
                            e.printStackTrace();
                        }Log.d("json","생성한 json : " +  jsonObject.toString());
                        String inputJson = jsonObject.toString();

                        HttpConnSender thread = new HttpConnSender();
                        thread.handler(inputJson);
                        thread.start();
                        Toast.makeText(getApplicationContext(),"저장 완료",Toast.LENGTH_SHORT).show();
                        break;
                }

            }
        };
        on_btn.setOnClickListener(listener);
        off_btn.setOnClickListener(listener);
        led_save_btn.setOnClickListener(listener);
        back_btn.setOnClickListener(listener);
    }

    public void InitializeView()
    {
        switchButton1 = (Switch)findViewById(R.id.led_switch1);
        switchButton2 = (Switch)findViewById(R.id.led_switch2);
        switchButton3 = (Switch)findViewById(R.id.led_switch3);
        switchButton4 = (Switch)findViewById(R.id.led_switch4);
        led_save_btn = (Button)findViewById(R.id.led_save_btn);
        on_btn = (Button)findViewById(R.id.on_allLed_btn);
        off_btn = (Button)findViewById(R.id.off_allLed_btn);
        back_btn = (Button)findViewById(R.id.back_btn1);
    }
}