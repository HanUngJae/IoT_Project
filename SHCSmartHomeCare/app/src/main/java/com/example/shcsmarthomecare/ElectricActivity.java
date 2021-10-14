package com.example.shcsmarthomecare;

import androidx.appcompat.app.AppCompatActivity;

import android.app.TimePickerDialog;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.RadioButton;
import android.widget.Switch;
import android.widget.TimePicker;
import android.widget.Toast;

import org.json.JSONException;
import org.json.JSONObject;

public class ElectricActivity extends AppCompatActivity {

    private String json;
    private Button start_time_btn;
    private Button end_time_btn;
    private Button save_btn;
    private TimePickerDialog.OnTimeSetListener start_callbackMethod;
    private TimePickerDialog.OnTimeSetListener end_callbackMethod;
    private RadioButton on;
    private RadioButton off;
    private RadioButton auto;
    private String startTime;
    private String endTime;
    private Button back_btn;
    private Switch multiState_switch;
    private Switch multiTimeState_switch;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_electric);
        try{
            HttpConnector thread = new HttpConnector();
            thread.setDaemon(true);
            thread.start();
            json = thread.getJson();
        }catch(Exception e){
            e.printStackTrace();
        }


        this.InitializeView();
        this.InitializeListener();

        try{
            JSONObject jsonObject = new JSONObject(json);

            String multitabState = jsonObject.getString("multitabState");
            String multitabTime = jsonObject.getString("multitabTime");
            String multitabStartTime = jsonObject.getString("multitabStartTime");
            String multitabEndTime = jsonObject.getString("multitabEndTime");

            Log.d("JasonParsing", "Select line multitabState : " + multitabState);
            Log.d("JasonParsing", "Select line multitabTime : " + multitabTime);
            Log.d("JasonParsing", "Select line multitabStartTime : " + multitabStartTime);
            Log.d("JasonParsing", "Select line multitabEndTime : " + multitabEndTime);

            if(multitabState.equals("1"))
                multiState_switch.setChecked(true);
            else
                multiState_switch.setChecked(false);

            if(multitabTime.equals("1"))
                multiTimeState_switch.setChecked(true);
            else
                multiTimeState_switch.setChecked(false);


            startTime = multitabStartTime.substring(0,2) + "-" + multitabStartTime.substring(3,5);
            endTime = multitabEndTime.substring(0,2) + "-" + multitabEndTime.substring(3,5);
            start_time_btn.setText(multitabStartTime.substring(0,2) + "시 " + multitabStartTime.substring(3,5) + "분");
            end_time_btn.setText(multitabEndTime.substring(0,2) + "시 " + multitabEndTime.substring(3,5)+"분");

        } catch (JSONException e) {
            e.printStackTrace();
        }
    }

    public void InitializeView()
    {
        start_time_btn = (Button)findViewById(R.id.start_time_multi_btn);
        end_time_btn = (Button)findViewById(R.id.end_time_multi_btn);
        save_btn = (Button)findViewById(R.id.save_multitab_btn);
        back_btn = (Button)findViewById(R.id.back_btn4);
        multiState_switch = (Switch)findViewById(R.id.multitab_switch);
        multiTimeState_switch = (Switch)findViewById(R.id.multitab_timer_switch);
    }

    public void InitializeListener()
    {
        start_callbackMethod = new TimePickerDialog.OnTimeSetListener() {
            @Override
            public void onTimeSet(TimePicker view, int hourOfDay, int minute) {
                start_time_btn.setText(hourOfDay + "시 " + minute + "분");
                if(hourOfDay < 10)
                    startTime = "0" + Integer.toString(hourOfDay)+"-";
                else
                    startTime = Integer.toString(hourOfDay)+"-";
                if(minute < 10)
                    startTime += "0" + Integer.toString(minute);
                else
                    startTime += Integer.toString(minute);

                Log.d("time reset","time : " + startTime);
            }
        };

        end_callbackMethod = new TimePickerDialog.OnTimeSetListener() {
            @Override
            public void onTimeSet(TimePicker view, int hourOfDay, int minute) {
                end_time_btn.setText(hourOfDay + "시 " + minute + "분");
                if(hourOfDay < 10)
                    endTime = "0" + Integer.toString(hourOfDay)+"-";
                else
                    endTime = Integer.toString(hourOfDay)+"-";
                if(minute < 10)
                    endTime += "0" + Integer.toString(minute);
                else
                    endTime += Integer.toString(minute);
                Log.d("time reset","time : " + endTime);
            }
        };
    }

    public void start_time_btnClicked(View view) {
        TimePickerDialog dialog = new TimePickerDialog(this, start_callbackMethod, 12, 0, true);
        dialog.show();
    }

    public void end_time_btnClicked(View view) {
        TimePickerDialog dialog = new TimePickerDialog(this, end_callbackMethod, 12, 0, true);
        dialog.show();
    }

    public void back_btnClicked(View view) {
        finish();
    }

    public void save_multitab_btnClicked(View view) {
        JSONObject jsonObject = new JSONObject();
        try {
            jsonObject.put("multitab","Multitab_Controller");

            if(multiState_switch.isChecked() == true)
                jsonObject.put("multitabState","1");
            else
                jsonObject.put("multitabState","0");

            if(multiTimeState_switch.isChecked() == true)
                jsonObject.put("multitabTime","1");
            else
                jsonObject.put("multitabTime","0");

            jsonObject.put("multitabStartTime",startTime);
            jsonObject.put("multitabEndTime",endTime);
        }catch (JSONException e){
            e.printStackTrace();
        }Log.d("json","생성한 json : " +  jsonObject.toString());
        String inputJson = jsonObject.toString();

        HttpConnSender thread = new HttpConnSender();
        thread.handler(inputJson);
        thread.start();
        Toast.makeText(getApplicationContext(),"저장 완료",Toast.LENGTH_SHORT).show();
    }
}