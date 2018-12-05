package com.konantech.spring.domain;

import lombok.Data;

import java.util.List;

/**
 * Created by gsm on 2017-06-15.
 */

@Data
public class AssetsResponse {
    private List<String> tags;

    public AssetsResponse(){
    }

    public AssetsResponse(List<String> tags, List<ShotInfo> shots) {
        this.tags = tags;
        this.shots = shots;
    }

    private List<ShotInfo> shots;
}
