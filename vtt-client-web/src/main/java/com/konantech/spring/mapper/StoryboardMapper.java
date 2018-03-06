package com.konantech.spring.mapper;

import com.konantech.spring.domain.content.ContentQuery;
import com.konantech.spring.domain.storyboard.ShotTB;
import org.apache.ibatis.annotations.Mapper;

import java.util.List;
import java.util.Map;


@Mapper
public interface StoryboardMapper {

    int getShotCount(ContentQuery param);
    List<ShotTB> getShotList(ContentQuery param);
    Map<String,Object> getShotItem(ContentQuery param);
    int putShotItem(ShotTB shotTB);
    int deleteShotItems(int videoid);

}
