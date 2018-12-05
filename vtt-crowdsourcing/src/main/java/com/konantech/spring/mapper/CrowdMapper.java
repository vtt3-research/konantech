package com.konantech.spring.mapper;

import org.apache.ibatis.annotations.Mapper;
import org.springframework.stereotype.Repository;

import java.util.List;
import java.util.Map;

@Mapper
@Repository
public interface CrowdMapper {

    int insertObjectTargetData() throws Exception;

    int insertBackgroundTargetData() throws Exception;

    int updateMetaData() throws Exception;

    Map getInObjectCode(Map map) throws Exception;

    Map getOutObjectCode(Map param) throws Exception;

    List<Map> getTargetData(Map param) throws Exception;

    List<Map> getInDragObjectCode(Map map) throws Exception;

    Map getOutDragObjectCode(Map param) throws Exception;

    List<Map> getDragTarget(Map map) throws Exception;

    int setTargetData(Map parom) throws Exception;

    int putUserTrust(Map parom) throws Exception;

    Map getMetaData(Map param) throws Exception;

    int checkLogTb(Map param) throws Exception;

    int createLogTb(Map parom) throws Exception;

    int putCrowdLog(Map parom) throws Exception;

    int putMeataData(Map parom) throws Exception;
}
