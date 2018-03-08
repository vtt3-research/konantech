package com.konantech.spring.domain;

import lombok.Data;

import java.io.Serializable;
import java.util.List;

@Data
public class AssetsResponse implements Serializable{
    private List<String> tags;

    public AssetsResponse(){
    }

    public AssetsResponse(List<String> tags, List<ShotInfo> shots) {
        this.tags = tags;
        this.shots = shots;
    }

    private List<ShotInfo> shots;
}
