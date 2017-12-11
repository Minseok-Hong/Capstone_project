package com.example.imyhs.moel.Request;

import com.android.volley.Response;
import com.android.volley.toolbox.StringRequest;

import java.util.HashMap;
import java.util.Map;

/**
 * Created by imyhs on 2017-12-07.
 */

public class ElevatorRequest extends StringRequest {
    final static private String URL = "http://52.79.39.200/ElevatorNumber.php";
    private Map<String, String> parameters;

    public ElevatorRequest(String buildingID, String numElevator, Response.Listener<String> listener)
    {
        super(Method.POST, URL, listener, null );
        parameters = new HashMap<>();
        parameters.put("Building_Id",buildingID);
        parameters.put("Elevator_Number", numElevator);
    }

    @Override
    public Map<String, String> getParams(){
        return parameters;
    }
}
