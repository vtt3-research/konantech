package com.konantech.spring.util;

import org.apache.commons.collections.map.HashedMap;

import java.util.Map;

public class ResultJSON {
    private int code = 200;
    private String status = "SUCCESS";
    private Object data;

    public ResultJSON error(String message) {
        Map<String, Object> resultMap = new HashedMap();
        resultMap.put("message", message);
        this.setCode(100);
        this.setStatus("ERROR");
        this.setData(resultMap);
        return this;
    }

    public int getCode() {
        return code;
    }

    public void setCode(int code) {
        this.code = code;
    }

    public String getStatus() {
        return status;
    }

    public void setStatus(String status) {
        this.status = status;
    }

    public Object getData() {
        return data;
    }

    public void setData(Object data) {
        this.data = data;
    }

}