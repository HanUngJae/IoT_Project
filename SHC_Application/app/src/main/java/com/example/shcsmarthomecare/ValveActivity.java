package com.example.shcsmarthomecare;

import androidx.appcompat.app.AppCompatActivity;

import android.app.TimePickerDialog;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.RadioButton;
import android.widget.TimePicker;
import android.widget.Toast;

import org.json.JSONException;
import org.json.JSONObject;

public class ValveActivity extends AppCompatActivity {

    public Button start_time_btn;
    public Button end_time_btn;
    private String json;
    private TimePickerDialog.OnTimeSetListener start_callbackMethod;
    private TimePickerDialog.OnTimeSetListener end_callbackMethod;
    private RadioButton on;
    private RadioButton auto;
    private RadioButton off;
    private String startTime;
    private String endTime;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_valve);

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

            String valveState = jsonObject.getString("valveState");
            String valveStarTime = jsonObject.getString("valveStartTime");
            String valveEndTime = jsonObject.getString("valveEndTime");

            Log.d("JasonParsing", "Select line valveState : " + valveState);
            Log.d("JasonParsing", "Select line valveStartTime : " + valveStarTime);
            Log.d("JasonParsing", "Select line valveEndTime : " + valveEndTime);

            if(valveState.equals("1")){
                on.setChecked(true);
            }
            else if(valveState.equals("2")){
                auto.setChecked(true);
            }
            else{
                off.setChecked(true);
            }
            startTime = valveStarTime.substring(0,2) + "-" + valveStarTime.substring(3,5);
            endTime = valveEndTime.substring(0,2) + "-" + valveEndTime.substring(3,5);
            start_time_btn.setText(valveStarTime.substring(0,2) + "시 " + valveStarTime.substring(3,5) + "분");
            end_time_btn.setText(valveEndTime.substring(0,2) + "시 " + valveEndTime.substring(3,5)+"분");

        } catch (JSONException e) {
            e.printStackTrace();
        }
    }

    public void InitializeView()
    {
        start_time_btn = (Button)findViewById(R.id.start_time_valve_btn);
        end_time_btn = (Button)findViewById(R.id.end_time_valve_btn);
        on = (RadioButton)findViewById(R.id.on_Radiobtn);
        auto = (RadioButton)findViewById(R.id.auto_radiobtn);
        off = (RadioButton)findViewById(R.id.off_radiobtn);
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

    public void back_valve_btnClicked(View view) {
        finish();
    }

    public void save_valve_btnClicked(View view) {

        JSONObject jsonObject = new JSONObject();
        try {
            jsonObject.put("valve","Valve_Controller");

            if(on.isChecked() == true)
                jsonObject.put("valveState","1");
            else if(auto.isChecked() == true)
                jsonObject.put("valveState","2");
            else
                jsonObject.put("valveState","0");

            jsonObject.put("valveStartTime",startTime);
            jsonObject.put("valveEndTime",endTime);
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