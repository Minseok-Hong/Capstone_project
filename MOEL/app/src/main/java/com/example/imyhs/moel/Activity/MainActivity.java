package com.example.imyhs.moel.Activity;

import android.content.pm.ActivityInfo;
import android.os.AsyncTask;
import android.os.Bundle;
import android.support.v4.app.FragmentManager;
import android.support.v4.app.FragmentTransaction;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.LinearLayout;
import android.widget.ListView;
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
import java.net.HttpURLConnection;
import java.net.URL;
import java.util.ArrayList;
import java.util.List;

public class MainActivity extends AppCompatActivity {

    private ListView noticeListView;
    private NoticeListAdapter noticeListAdapter;
    private List<Notice> noticeList;
    public static String userID;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        //화면 세로 고정
        setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_PORTRAIT);

        userID = getIntent().getStringExtra("userID");
        noticeListView = (ListView) findViewById(R.id.noticeListView);
        noticeList = new ArrayList<Notice>();

        noticeList.add(new Notice("공지사항","남화수","2017-11-18"));
        noticeListAdapter = new NoticeListAdapter(getApplicationContext(), noticeList);
        noticeListView.setAdapter(noticeListAdapter);

        final Button MyBuildingButton = (Button) findViewById(R.id.MyBuildingButton);
        final Button FindBuildingButton = (Button) findViewById(R.id.FindBuildingButton);
        final LinearLayout notice = (LinearLayout)findViewById(R.id.notice);

        MyBuildingButton.setOnClickListener(new View.OnClickListener() {

            @Override
            public void onClick(View v) {
                notice.setVisibility(View.GONE);
                MyBuildingButton.setBackgroundColor(getResources().getColor(R.color.colorGray));
                FindBuildingButton.setBackgroundColor(getResources().getColor(R.color.colorPrimaryMid));
                FragmentManager fragmentManager = getSupportFragmentManager();
                FragmentTransaction fragmentTransaction = fragmentManager.beginTransaction();
                fragmentTransaction.replace(R.id.fragment, new MyBuildingFragment());
                fragmentTransaction.commit();
            }
        });

        FindBuildingButton.setOnClickListener(new View.OnClickListener() {

            @Override
            public void onClick(View v) {
                notice.setVisibility(View.GONE);
                FindBuildingButton.setBackgroundColor(getResources().getColor(R.color.colorGray));
                MyBuildingButton.setBackgroundColor(getResources().getColor(R.color.colorPrimaryMid));
                FragmentManager fragmentManager = getSupportFragmentManager();
                FragmentTransaction fragmentTransaction = fragmentManager.beginTransaction();
                fragmentTransaction.replace(R.id.fragment, new FindBuildingFragment());
                fragmentTransaction.commit();
            }
        });

        new BackgroundTask().execute();
    }

    class BackgroundTask extends AsyncTask<Void, Void, String>
    {
        String target;

        @Override
        protected void onPreExecute(){
            target = "http://52.79.39.200//NoticeList.php";
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
               String noticeContent, noticeName, noticeDate;
               while(count < jsonArray.length())
               {
                   JSONObject object = jsonArray.getJSONObject(count);
                   noticeContent = object.getString("noticeContent");
                   noticeName = object.getString("noticeName");
                   noticeDate = object.getString("noticeDate");
                   Log.d("HWASU",noticeContent);
                   Notice notice = new Notice(noticeContent, noticeName, noticeDate);
                   noticeList.add(notice);
                   noticeListAdapter.notifyDataSetChanged();
                   count++;
               }
           } catch (JSONException e) {
               e.printStackTrace();
           }
       }
    }

    private long lastTimeBackPressed;

    @Override
    public void onBackPressed(){
        if(System.currentTimeMillis() - lastTimeBackPressed < 1500){
            finish();
            return;
        }
        Toast.makeText(this, "'뒤로'버튼을 한번 더 눌러 종료합니다.", Toast.LENGTH_SHORT).show();
        lastTimeBackPressed = System.currentTimeMillis();
    }
}
