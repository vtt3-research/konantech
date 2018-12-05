package com.konantech.spring.service;

import com.fasterxml.jackson.core.type.TypeReference;
import com.fasterxml.jackson.databind.ObjectMapper;
import com.konantech.spring.domain.*;
import com.konantech.spring.mapper.OgqMapper;
import com.konantech.spring.util.JSONUtils;
import com.konantech.spring.util.RestUtils;
import net.sf.json.JSONArray;
import org.apache.commons.collections.MapUtils;
import org.apache.commons.collections.map.HashedMap;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.ResponseEntity;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Propagation;
import org.springframework.transaction.annotation.Transactional;

import java.io.IOException;
import java.util.*;

/**
 * Created by Violet on 2017-01-17.
 */

@Service
@Transactional(propagation = Propagation.REQUIRED, rollbackFor = Exception.class)
public class OgqService  {

    @Autowired
    private OgqMapper ogqMapper;


    public ItemData itemInfo(int idx) {
        return ogqMapper.itemInfo(idx);
    }

    public List<ItemData> itemList(Map<String, Object> param) {
        return ogqMapper.itemList(param);
    }

    public int itemInsert(ItemData param) {
        return ogqMapper.itemInsert(param);
    }

    public int itemUpdate(ItemData param) {
        return ogqMapper.itemUpdate(param);
    }

    public int itemDelete(int idx) {
        return ogqMapper.itemDelete(idx);
    }


    public Map<String,Object> getDarc4Info(int uuid) throws Exception {

        ItemData info = ogqMapper.itemInfo(uuid);
        int videoid = info.getVideoId();
        if(videoid == 0) {
            throw new Exception("콘텐츠가 등록되지 않았습니다");
        }
        ResponseEntity responseVideoInfo = RestUtils.darc4info(videoid);
        if(responseVideoInfo.getStatusCodeValue() != 200) {
            throw new Exception(responseVideoInfo.getHeaders().get("message").toString());
        }
        Map item = (Map<String,Object>) ((Map)responseVideoInfo.getBody()).get("item");
        return item;
    }

    public DarcStatus getStatus(Map<String,Object> item) throws Exception {

        DarcStatus darcStatus = new DarcStatus();
        darcStatus.setTranscodingstatus(MapUtils.getIntValue(item,"transcodingstatus"));
        darcStatus.setCatalogstatus(MapUtils.getIntValue(item,"catalogstatus"));
        darcStatus.setTranscodingPercent(0);
        darcStatus.setCatalogPercent(0);

        if(darcStatus.getTranscodingstatus() < 1000) {
            darcStatus.setTranscodingMsg("NONE");
        } else if(darcStatus.getTranscodingstatus() < 2000 ) {
            darcStatus.setTranscodingMsg("READY");
        } else if(darcStatus.getTranscodingstatus() < 3000 ) {
            darcStatus.setTranscodingMsg("WORKING");
            darcStatus.setTranscodingPercent(darcStatus.getTranscodingstatus() - 2000);
        } else if(darcStatus.getTranscodingstatus() < 4000 ) {
            darcStatus.setTranscodingMsg("SUCCESS");
            darcStatus.setTranscodingPercent(100);
        } else if(darcStatus.getTranscodingstatus() < 5000 ) {
            darcStatus.setTranscodingMsg("FAIL");
        } else if(darcStatus.getTranscodingstatus() < 6000 ) {
            darcStatus.setTranscodingMsg("RETRY");
        } else if(darcStatus.getTranscodingstatus() < 6000 ) {
            darcStatus.setTranscodingMsg("POSTPONE");
        }

        if(darcStatus.getCatalogstatus() < 1000) {
            darcStatus.setCatalogMsg("NONE");
        } else if(darcStatus.getCatalogstatus() < 2000 ) {
            darcStatus.setCatalogMsg("READY");
        } else if(darcStatus.getCatalogstatus() < 3000 ) {
            darcStatus.setCatalogMsg("WORKING");
            darcStatus.setCatalogPercent(darcStatus.getCatalogstatus() - 2000);
        } else if(darcStatus.getCatalogstatus() < 4000 ) {
            darcStatus.setCatalogMsg("SUCCESS");
            darcStatus.setCatalogPercent(100);
        } else if(darcStatus.getCatalogstatus() < 5000 ) {
            darcStatus.setCatalogMsg("FAIL");
        } else if(darcStatus.getCatalogstatus() < 6000 ) {
            darcStatus.setCatalogMsg("RETRY");
        } else if(darcStatus.getCatalogstatus() < 6000 ) {
            darcStatus.setCatalogMsg("POSTPONE");
        }
        return darcStatus;
    }
//
//
//    private List<String> SoGangParsing(String jsonString) throws Exception {
//        String label;
//        List<String> tags = new ArrayList<>();
//        List<String> totalTags = new ArrayList<>();
//
//        ObjectMapper mapper = new ObjectMapper();
//        HashMap<String, Object> map = mapper.readValue(jsonString, new HashMap<String, Object>().getClass());
//
//        for( Object ob : objects) {
//            HashMap<String,String> m = (HashMap<String, String>) ob;
//            label = m.get("label");
//            tags.add(m.get("label"));
//            if(!totalTags.contains(label)) {
//                totalTags.add(label);
//            }
//        }
//        return totalTags;
//    }

