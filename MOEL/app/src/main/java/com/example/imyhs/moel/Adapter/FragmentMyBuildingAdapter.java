package com.example.imyhs.moel.Adapter;

import android.content.Context;
import android.content.Intent;
import android.support.v4.app.Fragment;
import android.support.v7.app.AlertDialog;
import android.util.Log;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.Button;
import android.widget.TextView;

import com.android.volley.RequestQueue;
import com.android.volley.Response;
import com.android.volley.toolbox.Volley;
import com.example.imyhs.moel.Activity.ElevatorCallActivity;
import com.example.imyhs.moel.Activity.MainActivity;
import com.example.imyhs.moel.Building;
import com.example.imyhs.moel.Notice;
import com.example.imyhs.moel.R;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.util.List;

/**
 * Created by imyhs on 2017-11-19.
 */

public class FragmentMyBuildingAdapter extends BaseAdapter {

    private Context context;
    private List<Building> FragmentBuildingList;
    private Fragment parent;
    private String userID = MainActivity.userID;

    public FragmentMyBuildingAdapter(Context context, List<Building> FragmentBuildingList, Fragment parent) {
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
        View v = View.inflate(context, R.layout.fragment_mybuilding, null);
        TextView nameText = (TextView) v.findViewById(R.id.addbuildingNameText);
        TextView idText = (TextView) v.findViewById(R.id.addBuildingIDText);
        TextView floorText = (TextView) v.findViewById(R.id.addBuildingFloorText);
        TextView numElevatorText = (TextView) v.findViewById(R.id.addElevatorNumText);

        nameText.setText("건물 이름 : " + FragmentBuildingList.get(i).getName());
        idText.setText("건물 ID : " + FragmentBuildingList.get(i).getid());
        floorText.setText(FragmentBuildingList.get(i).getFloor()+"층");
        numElevatorText.setText(FragmentBuildingList.get(i).getNum()+"대");

        v.setTag(FragmentBuildingList.get(i).getid());

        Button selectButton = (Button) v.findViewById(R.id.selectBuildingButton);

        selectButton.setOnClickListener(new View.OnClickListener() {


            @Override
            public void onClick(View v) {
                String Building_Floor, Elevator_Number, Building_Id;
                Building_Floor = FragmentBuildingList.get(i).getFloor();
                Elevator_Number = FragmentBuildingList.get(i).getNum();
                Building_Id = FragmentBuildingList.get(i).getid();

                System.out.println(Building_Floor + Elevator_Number);
                Intent intent = new Intent(parent.getActivity(), ElevatorCallActivity.class);
                intent.putExtra("Building_Floor", Building_Floor);
                intent.putExtra("Elevator_Number", Elevator_Number);
                intent.putExtra("Building_Id", Building_Id);

                parent.getActivity().startActivity(intent);
            }
        });
        return v;
    }
}
