package com.konantech.spring.service;

import com.konantech.spring.CommonTests;
import com.konantech.spring.domain.ItemData;
import com.konantech.spring.util.JSONUtils;
import com.konantech.spring.util.RestUtils;
import org.apache.commons.collections.MapUtils;
import org.apache.commons.collections.map.HashedMap;
import org.apache.commons.io.FilenameUtils;
import org.junit.Test;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.http.HttpMethod;
import org.springframework.http.ResponseEntity;
import org.springframework.transaction.annotation.Transactional;

import java.net.URI;
import java.net.URL;
import java.util.*;

/**
 * Created by ossboard on 28/08/2017.
 */
public class OgqBatchTest extends CommonTests {


    @Value("${darc4.restUrl}")
    public String restUrl;

    @Value("${darc4.token}")
    public String token;

    @Autowired
    private OgqService ogqService;

    @Test
    public void batch() throws Exception {

        Map<String, Object> param = new HashedMap();
        param.put("status", 0);
        URL url;
        List<ItemData> list = ogqService.itemList(param);
        for (ItemData item : list) {
            try {
                ResponseEntity responseEntity = RestUtils.darc4add(item);
                if (responseEntity != null) {
                    Map<String, Object> darc4items = (Map<String,Object>) ((Map)responseEntity.getBody()).get("item");
                    int videoid = MapUtils.getIntValue(darc4items, "idx");
                    item.setVideoId(videoid);
                    item.setStatus(1);
//                RestUtils.darc4workflow(videoid);
                    ogqService.itemUpdate(item);
                } else {
                    item.setStatus(-2);
                    ogqService.itemUpdate(item);
                }
            } catch (Exception e) {
                item.setStatus(-5);
                ogqService.itemUpdate(item);
            }

        }
    }

    @Test
    public void callbackTest() throws Exception {

        Map<String, Object> param = new HashedMap();
        param.put("status", 2);
        List<ItemData> list = ogqService.itemList(param);
        for (ItemData item : list) {
            try {
                ResponseEntity responseEntity = ogqService.callback(item);
                if (responseEntity != null) {
                    if (responseEntity.getStatusCodeValue() == 200) {
                        item.setStatus(2);
                        ogqService.itemUpdate(item);
                    } else {
                        item.setStatus(responseEntity.getStatusCodeValue());
                        ogqService.itemUpdate(item);
                    }
                }
            } catch (Exception ignore) {
                //ignore
                item.setStatus(-3);
                ogqService.itemUpdate(item);
            }
        }
    }

    @Test
    public void jsonTest() throws Exception {
        String body = "{\"filed\":null,\"item\":{\"assetfilepath\":\"http://s3.amazon.com/media/image/2017-06/te33033.mp4\",\"content\":\"1sdv23d4234\",\"idx\":8,\"orifilename\":\"te33033.mp4\",\"title\":\"1sdv23d4234\"}},{Access-Control-Allow-Origin=[*], Access-Control-Allow-Methods=[POST, GET, PUT, OPTIONS, DELETE], Access-Control-Max-Age=[3600], Access-Control-Allow-Headers=[x-auth-token, authorization, content-type], X-Application-Context=[vtt-slim-darc:ogq], Content-Type=[application/json;charset=UTF-8], Transfer-Encoding=[chunked], Date=[Mon, 05 Mar 2018 11:41:37 GMT]}";
        System.out.println(body);
        Map a = JSONUtils.jsonStringToMap(body);
        Map item = (LinkedHashMap) a.get("item");
        System.out.println(item);

    }

}