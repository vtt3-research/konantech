package com.konantech.spring.domain;

import lombok.Data;

import java.io.Serializable;

@Data
public class DarcCond implements Serializable {
    private String key;
    private String cond;
    private Object value;

    public DarcCond() { // restapi required
    }

    public DarcCond(String key, String cond, String value) {
        this.key = key;
        this.cond = cond;
        this.value = value;
    }
}
