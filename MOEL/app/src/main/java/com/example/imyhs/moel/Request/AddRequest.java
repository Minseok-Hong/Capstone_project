package com.example.imyhs.moel.Request;

import com.android.volley.Response;
import com.android.volley.toolbox.StringRequest;

import java.util.HashMap;
import java.util.Map;

/**
 * Created by imyhs on 2017-10-09.
 */

public class AddRequest extends StringRequest{

    final static private String URL = "http://52.79.39.200/MyBuildingAdd.php";
    private Map<String, String> parameters;

    public AddRequest(String userID, String buildingID, Response.Listener<String> listener){
        super(Method.POST, URL, listener, null);
        parameters = new HashMap<>();
        parameters.put("userID", userID);
        parameters.put("Building_Id", buildingID);
    }

    @Override
    public Map<String, String> getParams(){
        return parameters;
    }
}
