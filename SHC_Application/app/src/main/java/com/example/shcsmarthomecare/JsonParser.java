package com.example.shcsmarthomecare;

import android.util.Log;

import com.google.gson.JsonObject;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.util.ArrayList;

public class JsonParser {

    public static void jsonParser(String resultJson) {
        try{

            JSONObject jsonObject = new JSONObject(resultJson);
            //String userId = jsonObject.getString("userId");
            String id = jsonObject.getString("title");
            //String title = jsonObject.getString("title");
            String body = jsonObject.getString("body");


            //Log.d("JasonParsing", "Select line : " + userId);
            Log.d("JasonParsing", "Select line id : " + id);
            //Log.d("JasonParsing", "Select line : " + title);
            Log.d("JasonParsing", "Select line body : " + body);


        }catch(JSONException e) {
        }
    }
}
