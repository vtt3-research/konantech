package com.konantech.spring.mapper;

import org.apache.ibatis.annotations.Mapper;

import java.util.List;
import java.util.Map;


@Mapper
public interface CustomQueryMapper {

    Map<String, Object> selectItem(String query);

    List<Map<String, Object>> selectList(String query);

}
