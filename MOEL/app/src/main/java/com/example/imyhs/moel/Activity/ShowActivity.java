package com.example.imyhs.moel.Activity;

import android.app.Activity;
import android.app.ProgressDialog;
import android.content.Intent;
import android.content.pm.ActivityInfo;
import android.os.AsyncTask;
import android.os.Bundle;
import android.os.CountDownTimer;
import android.support.v4.app.FragmentManager;
import android.support.v4.app.FragmentTransaction;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.MotionEvent;
import android.view.View;
import android.view.Window;
import android.view.WindowManager;
import android.widget.Button;
import android.widget.LinearLayout;
import android.widget.ListView;
import android.widget.TextView;
import android.widget.Toast;

import com.example.imyhs.moel.Adapter.NoticeListAdapter;
import com.example.imyhs.moel.Fragment.FindBuildingFragment;
import com.example.imyhs.moel.Fragment.MyBuildingFragment;
import com.example.imyhs.moel.Notice;
import com.example.imyhs.moel.R;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.io.BufferedReader;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.UnsupportedEncodingException;
import java.net.HttpURLConnection;
import java.net.URL;
import java.net.URLEncoder;

/**
 * Created by imyhs on 2017-11-27.
 */

public class ShowActivity extends AppCompatActivity {

    private TextView text1;
    private TextView text2;

    private TextView text_Waiting;
    private TextView text_ElevatorNum;

    @Override
    protected void onCreate(Bundle savedInstanceState) {

        super.onCreate(savedInstanceState);

        requestWindowFeature(Window.FEATURE_NO_TITLE);
        setContentView(R.layout.dialog_call);

        text1 = (TextView) findViewById(R.id.text1);
        text2 = (TextView) findViewById(R.id.text2);
        text_Waiting = (TextView) findViewById(R.id.TextView_Waiting);
        text_ElevatorNum = (TextView) findViewById(R.id.TextView_ElevatorNumber);

        new BackgroundTask().execute();
    }

    class BackgroundTask extends AsyncTask<Void, Void, String>
    {
        String target;

        @Override
        protected void onPreExecute(){
            try {
                target = "http://52.79.39.200/ShowTime.php?userID=" + URLEncoder.encode(MainActivity.userID, "UTF-8");
            } catch (UnsupportedEncodingException e) {
                e.printStackTrace();
            }
        }

        @Override
        protected String doInBackground(Void... voids) {
            try{
                URL url = new URL(target);
                HttpURLConnection httpURLConnection = (HttpURLConnection) url.openConnection();
                InputStream inputStream = httpURLConnection.getInputStream();

                BufferedReader bufferedReader = new BufferedReader(new InputStreamReader(inputStream));
                String temp;
                StringBuilder stringBuilder = new StringBuilder();
                while((temp = bufferedReader.readLine()) != null)
                {
                    stringBuilder.append(temp + "\n");
                }

                bufferedReader.close();
                inputStream.close();
                httpURLConnection.disconnect();
                return stringBuilder.toString().trim();
            }catch(Exception e){
                e.printStackTrace();
            }
            return null;
        }

        @Override
        public void onProgressUpdate(Void... values){
            super.onProgressUpdate();
        }

        @Override
        public void onPostExecute(String result){
            try{
                JSONObject jsonObject = new JSONObject(result);
                JSONArray jsonArray = jsonObject.getJSONArray("response");
                int count = 0;
                String Time, Elevator_Id;

                while(count < jsonArray.length())
                {
                    JSONObject object = jsonArray.getJSONObject(count);
                    Time = object.getString("Time");
                    Elevator_Id = object.getString("Elevator_Id");


                        System.out.println("aaaaaaaaa"+Time);
                        System.out.println("aaaaaaaaa"+Elevator_Id);
                        text_Waiting.setText(Time + "seconds");
                        text_ElevatorNum.setText(Elevator_Id + "번");

                        count++;
                    }
            } catch (JSONException e) {
                e.printStackTrace();
            }
        }
    }

    //확인 버튼 클릭
    public void mOnClose(View v){
        //액티비티(팝업) 닫기
        finish();
    }

    @Override
    public boolean onTouchEvent(MotionEvent event) {
        //바깥레이어 클릭시 안닫히게
        if(event.getAction()==MotionEvent.ACTION_OUTSIDE){
            return false;
        }
        return true;
    }

    @Override
    public void onBackPressed() {
        //안드로이드 백버튼 막기
        return;
    }
}


//    @Override
//    protected  void onCreate(Bundle savedInstanceState){
//        super.onCreate(savedInstanceState);
//
//        requestWindowFeature(Window.FEATURE_NO_TITLE);
//        setContentView(R.layout.dialog_call);
//
//        TextView text1 = (TextView) findViewById(R.id.text1);
//        TextView text2 = (TextView) findViewById(R.id.text2);
//
//        TextView text_Waiting = (TextView) findViewById(R.id.TextView_Waiting);
//        TextView text_ElevatorNum = (TextView) findViewById(R.id.TextView_ElevatorNumber);
//
//        text_Waiting.setText("30.00");
//        text_ElevatorNum.setText("3번");
//    }
//
//    //확인 버튼 클릭
//    public void mOnClose(View v){
//        //액티비티(팝업) 닫기
//        finish();
//    }
//
//    @Override
//    public boolean onTouchEvent(MotionEvent event) {
//        //바깥레이어 클릭시 안닫히게
//        if(event.getAction()==MotionEvent.ACTION_OUTSIDE){
//            return false;
//        }
//        return true;
//    }
//
//    @Override
//    public void onBackPressed() {
//        //안드로이드 백버튼 막기
//        return;
//    }
//
//}
