package com.konantech.spring.domain;

import lombok.Data;

@Data
public class DarcStatus {

    private int transcodingstatus;
    private int catalogstatus;
    private int transcodingPercent;
    private int catalogPercent;

    private String transcodingMsg;
    private String catalogMsg;
}
