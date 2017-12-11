package com.example.imyhs.moel.Adapter;

import android.content.Context;
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

import org.json.JSONException;
import org.json.JSONObject;

import java.util.List;

/**
 * Created by imyhs on 2017-11-16.
 */

public class BuildingListAdapter extends BaseAdapter {

    private Context context;
    private List<Building> buildingList;

    public BuildingListAdapter(Context context, List<Building> buildingList) {
        this.context = context;
        this.buildingList = buildingList;
    }

    @Override
    public int getCount() {
        return buildingList.size();
    }

    @Override
    public Object getItem(int i) {
        return buildingList.get(i);
    }

    @Override
    public long getItemId(int i) {
        return i;
    }

    @Override
    public View getView(int i, View view, ViewGroup viewGroup) {
        View v = View.inflate(context, R.layout.building, null);
        TextView nameText = (TextView) v.findViewById(R.id.BuildingName);
        TextView idText = (TextView) v.findViewById(R.id.BuildingID);
        TextView floorText = (TextView) v.findViewById(R.id.floor);
        TextView numElevatorText = (TextView) v.findViewById(R.id.NumElevator);
        TextView ElevatorCapacityText = (TextView) v.findViewById(R.id.ElevatorCapacity);

        nameText.setText("건물 이름 : " + buildingList.get(i).getName());
        idText.setText("건물 ID : " + buildingList.get(i).getid());
        floorText.setText(buildingList.get(i).getFloor() +"층");
        numElevatorText.setText(buildingList.get(i).getNum() +"대");
        ElevatorCapacityText.setText("최대 수용 인원: " + buildingList.get(i).getCapacity() +"명");


        v.setTag(buildingList.get(i).getName());
        return v;
    }
}
