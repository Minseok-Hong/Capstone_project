package com.example.imyhs.moel.Activity;

import android.content.DialogInterface;
import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.support.v7.app.AlertDialog;
import android.support.v7.app.AppCompatActivity;
import android.view.LayoutInflater;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.NumberPicker;
import android.widget.TextView;
import android.widget.Toast;

import com.android.volley.RequestQueue;
import com.android.volley.Response;
import com.android.volley.toolbox.Volley;
import com.example.imyhs.moel.Building;
import com.example.imyhs.moel.R;
import com.example.imyhs.moel.Request.AdminRequest;
import com.example.imyhs.moel.Request.FloorRequest;

import org.json.JSONException;
import org.json.JSONObject;

/**
 * Created by imyhs on 2017-11-23.
 */

public class ElevatorCallActivity extends AppCompatActivity{

    NumberPicker numberPicker_Current;
    NumberPicker numberPicker_Go;
    private AlertDialog dialog;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_elevatorcall);
        Intent i = getIntent();
        String Building_Floor = i.getStringExtra("Building_Floor");
        String Elevator_Number = i.getStringExtra("Elevator_Number");
        final String Building_Id = i.getStringExtra("Building_Id");

        int floor = Integer.parseInt(Building_Floor);

        numberPicker_Current = (NumberPicker) findViewById(R.id.floor_current);
        numberPicker_Go = (NumberPicker) findViewById(R.id.floor_Go);

        numberPicker_Current.setMaxValue(floor);
        numberPicker_Current.setMinValue(1);

        numberPicker_Go.setMinValue(1);
        numberPicker_Go.setMaxValue(floor);


        Button button = (Button) findViewById(R.id.Button_activity_call);
        button.setOnClickListener(new View.OnClickListener(){

            public void onClick(View v) {

                String Current_Floor;
                String Go_Floor;
                String userID = MainActivity.userID;
                String Flag = "0";

                Current_Floor = String.valueOf(numberPicker_Current.getValue());
                Go_Floor = String.valueOf(numberPicker_Go.getValue());

                Response.Listener<String> responseListener = new Response.Listener<String>() {
                    @Override
                    public void onResponse(String response) {
                        try {
                            JSONObject jsonResponse = new JSONObject(response);
                            boolean success = jsonResponse.getBoolean("success");
                            if (success) {
                                AlertDialog.Builder builder = new AlertDialog.Builder(ElevatorCallActivity.this);
                                dialog = builder.setMessage("호출 성공! 잠시만 기다려 주세요..")
                                        .setPositiveButton("확인", null)
                                        .create();
                                dialog.show();

                                Handler handler = new Handler();
                                handler.postDelayed(new Runnable() {
                                    @Override
                                    public void run() {
                                        Intent intent = new Intent(getApplication(), ShowActivity.class);
                                        ElevatorCallActivity.this.startActivity(intent);
                                        finish();
                                    }
                                },5000);

                            } else {
                                AlertDialog.Builder builder = new AlertDialog.Builder(ElevatorCallActivity.this);
                                dialog = builder.setMessage("엘리베이터 호출에 실패했습니다.")
                                        .setNegativeButton("확인", null)
                                        .create();
                                dialog.show();
                            }
                        } catch (JSONException e) {
                            e.printStackTrace();
                        }
                    }
                };
                FloorRequest floorRequest = new FloorRequest(userID, Current_Floor, Go_Floor, Building_Id, Flag,responseListener);
                RequestQueue requestQueue = Volley.newRequestQueue(ElevatorCallActivity.this);
                requestQueue.add(floorRequest);

            }
        });
    }

    @Override
    protected void onStop(){
        super.onStop();
        if(dialog != null)
        {
            dialog.dismiss();
            dialog = null;
        }
    }
}
