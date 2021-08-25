package com.example.shcsmarthomecare;

import android.os.Message;
import android.os.Handler;
import android.util.Log;
import android.widget.Toast;


import com.google.gson.JsonObject;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;
import java.io.BufferedReader;
import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.net.HttpURLConnection;
import java.net.MalformedURLException;
import java.net.URL;
import java.util.logging.LogRecord;

public class HttpConnSender extends Thread{

    private String result;
    private String inputJson;

    public void handler(String json){
        this.inputJson = json;
    }

    @Override
    public void run() {
        String response = "";
        InputStream is = null;
        OutputStream os = null;
        ByteArrayOutputStream baos = null;

        try {
            URL url = new URL("http://piflask.iptime.org:5000/get_json");
            HttpURLConnection conn = (HttpURLConnection)url.openConnection();


            conn.setConnectTimeout(5*1000);
            conn.setReadTimeout(5*1000);
            conn.setRequestMethod("POST");
            conn.setRequestProperty("Cache-Control", "no-cache");
            conn.setRequestProperty("Content-Type", "application/json; utf-8");
            conn.setRequestProperty("Accept", "application/json");
            conn.setDoOutput(true);
            conn.setDoInput(true);

            os = conn.getOutputStream();
            os.write(inputJson.getBytes());
            os.flush();

            int resCode = conn.getResponseCode();
            int HTTP_OK = HttpURLConnection.HTTP_OK;

            if(resCode == HTTP_OK) {
                is = conn.getInputStream();
                baos = new ByteArrayOutputStream();
                byte[] byteBuffer = new byte[1024];
                byte[] byteData = null;
                int nLength = 0;
                while((nLength = is.read(byteBuffer,0,byteBuffer.length)) != -1) {
                    baos.write(byteBuffer,0,nLength);
                }
                byteData = baos.toByteArray();
                response = new String(byteData);
                Log.d("JsonParsing","DATA response = " + response);
            }

            Log.d("jasonParsing good", "resCode : " + resCode);
            Log.d("JasonParsing", "HTTP_OK : " + HTTP_OK);

            conn.disconnect();

        }catch (Exception e){
            e.printStackTrace();
        }
    }
}