    public AssetsResponse getShotTgas(int videoid) throws Exception {

        List<String> tags;
        List<String> locations;
        List<String> totalTags = new ArrayList<>();
        String tag,location;

        AssetsResponse assetsResponse = new AssetsResponse();
        List<ShotInfo> shotInfos = new ArrayList<>();
        ResponseEntity responseShots = RestUtils.darc4shots(videoid);
        ArrayList<LinkedHashMap> shotItems = (ArrayList) ((Map) responseShots.getBody()).get("list");
        for (LinkedHashMap shot : shotItems) {

            int startframeindex = MapUtils.getIntValue(shot, "startframeindex");
            int endframeindex = MapUtils.getIntValue(shot, "endframeindex");
            String starttimecode = MapUtils.getString(shot, "starttimecode");
            String endtimecode = MapUtils.getString(shot, "endtimecode");
            String object = MapUtils.getString(shot, "object");
            String assetfilepath = MapUtils.getString(shot, "assetfilepath");
            String assetfilename = MapUtils.getString(shot, "assetfilename");
            tags = new ArrayList<>();
            locations = new ArrayList<>();

            // google
                /*
                LinkedList objects = (LinkedList) JSONUtils.jsonStringToObject(object, LinkedList.class);

                for( Object ob : objects) {
                    HashMap<String,String> m = (HashMap<String, String>) ob;
                    tag = m.get("tag");
                    tags.add(m.get("tag"));
                    if(!totalTags.contains(tag)) {
                        totalTags.add(tag);
                    }
                }
                */

            // SoGang
            SoGangParsing soGangParsing = new SoGangParsing(object);
            boolean first = true;
            for( SoGangParsing.ObjectInfo objectData : soGangParsing.getObjectData()) {
                tag = objectData.getInfo().getObjectName();
                location = objectData.getLocation();
                tags.add(tag);
                locations.add(location);
                if(first) {
                    if(!totalTags.contains(tag)) {
                        totalTags.add(tag);
                    }
                    first = false;
                }
            }
            ShotInfo shotInfo = new ShotInfo();
            shotInfo.setStartframeindex(startframeindex);
            shotInfo.setStarttimecode(starttimecode);
            shotInfo.setEndframeindex(endframeindex);
            shotInfo.setEndtimecode(endtimecode);
            shotInfo.setObject(object);
            shotInfo.setImage(assetfilepath + assetfilename);
            shotInfo.setTags(tags);
            shotInfos.add(shotInfo);
        }

        assetsResponse.setTags(totalTags);
        assetsResponse.setShots(shotInfos);
        return assetsResponse;
    }

    public ResponseEntity callback(ItemData item) throws Exception {

        String callbackUrl = item.getCallbackUrl();
        int idx = item.getIdx();
        Map<String, Object> info = this.getDarc4Info(idx);
        DarcStatus status = this.getStatus(info);
        if(status.getCatalogstatus() == 3000) {
            AssetsResponse assetsResponse = this.getShotTgas(idx);
            ResponseEntity responseEntity = RestUtils.callback(callbackUrl, assetsResponse);
            return responseEntity;
        } else {
            return null;
        }
    }

}
