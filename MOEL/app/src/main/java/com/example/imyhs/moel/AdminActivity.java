package com.example.imyhs.moel;

import android.content.DialogInterface;
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

import java.util.ArrayList;

/**
 * Created by imyhs on 2017-11-02.
 */

public class AdminActivity extends AppCompatActivity implements AdapterView.OnItemClickListener {

    //Test
    private ListView mListView;
    //데이터 리스트
    private ArrayList<String> mArrayList;
    //리스트뷰에 사용되는 ArrayAdapter
    private ArrayAdapter<String> mArrayAdapter;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_admin);

        mListView = (ListView) findViewById(R.id.List_building);

        //ArrayList,ArrayListAdapter생성
        mArrayList = new ArrayList<String>();
        mArrayAdapter = new ArrayAdapter<String>(this, android.R.layout.simple_list_item_1, mArrayList);

        //어댑터를 리스트뷰에 세팅
        mListView.setAdapter(mArrayAdapter);
        //리스트뷰에 아이템클릭리스너 등록록
        mListView.setOnItemClickListener(this);

        findViewById(R.id.Button_admin).setOnClickListener(mClickListener);
    }

    /*
    @Override
    protected  void onResume(){
        super.onResume();
        mArrayList.clear();
    }*/

    public void onItemClick(AdapterView<?> parent, View v, final int position, long id) {

        // String data = (String)parent.getItemAtPosition(position);
        String data = mArrayList.get(position);

        String message = "해당 데이터를 삭제하시겠습니까?";

        DialogInterface.OnClickListener deleteListener = new DialogInterface.OnClickListener() {
            @Override
            public void onClick(DialogInterface dialog, int which) {
                mArrayList.remove(position);
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

                            String buildingName = edit_buildingName.getText().toString();
                            String buildingNo = edit_buildingNo.getText().toString();
                            String floor = edit_floor.getText().toString();
                            String numElevator = edit_elevator.getText().toString();

                            mArrayList.add(buildingName);
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
