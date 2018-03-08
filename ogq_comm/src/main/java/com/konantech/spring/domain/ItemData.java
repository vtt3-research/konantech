package com.konantech.spring.domain;

import lombok.Data;

import java.io.Serializable;
import java.util.Date;
import java.util.List;

@Data
public class ItemData implements Serializable {

    int idx;
    String requestId;
    String videoUrl;
    String callbackUrl;
    String resultString;
    Date createDate;
    Date updateDate;
    int status;
    int videoId;
    int isUsed;



}