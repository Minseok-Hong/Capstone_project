package com.example.imyhs.moel.Request;

import com.android.volley.Request;
import com.android.volley.Response;
import com.android.volley.toolbox.StringRequest;

import java.util.HashMap;
import java.util.Map;

/**
 * Created by imyhs on 2017-11-27.
 */

public class FloorRequest extends StringRequest {

    final static private String URL = "http://52.79.39.200/FloorInfo.php";
    private Map<String, String> parameters;

    public FloorRequest(String userID, String Current_Floor, String Go_Floor, String Building_Id, String Flag, Response.Listener<String> listener){
        super(Method.POST, URL, listener, null);
        parameters = new HashMap<>();
        parameters.put("userID", userID);
        parameters.put("Current_Floor", Current_Floor);
        parameters.put("Go_Floor", Go_Floor);
        parameters.put("Building_Id", Building_Id);
        parameters.put("Flag", Flag);
    }

    @Override
    public Map<String, String> getParams(){
        return parameters;
    }
}
