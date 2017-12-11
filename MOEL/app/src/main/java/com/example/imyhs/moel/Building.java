package com.example.imyhs.moel;

/**
 * Created by imyhs on 2017-11-16.
 */

public class Building {

    String name;
    String id;
    String floor;
    String num;
    String capacity;

    public Building(String name, String id, String floor, String num, String capacity) {
        this.name = name;
        this.id = id;
        this.floor = floor;
        this.num = num;
        this.capacity = capacity;
    }

    public String getCapacity() {
        return capacity;
    }

    public void setCapacity(String capacity) {
        this.capacity = capacity;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public String getid() {
        return id;
    }

    public void setid(String id) {
        this.id = id;
    }

    public String getFloor() {
        return floor;
    }

    public void setFloor(String floor) {
        this.floor = floor;
    }

    public String getNum() {
        return num;
    }

    public void setNum(String num) {
        this.num = num;
    }


}
