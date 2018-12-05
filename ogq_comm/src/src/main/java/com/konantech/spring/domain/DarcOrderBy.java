package com.konantech.spring.domain;

import lombok.Data;

import java.io.Serializable;

@Data
public class DarcOrderBy implements Serializable {
    private String key;
    private String sort;

    public DarcOrderBy(String key, String sort) {
        this.key = key;
        this.sort = sort;
    }

    public DarcOrderBy() { // restapi required
    }
}
