package com.example.imyhs.moel.Adapter;

import android.content.Context;
import android.support.v4.app.Fragment;
import android.support.v7.app.AlertDialog;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.Button;
import android.widget.TextView;

import com.android.volley.RequestQueue;
import com.android.volley.Response;
import com.android.volley.toolbox.Volley;
import com.example.imyhs.moel.Activity.MainActivity;
import com.example.imyhs.moel.Activity.RegisterActivity;
import com.example.imyhs.moel.Building;
import com.example.imyhs.moel.R;
import com.example.imyhs.moel.Request.AddRequest;

import org.json.JSONException;
import org.json.JSONObject;

import java.util.List;

/**
 * Created by imyhs on 2017-11-19.
 */

public class FragmentBuildingAdapter extends BaseAdapter {

    private Context context;
    private List<Building> FragmentBuildingList;
    private Fragment parent;

    public FragmentBuildingAdapter(Context context, List<Building> FragmentBuildingList, Fragment parent) {
        this.context = context;
        this.FragmentBuildingList = FragmentBuildingList;
        this.parent = parent;
    }

    @Override
    public int getCount() {
        return FragmentBuildingList.size();
    }

    @Override
    public Object getItem(int i) {
        return FragmentBuildingList.get(i);
    }

    @Override
    public long getItemId(int i) {
        return i;
    }

    @Override
    public View getView(final int i, View view, ViewGroup viewGroup)
    {
        View v = View.inflate(context, R.layout.fragment_building, null);
        TextView nameText = (TextView) v.findViewById(R.id.Fragment_BuildingName);
        TextView idText = (TextView) v.findViewById(R.id.Fragment_BuildingID);
        TextView floorText = (TextView) v.findViewById(R.id.Fragment_floor);
        TextView numElevatorText = (TextView) v.findViewById(R.id.Fragment_NumElevator);

        nameText.setText("건물 이름 : " + FragmentBuildingList.get(i).getName());
        idText.setText("건물 ID : " + FragmentBuildingList.get(i).getid());
        floorText.setText(FragmentBuildingList.get(i).getFloor()+"층");
        numElevatorText.setText(FragmentBuildingList.get(i).getNum()+"대");

        v.setTag(FragmentBuildingList.get(i).getid());

        Button addButton = (Button) v.findViewById(R.id.addBuildingButton);
        addButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                String userID = MainActivity.userID;
                System.out.println(userID);
                Response.Listener<String> responseListener = new Response.Listener<String>(){

                    @Override
                    public void onResponse(String response) {
                        try
                        {
                            JSONObject jsonResponse = new JSONObject(response);
                            boolean success = jsonResponse.getBoolean("success");

                            if(success){
                                AlertDialog.Builder builder = new AlertDialog.Builder(parent.getActivity());
                                AlertDialog dialog= builder.setMessage("빌딩이 추가되었습니다.")
                                        .setPositiveButton("확인",null)
                                        .create();
                                dialog.show();
                            }
                            else{
                                AlertDialog.Builder builder = new AlertDialog.Builder(parent.getActivity());
                                AlertDialog dialog = builder.setMessage("빌딩 추가에 실패하였습니다.")
                                        .setNegativeButton("확인",null)
                                        .create();
                                dialog.show();
                            }
                        }
                        catch (JSONException e)
                        {
                            e.printStackTrace();
                        }
                    }
                };
                AddRequest addRequest = new AddRequest(userID, FragmentBuildingList.get(i).getid() + "", responseListener);
                RequestQueue queue = Volley.newRequestQueue(parent.getActivity());
                queue.add(addRequest);
            }
        });
        return v;
    }
}
