package com.example.imyhs.moel.Fragment;

import android.net.Uri;
import android.os.AsyncTask;
import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.support.v7.app.AlertDialog;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ListView;
import android.widget.TextView;

import com.example.imyhs.moel.Activity.MainActivity;
import com.example.imyhs.moel.Adapter.FragmentMyBuildingAdapter;
import com.example.imyhs.moel.Building;
import com.example.imyhs.moel.R;

import org.json.JSONArray;
import org.json.JSONObject;

import java.io.BufferedReader;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.net.HttpURLConnection;
import java.net.URL;
import java.net.URLEncoder;
import java.util.ArrayList;
import java.util.List;


/**
 * A simple {@link Fragment} subclass.
 * Activities that contain this fragment must implement the
 * {@link MyBuildingFragment.OnFragmentInteractionListener} interface
 * to handle interaction events.
 * Use the {@link MyBuildingFragment#newInstance} factory method to
 * create a
 * n instance of this fragment.
 */
public class MyBuildingFragment extends Fragment {
    // TODO: Rename parameter arguments, choose names that match
    // the fragment initialization parameters, e.g. ARG_ITEM_NUMBER
    private static final String ARG_PARAM1 = "param1";
    private static final String ARG_PARAM2 = "param2";

    // TODO: Rename and change types of parameters
    private String mParam1;
    private String mParam2;

    private OnFragmentInteractionListener mListener;

    public MyBuildingFragment() {
        // Required empty public constructor
    }

    /**
     * Use this factory method to create a new instance of
     * this fragment using the provided parameters.
     *
     * @param param1 Parameter 1.
     * @param param2 Parameter 2.
     * @return A new instance of fragment MyBuildingFragment.
     */
    // TODO: Rename and change types and number of parameters
    public static MyBuildingFragment newInstance(String param1, String param2) {
        MyBuildingFragment fragment = new MyBuildingFragment();
        Bundle args = new Bundle();
        args.putString(ARG_PARAM1, param1);
        args.putString(ARG_PARAM2, param2);
        fragment.setArguments(args);
        return fragment;
    }

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        if (getArguments() != null) {
            mParam1 = getArguments().getString(ARG_PARAM1);
            mParam2 = getArguments().getString(ARG_PARAM2);
        }
    }

    private ListView MyBuildingListView;
    private FragmentMyBuildingAdapter adapter;
    private List<Building> MyBuildingList;

    @Override
    public void onActivityCreated(Bundle b){
        super.onActivityCreated(b);
        MyBuildingListView = (ListView) getView().findViewById(R.id.MyBuildingListView);
        MyBuildingList = new ArrayList<Building>();
        adapter = new FragmentMyBuildingAdapter(getContext().getApplicationContext(), MyBuildingList, this);
        MyBuildingListView.setAdapter(adapter);
        new BackgroundTask().execute();
    }

    class BackgroundTask extends AsyncTask<Void, Void, String>
    {
        String target;

        @Override
        protected void onPreExecute(){
            try {
                System.out.println(MainActivity.userID);
                target = "http://52.79.39.200/ShowMyBuildingList.php?userID=" + URLEncoder.encode(MainActivity.userID, "UTF-8");
            } catch (Exception e) {
                e.printStackTrace();
            }
        }

        @Override
        protected String doInBackground(Void... voids) {
            try{
                URL url = new URL(target);
                HttpURLConnection httpURLConnection = (HttpURLConnection) url.openConnection();
                InputStream inputStream = httpURLConnection.getInputStream();

                BufferedReader bufferedReader = new BufferedReader(new InputStreamReader(inputStream));
                String temp;
                StringBuilder stringBuilder = new StringBuilder();
                while((temp = bufferedReader.readLine()) != null)
                {
                    stringBuilder.append(temp + "\n");
                }
                bufferedReader.close();
                inputStream.close();
                httpURLConnection.disconnect();
                return stringBuilder.toString().trim();

            }catch(Exception e){
                e.printStackTrace();
            }
            return null;
        }

        @Override
        public void onProgressUpdate(Void... values){
            super.onProgressUpdate();
        }

        @Override
        public void onPostExecute(String result){
            try{
                JSONObject jsonObject = new JSONObject(result);
                //response라는 배열의 이름으로 받아준다
                JSONArray jsonArray = jsonObject.getJSONArray("response");
                int count = 0;

                String name;
                String id;
                String floor;
                String num;

                while (count<jsonArray.length())
                {
                    JSONObject object = jsonArray.getJSONObject(count);
                    name = object.getString("Building_Name");
                    System.out.println(name);
                    id = object.getString("Building_Id");
                    floor = object.getString("Building_Floor");
                    num = object.getString("Elevator_Number");
                    Building building = new Building(name, id, floor, num);
                    MyBuildingList.add(building);
                    count++;
                }
                adapter.notifyDataSetChanged();
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        // Inflate the layout for this fragment
        return inflater.inflate(R.layout.fragment_my_building, container, false);
    }

    // TODO: Rename method, update argument and hook method into UI event
    public void onButtonPressed(Uri uri) {
        if (mListener != null) {
            mListener.onFragmentInteraction(uri);
        }
    }

    @Override
    public void onDetach() {
        super.onDetach();
        mListener = null;
    }

    /**
     * This interface must be implemented by activities that contain this
     * fragment to allow an interaction in this fragment to be communicated
     * to the activity and potentially other fragments contained in that
     * activity.
     * <p>
     * See the Android Training lesson <a href=
     * "http://developer.android.com/training/basics/fragments/communicating.html"
     * >Communicating with Other Fragments</a> for more information.
     */
    public interface OnFragmentInteractionListener {
        // TODO: Update argument type and name
        void onFragmentInteraction(Uri uri);
    }
}
