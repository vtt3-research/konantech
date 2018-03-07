package com.konantech.spring.domain;

import io.swagger.annotations.ApiModel;
import io.swagger.annotations.ApiModelProperty;
import lombok.Data;

import java.io.Serializable;
import java.util.List;

@Data
@ApiModel
public class AssetsGetRequest implements Serializable {

    @ApiModelProperty(position = 1, required = true, value = "0")
    private int offset;
    @ApiModelProperty(position = 2, required = true, value = "10")
    private int limit;

    private List<DarcCond> where;

    public AssetsGetRequest() {
        this.offset = 0;
        this.limit = 10;
    }
    private List<DarcOrderBy> orderBy;


    public static AssetsGetRequest test() {
       return new AssetsGetRequest();
    }
}