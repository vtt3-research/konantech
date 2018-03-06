package com.konantech.spring.service;

import com.konantech.spring.core.RedisRepository;
import com.konantech.spring.domain.content.ContentField;
import com.konantech.spring.domain.content.ContentQuery;
import com.konantech.spring.domain.mamex.MamExCnfTb;
import com.konantech.spring.domain.storyboard.ShotTB;
import com.konantech.spring.mapper.MamExMapper;
import com.konantech.spring.util.JSONUtils;
import com.konantech.spring.util.RequestUtils;
import com.konantech.spring.util.RestApiUtil;
import org.apache.commons.collections.MapUtils;
import org.apache.commons.io.FilenameUtils;
import org.apache.commons.lang3.StringUtils;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Propagation;
import org.springframework.transaction.annotation.Transactional;

import java.io.File;
import java.text.Normalizer;
import java.text.SimpleDateFormat;
import java.util.*;

@Service
@Transactional(propagation = Propagation.REQUIRED, rollbackFor = Exception.class)
public class MamExService {

    @Value("${darc.videoFolder}")
    public String videoFolder;

    @Value("${darc.proxyShotFolder}")
    public String proxyShotFolder;

    @Autowired
    RedisRepository redisRepository;

    @Autowired
    private ContentService contentService;


    @Autowired
    private StoryboardService storyboardService;

    @Autowired
    private MamExMapper mamExMapper;

    // 임시!
    public String ex_update_jobresult(String trName, int pool, String request) throws Exception {
        try {
            Map<String, Object> param = JSONUtils.jsonStringToMap(request);
            Map assetinfo = (HashMap) param.get("assetinfo");
            String type = (String) assetinfo.get("type");
            if(StringUtils.equals(type,"shot")) {
                Map workinfo = (HashMap) param.get("workinfo");
                Map shotinfo = (HashMap) assetinfo.get("shotinfo");
                shotinfo.put("object",shotinfo.get("object"));
                return shot(shotinfo);
            } else if(StringUtils.equals(type,"qualitycheck")) {
                System.out.println(param);
            }

        } catch (Exception e) {
            throw new Exception(e.getMessage(), e);
        }
        return "SUCCESS";
    }


    public String shot(Map shotinfo) throws Exception {

        String filepath = MapUtils.getString(shotinfo, "filepath");
        filepath = Normalizer.normalize(filepath, Normalizer.Form.NFC);

        int idx = MapUtils.getIntValue(shotinfo,"assetid");
        ContentQuery query = new ContentQuery();
        query.setIdx(idx);
        ContentField asset = contentService.getContentItem(query);

        // reset
        if(StringUtils.equals(MapUtils.getString(shotinfo,"sequencetype"),"start")) {
            storyboardService.deleteShotItems(idx);
        }

        // shot insert
        if (asset != null) {
            ShotTB shotTB = new ShotTB();
            shotTB.setVideoid(idx);
            shotTB.setContent(filepath);
            shotTB.setStarttimecode(MapUtils.getString(shotinfo, "starttimecode"));
            shotTB.setStartframeindex(MapUtils.getIntValue(shotinfo, "startframeindex"));
            shotTB.setEndtimecode(MapUtils.getString(shotinfo, "endtimecode"));
            shotTB.setEndframeindex(MapUtils.getIntValue(shotinfo, "endframeindex"));
            shotTB.setAssetfilepath(FilenameUtils.getPath(filepath));
            shotTB.setAssetfilename(FilenameUtils.getName(filepath));
            Object object = shotinfo.get("object");
            if(object != null) {
                shotTB.setObject(JSONUtils.jsonStringFromObject(object));
            }
            storyboardService.putShotItem(shotTB);
        }

        if(StringUtils.equals(MapUtils.getString(shotinfo,"sequencetype"),"end")) {
            //end, status update
            LinkedHashMap<String, Object> param = new LinkedHashMap<>();
            param.put("idx", idx);
            param.put("catalogstatus", 3000);
//            param.put("catalogendtime", new Date());
            contentService.updateContentItem(param);
        }
        return "success";
    }


    public String callMamEx(String trName, int pool, String request) throws Exception {

        if(StringUtils.equals(trName,"ex_update_jobresult")) {
            return ex_update_jobresult(trName, pool, request);
        }

        String response = "";
        List<MamExCnfTb> mamExList = mamExMapper.selectMamEx(trName, pool);
        for (MamExCnfTb mamEx : mamExList) {
            try {
                String uri = String.format("%s/%s", mamEx.getUri(), trName);
                response = RestApiUtil.put(uri, request);
                mamEx.setFail(false);
                mamExMapper.updateMamEx(mamEx);
                return response;
            } catch (Exception ex) {
                mamEx.setFail(true);
                mamExMapper.updateMamEx(mamEx);
            }
        }

        // ToDo Throw error : no exist mamex

        return response;
    }

    public void createMamEx(String uri, int pool, List<String> trNameList) {
        mamExMapper.deleteMamEx(uri);
        for (String trName : trNameList) {
            mamExMapper.insertMamEx(uri, trName, pool);
        }
    }
}


