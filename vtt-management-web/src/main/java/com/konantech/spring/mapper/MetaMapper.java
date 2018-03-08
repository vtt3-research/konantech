package com.konantech.spring.mapper;

import org.apache.ibatis.annotations.Mapper;

import java.util.List;
import java.util.Map;


@Mapper
public interface MetaMapper {

    int putMeta(Map<String, Object> request);
    Map<String, Object> getMetaIdx(int idx);

    List<Map<String,Object>> getMetaList(String jsonString);

}
