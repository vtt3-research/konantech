package com.konantech.spring.domain.content;

import lombok.Data;

@Data
public class ContentQuery {

    private int idx;

    private int limit;
    private int offset;
}
