package com.konantech.spring.service;

import com.konantech.spring.mapper.MetaMapper;
import org.apache.commons.collections.MapUtils;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Propagation;
import org.springframework.transaction.annotation.Transactional;

import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map;

@Service
@Transactional(propagation = Propagation.REQUIRED, rollbackFor = Exception.class)
public class MetaService {

    private static Logger log = LoggerFactory.getLogger(MetaService.class);

    @Autowired
    private MetaMapper metaMapper;

    public Map<String,Object> putMeta(Map<String, Object> request) {
        metaMapper.putMeta(request);
        return metaMapper.getMetaIdx(MapUtils.getIntValue(request,"IDX"));
    }

    public Map<String,Object> getMetaIdx(int idx) {
        return metaMapper.getMetaIdx(idx);
    }

    public List<Map<String,Object>> getMetaList(String jsonString) {
        return metaMapper.getMetaList(jsonString);
    }

}