package com.konantech.spring.domain;

import lombok.Data;

import java.io.Serializable;
import java.util.List;
import java.util.Map;

/**
 * Created by gsm on 2017-06-15.
 */

@Data
public class ShotInfo implements Serializable{

    private int startframeindex;
    private String starttimecode;
    private int endframeindex;
    private String endtimecode;
    private String image;
    private List<String> tags;
    private List<String> location;
    private Object object;

    public ShotInfo(){

    }

}
