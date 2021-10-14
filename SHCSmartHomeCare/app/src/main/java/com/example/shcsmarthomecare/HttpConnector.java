package com.example.shcsmarthomecare;

import android.util.Log;

import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.net.HttpURLConnection;
import java.net.URL;

import okhttp3.Request;


public class HttpConnector extends Thread{
    private String jsonparse = "";
    private volatile boolean done = false;

    @Override
    public void run() {
        try {
            URL url = new URL("http://piflask.iptime.org:5000/json_test");
            HttpURLConnection conn = (HttpURLConnection)url.openConnection();

            if(conn != null){
                conn.setConnectTimeout(10000);
                conn.setReadTimeout(10*1000);
                conn.setRequestMethod("GET");

                int resCode = conn.getResponseCode();
                int HTTP_OK = HttpURLConnection.HTTP_OK;

                Log.d("jasonParsing good", "resCode : " + resCode);
                Log.d("JasonParsing", "HTTP_OK : " + HTTP_OK);

                if(resCode == HTTP_OK){
                    BufferedReader reader = new BufferedReader(new InputStreamReader(conn.getInputStream()));
                    String line = null;

                    while(true) {
                        line = reader.readLine();
                        jsonparse = jsonparse + line;
                        Log.d("JasonParsing", "line : " + line);

                        if (line == null) {
                            break;
                        }
                    }

                    conn.disconnect();
                    reader.close();
                    /****unlock****/
                    done = true;
                    synchronized (this) {
                        this.notifyAll();
                    }
                }
            }

        }catch(Exception e) {
            e.printStackTrace();
        }
    }

    public String getJson() {
        /****lock****/
        if(!done) {
            synchronized (this) {
                try{
                    this.wait();
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }
        }
        return jsonparse;
    }
}
