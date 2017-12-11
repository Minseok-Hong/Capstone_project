package com.example.imyhs.moel.Activity;

import android.content.DialogInterface;
import android.os.AsyncTask;
import android.os.Bundle;
import android.support.v7.app.AlertDialog;
import android.support.v7.app.AppCompatActivity;
import android.text.Html;
import android.view.LayoutInflater;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ListView;
import android.widget.Toast;

import com.android.volley.RequestQueue;
import com.android.volley.Response;
import com.android.volley.toolbox.Volley;
import com.example.imyhs.moel.Adapter.BuildingListAdapter;
import com.example.imyhs.moel.Building;
import com.example.imyhs.moel.R;
import com.example.imyhs.moel.Request.AdminRequest;
import com.example.imyhs.moel.Request.ElevatorRequest;
import com.example.imyhs.moel.Request.IdRequest;

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

/**
 * Created by imyhs on 2017-11-02.
 */

public class AdminActivity extends AppCompatActivity implements AdapterView.OnItemClickListener {

    private ListView buildingListView;
    //데이터 리스트
    private List<Building> buildingList;
    //리스트뷰에 사용되는 ArrayAdapter
    private BuildingListAdapter mArrayAdapter;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_admin);

        buildingListView = (ListView) findViewById(R.id.List_building);

        //ArrayList,ArrayListAdapter생성
        buildingList = new ArrayList<Building>();

        mArrayAdapter = new BuildingListAdapter(getApplicationContext(), buildingList);

        //어댑터를 리스트뷰에 세팅
        buildingListView.setAdapter(mArrayAdapter);
        //리스트뷰에 아이템클릭리스너 등록록
        buildingListView.setOnItemClickListener(this);

        findViewById(R.id.Button_admin).setOnClickListener(mClickListener);
        new BackgroundTask().execute();
    }

    class BackgroundTask extends AsyncTask<Void, Void, String>
    {
        String target;

        @Override
        protected void onPreExecute(){
            target = "http://52.79.39.200/BuildingList.php";
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
                String Building_Id, Building_Name, Building_Floor, Elevator_Number, Elevator_Capacity;
                while(count < jsonArray.length())
                {
                    JSONObject object = jsonArray.getJSONObject(count);
                    Building_Id = object.getString("Building_Id");
                    Building_Name = object.getString("Building_Name");
                    Building_Floor = object.getString("Building_Floor");
                    Elevator_Number = object.getString("Elevator_Number");
                    Elevator_Capacity = object.getString("Elevator_Capacity");
                    Building building = new Building(Building_Name,Building_Id, Building_Floor,Elevator_Number, Elevator_Capacity);
                    buildingList.add(building);
                    mArrayAdapter.notifyDataSetChanged();
                    count++;
                }
            } catch (JSONException e) {
                e.printStackTrace();
            }
        }
    }

    private long pressedTime;
    @Override
    public void onBackPressed() {
        super.onBackPressed();

        if(pressedTime == 0){
            Toast.makeText(AdminActivity.this, "한번 더 누르면 종료됩니다.", Toast.LENGTH_LONG).show();
            pressedTime = System.currentTimeMillis();
        }else{
            int seconds = (int) (System.currentTimeMillis() - pressedTime);
            if(seconds > 2000){
                pressedTime = 0;
            }else{
                finish();
            }
        }
    }

    /*
    @Override
    protected  void onResume(){
        super.onResume();
        buildingList.clear();
    }*/

    public void onItemClick(AdapterView<?> parent, View v, final int position, long id) {

        // String data = (String)parent.getItemAtPosition(position);
        Building data = buildingList.get(position);

        String message = "해당 데이터를 삭제하시겠습니까?";

        DialogInterface.OnClickListener deleteListener = new DialogInterface.OnClickListener() {
            @Override
            public void onClick(DialogInterface dialog, int which) {
                buildingList.remove(position);
                mArrayAdapter.notifyDataSetChanged();
            }
        };

        new AlertDialog.Builder(this)
                .setTitle("Delete")
                .setMessage(Html.fromHtml(message))
                .setPositiveButton("삭제", deleteListener)
                .show();
    }

    Button.OnClickListener mClickListener = new View.OnClickListener() {
        public void onClick(View v) {
            switch (v.getId()) {
                case R.id.Button_admin:
                    LayoutInflater inflater = getLayoutInflater();
                    final View dialogView = inflater.inflate(R.layout.dialog_addbuilding, null);

                    AlertDialog.Builder builder = new AlertDialog.Builder(AdminActivity.this);
                    builder.setTitle("Building Information");
                    builder.setView(dialogView);
                    builder.setPositiveButton("OK", new DialogInterface.OnClickListener() {
                        @Override
                        public void onClick(DialogInterface dialog, int which) {
                            EditText edit_buildingName = (EditText) dialogView.findViewById(R.id.dialog_buildingName);
                            EditText edit_buildingNo = (EditText) dialogView.findViewById(R.id.dialog_buildingNo);
                            EditText edit_floor = (EditText) dialogView.findViewById(R.id.dialog_floor);
                            EditText edit_elevator = (EditText) dialogView.findViewById(R.id.dialog_elevator);
                            EditText edit_capacity = (EditText) dialogView.findViewById(R.id.dialog_Capacity);

                            String Building_Name = edit_buildingName.getText().toString();
                            String Building_Id = edit_buildingNo.getText().toString();
                            String Building_Floor = edit_floor.getText().toString();
                            String Elevator_Number = edit_elevator.getText().toString();
                            String Elevator_Capacity = edit_capacity.getText().toString();

                            buildingList.add(new Building(Building_Name,Building_Id,Building_Floor,Elevator_Number,Elevator_Capacity));
                            Response.Listener<String> responseListener = new Response.Listener<String>()
                            {
                                @Override
                                public void onResponse(String response)
                                {
                                    try {

                                        JSONObject jsonResponse = new JSONObject(response);
                                        boolean success = jsonResponse.getBoolean("success");
                                    } catch (JSONException e) {
                                        e.printStackTrace();
                                    }
                                }
                            };
                            AdminRequest adminRequest = new AdminRequest(Building_Name, Building_Id, Building_Floor, Elevator_Number,Elevator_Capacity, responseListener);
                            ElevatorRequest elevatorRequest = new ElevatorRequest(Building_Id, Elevator_Number, responseListener);
                            RequestQueue requestQueue = Volley.newRequestQueue(AdminActivity.this);
                            requestQueue.add(adminRequest);
                            requestQueue.add(elevatorRequest);

                            mArrayAdapter.notifyDataSetChanged();

                            Toast.makeText(AdminActivity.this, "새로운 건물을 추가했습니다", Toast.LENGTH_SHORT).show();
                        }
                    });
                    builder.setNegativeButton("Cancel", new DialogInterface.OnClickListener() {
                        @Override
                        public void onClick(DialogInterface dialog, int which) {
                            Toast.makeText(AdminActivity.this, "추가 등록을 취소합니다", Toast.LENGTH_SHORT).show();
                        }
                    });

                    AlertDialog dialog = builder.create();
                    //dialog의 바깥쪽을 터치했을 때 Dialog가 없어지지 않도록 설정
                    dialog.setCanceledOnTouchOutside(false);
                    dialog.show();

                    break;
            }
        }
    };
}
