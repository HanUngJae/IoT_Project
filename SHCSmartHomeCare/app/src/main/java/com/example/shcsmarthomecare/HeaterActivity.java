package com.example.shcsmarthomecare;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.RadioButton;
import android.widget.SeekBar;
import android.widget.Switch;
import android.widget.TextView;

import org.json.JSONException;
import org.json.JSONObject;


public class HeaterActivity extends AppCompatActivity {

    private String json;
    private TextView textView;
    private SeekBar seekBar;
    private TextView textView_temp;
    private TextView textView_setTemp;
    private Switch heater_switch;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_heater);
        try{
            HttpConnector thread = new HttpConnector();
            thread.setDaemon(true);
            thread.start();
            json = thread.getJson();
        }catch(Exception e){
            e.printStackTrace();
        }

        this.InitializeView();

        try{
            JSONObject jsonObject = new JSONObject(json);

            String heaterState = jsonObject.getString("heaterState");
            String temp = jsonObject.getString("temp");
            String setTemp = jsonObject.getString("setTemp");

            Log.d("JasonParsing", "Select line heaterState : " + heaterState);
            Log.d("JasonParsing", "Select line temp : " + temp);
            Log.d("JasonParsing", "Select line setTemp : " + setTemp);

            if(heaterState.equals("1"))
                heater_switch.setChecked(true);
            else
                heater_switch.setChecked(false);

            textView_temp.setText("현재 온도 : " + temp);
            textView_setTemp.setText("희망 온도 : " + setTemp);

            double tmp = Double.parseDouble(setTemp);
            int result = (int)Math.round(tmp);
            seekBar.setProgress(result);
            textView.setText(setTemp);



       }catch(JSONException e){
            e.printStackTrace();
        }
        seekBar.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
                textView.setText("" + progress);
            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {
            }

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {
            }
        });
    }

    public void InitializeView()
    {
        textView = (TextView) findViewById(R.id.textView13);
        seekBar = (SeekBar) findViewById(R.id.heater_seekBar);
        textView_temp = (TextView)findViewById(R.id.textView9);
        textView_setTemp = (TextView)findViewById(R.id.textView11);
        heater_switch = (Switch)findViewById(R.id.heater_switch);
    }

    public void save_heater_btnClicked(View view){
        JSONObject jsonObject = new JSONObject();
        try {
            jsonObject.put("heater","Heater_Controller");

            if(heater_switch.isChecked() == true)
                jsonObject.put("heaterState","1");
            else
                jsonObject.put("heaterState","0");
            String setTemp = Integer.toString(seekBar.getProgress());
            jsonObject.put("setTemp",setTemp);

        }catch (JSONException e){
            e.printStackTrace();
        }Log.d("json","생성한 json : " +  jsonObject.toString());
        String inputJson = jsonObject.toString();

        HttpConnSender thread = new HttpConnSender();
        thread.handler(inputJson);
        thread.start();
    }
}