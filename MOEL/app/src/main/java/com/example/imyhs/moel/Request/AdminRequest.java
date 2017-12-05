package com.example.imyhs.moel.Request;

import com.android.volley.Response;
import com.android.volley.toolbox.StringRequest;

import java.util.HashMap;
import java.util.Map;

/**
 * Created by imyhs on 2017-11-16.
 */

public class AdminRequest extends StringRequest{
    final static private String URL = "http://52.79.39.200/buildingRegister.php";
    private Map<String, String> parameters;

    public AdminRequest(String buildingName, String floor, String buildingID, String numElevator, Response.Listener<String> listener)
    {
        super(Method.POST, URL, listener, null );
        parameters = new HashMap<>();
        parameters.put("Building_Name", buildingName);
        parameters.put("Building_Id",buildingID);
        parameters.put("Building_Floor", floor);
        parameters.put("Elevator_Number", numElevator);
    }

    @Override
    public Map<String, String> getParams(){
        return parameters;
    }
}
