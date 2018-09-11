package com.konantech.spring.service;

import com.konantech.spring.domain.vtt.RepImgVo;
import org.springframework.stereotype.Service;

import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map;

@Service
public interface VisualService {
    int getPutRepreImg(Map param) throws Exception;

    int getCntRepreImg(Map map) throws Exception;

    List<RepImgVo> getSelectRepImg(Map map) throws Exception;

    int getPutMetaInfo(Map map) throws Exception;

    LinkedHashMap<String, Object> getSelectMetaInfo(Map<String,Object> map) throws Exception;

    Map getRepImgInfo(Map repMap) throws Exception;

    int getPutSecInfo(Map<String,Object> pramMap) throws Exception;

    List<LinkedHashMap<String,Object>> getSecInfo(Map<String,Object> pramMap) throws Exception;;
}
