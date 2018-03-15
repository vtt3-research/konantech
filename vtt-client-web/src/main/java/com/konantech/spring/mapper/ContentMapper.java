package com.konantech.spring.mapper;

import com.konantech.spring.domain.content.ContentField;
import com.konantech.spring.domain.content.ContentQuery;
import org.apache.ibatis.annotations.Mapper;

import java.util.List;
import java.util.Map;


@Mapper
public interface ContentMapper {

    int getContentCount(ContentQuery param);
    List<Map<String,Object>> getContentList(ContentQuery param);
    ContentField getContentItem(ContentQuery param);
    int putContentItem(Map<String, Object> request);
    int updateContentItem(Map<String, Object> request);
    int deleteContentItem(int idx);

}
