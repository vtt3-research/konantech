package com.konantech.spring.service;

import com.konantech.spring.domain.content.ContentQuery;
import com.konantech.spring.domain.storyboard.ShotTB;
import com.konantech.spring.mapper.StoryboardMapper;
import com.konantech.spring.util.FFmpegUtil;
import org.apache.commons.collections.MapUtils;
import org.apache.commons.io.FilenameUtils;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Propagation;
import org.springframework.transaction.annotation.Transactional;

import java.util.List;
import java.util.Map;

@Service
@Transactional(propagation = Propagation.REQUIRED, rollbackFor = Exception.class)
public class StoryboardService {

    @Autowired
    private StoryboardMapper storyboardMapper;

    @Value("${darc.proxyShotFolder}")
    public String proxyShotFolder;

    @Autowired
    private FFmpegUtil fFmpegUtil;

    public int getShotCount(ContentQuery param) {
        return storyboardMapper.getShotCount(param);
    }

    public List<ShotTB> getShotList(ContentQuery n) {
        return storyboardMapper.getShotList(n);
    }

    public Map<String,Object> getShotItem(ContentQuery n) {
        return storyboardMapper.getShotItem(n);
    }

    public int putShotItem(ShotTB shotTB) {
        return storyboardMapper.putShotItem(shotTB);
    }
    public int deleteShotItems(int videoid) {
        return storyboardMapper.deleteShotItems(videoid);
    }

    public Map<String,Object> getShotSize(ShotTB asset) throws Exception {
        String assetfilepath = asset.getAssetfilepath();
        String assetfilename = asset.getAssetfilename();
        String sourceFile = FilenameUtils.normalize(proxyShotFolder + "/" + assetfilepath + "/" + assetfilename);
        String mediainfo = fFmpegUtil.getMediaInfo(sourceFile);
        return fFmpegUtil.shotSize(mediainfo);
    }
}