package com.example.imyhs.moel.Request;

import com.android.volley.Response;
import com.android.volley.toolbox.StringRequest;

import java.util.HashMap;
import java.util.Map;

/**
 * Created by imyhs on 2017-11-28.
 */

public class IdRequest extends StringRequest {

    final static private String URL = "http://52.79.39.200/IntoId.php";
    private Map<String, String> parameters;

    public IdRequest(String userID, Response.Listener<String> listener){

        super(Method.POST, URL, listener, null );
        parameters = new HashMap<>();
        parameters.put("userID", userID);
    }


    @Override
    public Map<String, String> getParams(){
        return parameters;
    }
}
