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
    Date createDate;
    Date updateDate;
    String resultString;
    int status;
    int videoId;
    int isUsed;



}