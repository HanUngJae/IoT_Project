package com.example.shcsmarthomecare;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;

public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
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