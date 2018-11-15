package com.konantech.spring.mapper;

import com.konantech.spring.domain.vtt.RepImgVo;
import org.apache.ibatis.annotations.Mapper;
import org.springframework.stereotype.Repository;

import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map;

@Mapper
@Repository
public interface VisualMapper {

    int getPutRepreImg(Map param) throws Exception;

    int getCntRepreImg(Map map) throws Exception;

    List<RepImgVo> getSelectRepImg(Map map) throws Exception;

    int getPutMetaInfo(Map map) throws Exception;

    LinkedHashMap<String, Object> getSelectMetaInfo(Map<String,Object> map) throws Exception;

    Map getRepImgInfo(Map map) throws Exception;

    int getPutSecInfo(Map<String,Object> map) throws Exception;

    List<LinkedHashMap<String,Object>> getSecInfo(Map<String,Object> map) throws Exception;

    List<String> getJsonData(Map map) throws Exception;

    int deleteRepImg(Map map) throws Exception;

    List<Map> getWorkerList(Map map) throws Exception;
}
